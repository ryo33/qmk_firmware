#pragma once

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

// Defines names for use in layer keycodes and the keymap
enum layer_names {
  _COLEMAK = 0,
  _NUMBER,
  _DOUBLE,
  _ADJUST
};

enum host_layout {
  JP = 0,
  US,
  Colemak
};

extern int8_t HOST_LAYOUT;
extern int8_t NUMBER_PRESSED;

#if defined(SSD1306OLED)
extern void init_helix_oled(void);
#   define INIT_HELIX_OLED() init_helix_oled();
#else
#   define INIT_HELIX_OLED()
#endif
