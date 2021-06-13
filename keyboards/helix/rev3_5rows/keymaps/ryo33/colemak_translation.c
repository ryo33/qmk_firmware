#include QMK_KEYBOARD_H
#include "colemak_translation.h"

bool translate_colemak_to_ansi(uint16_t *keycode) {
  switch (*keycode) {
    case KC_F   : *keycode = KC_E;    break;
    case KC_P   : *keycode = KC_R;    break;
    case KC_G   : *keycode = KC_T;    break;
    case KC_J   : *keycode = KC_Y;    break;
    case KC_L   : *keycode = KC_U;    break;
    case KC_U   : *keycode = KC_I;    break;
    case KC_Y   : *keycode = KC_O;    break;
    case KC_SCLN: *keycode = KC_P;    break;
    case LSFT(KC_SCLN): *keycode = LSFT(KC_P); break;
    case RSFT(KC_SCLN): *keycode = RSFT(KC_P); break;
    case KC_R   : *keycode = KC_S;    break;
    case KC_S   : *keycode = KC_D;    break;
    case KC_T   : *keycode = KC_F;    break;
    case KC_D   : *keycode = KC_G;    break;
    case KC_N   : *keycode = KC_J;    break;
    case KC_E   : *keycode = KC_K;    break;
    case KC_I   : *keycode = KC_L;    break;
    case KC_O   : *keycode = KC_SCLN; break;
    case KC_K   : *keycode = KC_N;    break;
    default:
      return true;
  }
  return false;
}