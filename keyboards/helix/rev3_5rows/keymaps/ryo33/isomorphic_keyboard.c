#include QMK_KEYBOARD_H
#include "isomorphic_keyboard.h"
#define ACTUAL_ROWS (MATRIX_ROWS / 2)
#define SECOND 2
#define THIRD 4  
#define FIFTH 7 

uint16_t midi_origin_keycode = MI_C_2;
uint8_t midi_row_gap = THIRD;
int8_t midi_row_transpose = 0;
int8_t midi_col_transpose = -1;

void compute_colrow(int8_t *col, int8_t *row, uint8_t original_col, uint8_t original_row) {
  bool left_hand = original_row < ACTUAL_ROWS;
  if (left_hand) {
    *col = original_col;
  } else {
    // right hand is reversed.
    *col = (MATRIX_COLS - 1) - original_col;
  }
  // swaps left and right.
  *col += (left_hand * MATRIX_COLS);

  // (ACTUAL_ROWS - 1) - ...: makes the row order ascending from the bottom.
  // (original_row % ACTUAL_ROWS): removes right hand's offset. 
  *row = (ACTUAL_ROWS - 1) - (original_row % ACTUAL_ROWS);
}

void midi_transpose_colrow(uint8_t original_col, uint8_t original_row) {
  int8_t col;
  int8_t row;
  compute_colrow(&col, &row, original_col, original_row);
  midi_col_transpose = -col;
  midi_row_transpose = -row;
}

uint16_t midi_keycode_from_colrow(uint8_t original_col, uint8_t original_row) {
  int8_t col;
  int8_t row;
  compute_colrow(&col, &row, original_col, original_row);
  col += midi_col_transpose;
  row += midi_row_transpose;
  int8_t tone = row * midi_row_gap + (col % 2) * FIFTH + (col / 2) * SECOND;
  uint16_t keycode = midi_origin_keycode + tone;
  if (keycode < MIDI_TONE_MIN || MIDI_TONE_MAX < keycode) {
    return 0;
  }
  return keycode;
}