#include "satel_ca6.h"
#include "esphome/core/log.h"
#include "esphome/components/mqtt/mqtt_client.h"

namespace esphome {
namespace satel_ca6 {

static const char *TAG = "satel_ca6";

extern mqtt::MQTTClient *global_mqtt_client;

SatelCA6::SatelCA6(int clk, int data) {
  clk_pin_ = clk;
  data_pin_ = data;
}

void SatelCA6::setup() {
  pinMode(data_pin_, INPUT_PULLUP);

  rmt_config_t config;
  config.rmt_mode = RMT_MODE_RX;
  config.channel = RMT_CHANNEL_0;
  config.gpio_num = (gpio_num_t)clk_pin_;
  config.clk_div = 80;
  config.mem_block_num = 1;

  config.rx_config.filter_en = true;
  config.rx_config.filter_ticks_thresh = 10;
  config.rx_config.idle_threshold = 1000;

  rmt_config(&config);
  rmt_driver_install(config.channel, 1000, 0);

  rmt_get_ringbuf_handle(config.channel, &rb_);
  rmt_rx_start(config.channel, true);

  ESP_LOGI(TAG, "Satel CA-6 initialized");
}

void SatelCA6::loop() {
  size_t length = 0;
  auto *items = (rmt_item32_t *) xRingbufferReceive(rb_, &length, 0);

  if (items) {
    int num = length / sizeof(rmt_item32_t);

    for (int i = 0; i < num; i++) {
      handle_edge_();
    }

    vRingbufferReturnItem(rb_, (void *) items);
  }
}

void SatelCA6::handle_edge_() {
  bool bit = digitalRead(data_pin_);

  frame_ <<= 1;
  if (bit) frame_ |= 1;

  bit_count_++;

  if (transmitting_) {
    send_bit_();
  }

  if (bit_count_ == 32) {
    process_frame_(frame_);
    frame_ = 0;
    bit_count_ = 0;
  }
}

void SatelCA6::process_frame_(uint32_t f) {
  ESP_LOGD(TAG, "frame: %08X", f);

  // --- MAPOWANIE ---
  bool z1 = f & (1UL << 31);
  bool z2 = f & (1UL << 29);
  bool z3 = f & (1UL << 25);
  bool z4 = f & (1UL << 27);
  bool z5 = f & (1UL << 15);
  bool z6 = f & (1UL << 13);
  bool z7 = (f & (1UL << 11)) || (f & (1UL << 20));

  bool trouble = f & (1UL << 19);

  // --- MQTT ---
  if (global_mqtt_client != nullptr) {
    global_mqtt_client->publish("home/alarm/z1", z1 ? "ON" : "OFF");
    global_mqtt_client->publish("home/alarm/z2", z2 ? "ON" : "OFF");

    std::string state = "disarmed";
    if (trouble) state = "triggered";
    else if (z1 || z2 || z3 || z4 || z5 || z6 || z7)
      state = "armed_away";

    global_mqtt_client->publish("home/alarm/state", state);
  }
}

void SatelCA6::send_bit_() {
  bool bit = tx_frame_ & (1UL << (31 - tx_bit_));

  if (!bit) {
    pinMode(data_pin_, OUTPUT);
    digitalWrite(data_pin_, LOW);
  } else {
    pinMode(data_pin_, INPUT);
  }

  tx_bit_++;

  if (tx_bit_ >= 32) {
    transmitting_ = false;
    tx_bit_ = 0;
    pinMode(data_pin_, INPUT);
  }
}

uint32_t SatelCA6::encode_key_(uint8_t key) {
  uint32_t f = 0;
  f |= (key & 0xF) << 8;  // DO DOPASOWANIA
  return f;
}

void SatelCA6::arm() {
  ESP_LOGI(TAG, "ARM");
  tx_frame_ = encode_key_(1); // przykad
  transmitting_ = true;
  tx_bit_ = 0;
}

void SatelCA6::disarm() {
  ESP_LOGI(TAG, "DISARM");
  tx_frame_ = encode_key_(2); // przykad
  transmitting_ = true;
  tx_bit_ = 0;
}

}  // namespace satel_ca6
}  // namespace esphome
