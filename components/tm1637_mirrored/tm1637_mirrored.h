#pragma once

#include "esphome/core/component.h"
#include "esphome/components/tm1637/tm1637.h"
#include "esphome/components/tm1637/tm1637_display.h"
#include "esphome/components/display/text_display.h"

namespace esphome {
namespace tm1637_mirrored {

class TM1637MirroredDisplay : public tm1637::TM1637Display {
 public:
  void setup() override;
  
  // Enable/disable segment mirroring at runtime
  void set_mirror_segments(bool mirror) { mirror_enabled_ = mirror; }
  bool get_mirror_segments() const { return mirror_enabled_; }
  
  // Enable/disable digit order reversal at runtime  
  void set_reverse_digits(bool reverse) { reverse_digits_ = reverse; }
  bool get_reverse_digits() const { return reverse_digits_; }
  
  // Set/update interval helper
  void set_update_interval(uint32_t interval_ms) {
    this->set_update_interval(interval_ms / 1000.0f);
  }

 protected:
  bool mirror_enabled_{true};
  bool reverse_digits_{true};
  
  // Override write_buffer to apply transformations
  void write_buffer(const uint8_t *data, uint8_t length) override;
  
  // Helper to convert digit to mirrored segment pattern
  uint8_t digit_to_mirrored_segment(uint8_t digit);
  
  // Standard segment map (a=LSB, g=bit6, dp=bit7)
  static const uint8_t SEGMENT_MAP[10];
  
  // Mirrored segment map for floor projection
  static const uint8_t MIRRORED_SEGMENT_MAP[10];
};

}  // namespace tm1637_mirrored
}  // namespace esphome