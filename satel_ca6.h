#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

#include "driver/rmt.h"

namespace esphome {
namespace satel_ca6 {

class SatelCA6 : public Component {
 public:
  SatelCA6(int clk, int data);

  void setup() override;
  void loop() override;

  void arm();
  void disarm();

  void set_z1_sensor(binary_sensor::BinarySensor *s) { z1_sensor_ = s; }
  void set_z2_sensor(binary_sensor::BinarySensor *s) { z2_sensor_ = s; }
  void set_z3_sensor(binary_sensor::BinarySensor *s) { z3_sensor_ = s; }
  void set_z4_sensor(binary_sensor::BinarySensor *s) { z4_sensor_ = s; }
  void set_z5_sensor(binary_sensor::BinarySensor *s) { z5_sensor_ = s; }
  void set_z6_sensor(binary_sensor::BinarySensor *s) { z6_sensor_ = s; }
  void set_z7_sensor(binary_sensor::BinarySensor *s) { z7_sensor_ = s; }

  void set_trouble_sensor(binary_sensor::BinarySensor *s) {
    trouble_sensor_ = s;
  }

  void set_state_sensor(text_sensor::TextSensor *s) {
    state_sensor_ = s;
  }

 protected:
  int clk_pin_;
  int data_pin_;

  RingbufHandle_t rb_ = nullptr;

  uint32_t frame_ = 0;
  uint8_t bit_count_ = 0;

  uint32_t tx_frame_ = 0;
  bool transmitting_ = false;
  uint8_t tx_bit_ = 0;

  binary_sensor::BinarySensor *z1_sensor_ = nullptr;
  binary_sensor::BinarySensor *z2_sensor_ = nullptr;
  binary_sensor::BinarySensor *z3_sensor_ = nullptr;
  binary_sensor::BinarySensor *z4_sensor_ = nullptr;
  binary_sensor::BinarySensor *z5_sensor_ = nullptr;
  binary_sensor::BinarySensor *z6_sensor_ = nullptr;
  binary_sensor::BinarySensor *z7_sensor_ = nullptr;

  binary_sensor::BinarySensor *trouble_sensor_ = nullptr;

  text_sensor::TextSensor *state_sensor_ = nullptr;

  void handle_edge_();
  void process_frame_(uint32_t f);
  void send_bit_();
  uint32_t encode_key_(uint8_t key);
};

}  // namespace satel_ca6
}  // namespace esphome
