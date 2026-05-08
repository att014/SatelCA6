#include "satel_ca6.h"
    z4_sensor_->publish_state(z4);

  if (z5_sensor_ != nullptr)
    z5_sensor_->publish_state(z5);

  if (z6_sensor_ != nullptr)
    z6_sensor_->publish_state(z6);

  if (z7_sensor_ != nullptr)
    z7_sensor_->publish_state(z7);

  if (trouble_sensor_ != nullptr)
    trouble_sensor_->publish_state(trouble);

  std::string state = "disarmed";

  if (trouble)
    state = "triggered";
  else if (z1 || z2 || z3 || z4 || z5 || z6 || z7)
    state = "armed_away";

  if (state_sensor_ != nullptr)
    state_sensor_->publish_state(state);
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

  f |= (key & 0xF) << 8;

  return f;
}

void SatelCA6::arm() {
  ESP_LOGI(TAG, "ARM");

  tx_frame_ = encode_key_(1);
  transmitting_ = true;
  tx_bit_ = 0;
}

void SatelCA6::disarm() {
  ESP_LOGI(TAG, "DISARM");

  tx_frame_ = encode_key_(2);
  transmitting_ = true;
  tx_bit_ = 0;
}

}  // namespace satel_ca6
}  // namespace esphome
