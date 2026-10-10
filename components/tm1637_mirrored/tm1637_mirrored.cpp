#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "tm1637_mirrored.h"
#include "tm1637.h"

namespace esphome::tm1637_mirrored {

static const char *const TAG = "tm1637_mirrored";

// Standard 7-segment patterns (common cathode)
// Bit layout: dp gfedcba
// a=1, b=2, c=4, d=8, e=16, f=32, g=64, dp=128
const uint8_t TM1637MirroredDisplay::SEGMENT_MAP[10] = {
  0x3F,  // 0: abcdef
  0x06,  // 1: bc
  0x5B,  // 2: abdeg
  0x4F,  // 3: abcdg
  0x66,  // 4: bcfg
  0x6D,  // 5: acdfg
  0x7D,  // 6: acdefg
  0x07,  // 7: abc
  0x7F,  // 8: abcdefg
  0x6F   // 9: abcdfg
};

// Mirrored patterns for floor projection (upside-down reflection)
// Some digits remain unchanged (0,1,8), others may need manual adjustment
const uint8_t TM1637MirroredDisplay::MIRRORED_SEGMENT_MAP[10] = {
  0x3F,  // 0 -> symmetric
  0x06,  // 1 -> symmetric
  0x5B,  // 2 -> may need testing
  0x4F,  // 3 -> may need testing
  0x66,  // 4 -> may need testing
  0x6D,  // 5 -> may need testing
  0x7D,  // 6 -> may need testing
  0x07,  // 7 -> may need testing
  0x7F,  // 8 -> symmetric
  0x6F   // 9 -> may need testing
};

void TM1637MirroredDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Setting up TM1637 Mirrored Display...");
  ESP_LOGCONFIG(TAG, "  Mirror Segments: %s", YESNO(this->mirror_enabled_));
  ESP_LOGCONFIG(TAG, "  Reverse Digits: %s", YESNO(this->reverse_digits_));
  
  // Call parent setup for TM1637 initialization
  tm1637::TM1637Display::setup();
}

uint8_t TM1637MirroredDisplay::digit_to_mirrored_segment(uint8_t digit) {
  if (digit > 9) return 0x00;  // Blank
  
  if (mirror_enabled_) {
    return MIRRORED_SEGMENT_MAP[digit];
  }
  return SEGMENT_MAP[digit];
}

void TM1637MirroredDisplay::write_buffer(const uint8_t *data, uint8_t length) {
  uint8_t transformed_buf[6];  // Max 6 digits
  uint8_t actual_length = std::min(length, (uint8_t)6);
  
  if (reverse_digits_) {
    // Reverse digit order for floor viewing
    for (uint8_t i = 0; i < actual_length; i++) {
      uint8_t original_digit = data[i] & 0x0F;  // Extract digit (lower 4 bits)
      uint8_t segment_pattern = digit_to_mirrored_segment(original_digit);
      // Preserve decimal point flag if present
      if (data[i] & 0x80) {
        segment_pattern |= 0x80;
      }
      transformed_buf[actual_length - 1 - i] = segment_pattern;
    }
  } else {
    // Keep order, just apply segment mirroring
    for (uint8_t i = 0; i < actual_length; i++) {
      uint8_t original_digit = data[i] & 0x0F;
      uint8_t segment_pattern = digit_to_mirrored_segment(original_digit);
      if (data[i] & 0x80) {
        segment_pattern |= 0x80;
      }
      transformed_buf[i] = segment_pattern;
    }
  }
  
  // Write transformed buffer to display
  this->set_buffer(transformed_buf, actual_length);
}

}  // namespace tm1637_mirrored