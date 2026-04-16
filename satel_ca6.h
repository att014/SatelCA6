#pragma once

#include "esphome/core/component.h"
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

 protected:
  int clk_pin_;
  int data_pin_;

  RingbufHandle_t rb_ = NULL;

  uint32_t frame_ = 0;
  uint8_t bit_count_ = 0;

  // TX
  uint32_t tx_frame_ = 0;
  bool transmitting_ = false;
  uint8_t tx_bit_ = 0;

  void handle_edge_();
  void process_frame_(uint32_t f);
  void send_bit_();
  uint32_t encode_key_(uint8_t key);
};

}  // namespace satel_ca6
}  // namespace esphome
