#pragma once

#include "esphome/core/component.h"
#include "esphome/components/display/text_display.h"
#include "esphome/components/tm1637/tm1637_display.h"

namespace esphome {
namespace tm1637_mirrored {

class TM1637MirroredDisplay : public tm1637::TM1637Display {
 public:
  void setup() override;
  
  // Enable/disable segment mirroring at runtime
  void set_mirror_segments(bool mirror) { mirror_enabled_ = mirror; }
  
  // Enable/disable digit order reversal at runtime  
  void set_reverse_digits(bool reverse) { reverse_digits_ = reverse; }

 protected:
  bool mirror_enabled_{false};
  bool reverse_digits_{false};
  
  // Override render method to apply transformations
  void render(const char *text) override;
  
  // Helper to convert digit to mirrored segment pattern
  uint8_t digit_to_mirrored_segment(uint8_t digit);
};

}  // namespace tm1637_mirrored
}  // namespace esphome