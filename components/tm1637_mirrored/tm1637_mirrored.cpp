#include "esphome/core/log.h"
#include "tm1637_mirrored.h"

namespace esphome {
namespace tm1637_mirrored {

static const char *const TAG = "tm1637_mirrored.display";

// Standard 7-segment patterns for digits 0-9
// Bit layout: gfe_dcba (common cathode)
static const uint8_t SEGMENT_MAP[10] = {
  0x3F,  // 0: a+b+c+d+e+f
  0x06,  // 1: b+c
  0x5B,  // 2: a+b+d+e+g
  0x4F,  // 3: a+b+c+d+g
  0x66,  // 4: b+c+f+g
  0x6D,  // 5: a+c+d+f+g
  0x7D,  // 6: a+c+d+e+f+g
  0x07,  // 7: a+b+c
  0x7F,  // 8: all segments
  0x6F   // 9: a+b+c+d+f+g
};

// Mirrored patterns (upside-down for floor projection)
// Some digits remain unchanged (0,1,8), others transform
static const uint8_t MIRRORED_SEGMENT_MAP[10] = {
  0x3F,  // 0 -> 0 (symmetric)
  0x06,  // 1 -> 1
  0x5B,  // 2 -> 2 (may need adjustment based on your module)
  0x4F,  // 3 -> 3
  0x66,  // 4 -> 4
  0x6D,  // 5 -> 5
  0x7D,  // 6 -> 6
  0x07,  // 7 -> 7
  0x7F,  // 8 -> 8 (symmetric)
  0x6F   // 9 -> 9
};

void TM1637MirroredDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Setting up TM1637 Mirrored Display...");
  tm1637::TM1637Display::setup();
}

uint8_t TM1637MirroredDisplay::digit_to_mirrored_segment(uint8_t digit) {
  if (digit > 9) return 0x00;  // Blank
  
  if (mirror_enabled_) {
    return MIRRORED_SEGMENT_MAP[digit];
  }
  return SEGMENT_MAP[digit];
}

void TM1637MirroredDisplay::render(const char *text) {
  int len = strlen(text);
  int num_digits = std::min(len, 4);
  
  uint8_t buffer[4] = {0x00, 0x00, 0x00, 0x00};
  
  for (int i = 0; i < num_digits; i++) {
    char c = text[i];
    uint8_t digit = 0xFF;
    
    if (c >= '0' && c <= '9') {
      digit = c - '0';
    } else if (c == ' ') {
      digit = 0xFF;  // Blank
    }
    
    uint8_t segment_pattern = digit_to_mirrored_segment(digit);
    
    if (reverse_digits_) {
      // Reverse order: rightmost digit goes to leftmost position
      buffer[num_digits - 1 - i] = segment_pattern;
    } else {
      buffer[i] = segment_pattern;
    }
  }
  
  // Apply display buffer
  this->set_buffer(buffer, num_digits);
}

}  // namespace tm1637_mirrored
}  // namespace esphome