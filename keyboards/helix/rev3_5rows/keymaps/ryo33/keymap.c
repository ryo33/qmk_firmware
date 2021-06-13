/* Copyright 2020 yushakobo
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "layer_number.h"
#include "colemak_translation.h"
#include "a2j/translate_ansi_to_jis.h"

#define TMUX LCTL(KC_B)
#define LSTAB LSFT(KC_TAB)
#define ADJUST MO(_ADJUST)
#define LGU_ENT LGUI_T(KC_ENT)
#define LCT_SPC LCTL_T(KC_SPC)
#define LSF_SPC LSFT_T(KC_SPC)
#define RGU_ENT RGUI_T(KC_ENT)
#define RCT_SPC RCTL_T(KC_SPC)
#define RSF_SPC RSFT_T(KC_SPC)



// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
  EISU = SAFE_RANGE,
  KANA,
  RGBRST,
  HCOLEMK,
  HUS,
  HJP,
  NUMBER
};

int8_t NUMBER_PRESSED = 0;

int8_t HOST_LAYOUT = Colemak;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_COLEMAK] = LAYOUT( \
      KC_Q   , KC_W,    KC_F,    KC_P,    KC_G,    XXXXXXX,                   KC_ZKHK, KC_J,    KC_L,    KC_U,    KC_Y,    XXXXXXX, \
      KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    XXXXXXX,                   XXXXXXX, KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    \
      KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    LSTAB  ,                   KC_TAB , KC_K,    KC_M,    KC_COMM, KC_DOT , KC_SLSH, \
      KC_LALT, TMUX   , KC_ESC , KC_BSPC, LCT_SPC, LGU_ENT, EISU   , KANA   , RGU_ENT, RCT_SPC, KC_BSPC, KC_ESC , KC_UP,   KC_RALT, \
      ADJUST , XXXXXXX, XXXXXXX, NUMBER , LSF_SPC, LGU_ENT, KC_ESC , KC_ESC , RGU_ENT, RSF_SPC, NUMBER , KC_LEFT, KC_DOWN, KC_RGHT  \
      ),

  [_NUMBER] = LAYOUT( \
      KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, _______,                   _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_MINS, KC_EQL , \
      KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , _______,                   _______, KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , \
     KC_GRAVE, KC_TILD, KC_LBRC, KC_LCBR, KC_LPRN, KC_PGUP,                   KC_PGDN, KC_RPRN, KC_RCBR, KC_RBRC, KC_UNDS, KC_PLUS, \
      _______, _______, _______, KC_DEL , _______, _______, _______, _______, _______, _______, KC_DEL , _______, _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
      ),

  [_DOUBLE] = LAYOUT( \
      _______, _______, KC_LT  , KC_BSLS, _______, _______,                   _______, _______, KC_PIPE, KC_GT  , _______, _______, \
      _______,KC_GRAVE, KC_EXLM, KC_QUOT, _______, _______,                   _______, _______, KC_DQT , KC_QUES, KC_TILD, _______, \
      _______, KC_COMM, KC_MINS, KC_SCLN, _______, _______,                   _______, _______, KC_COLN, KC_EQL , KC_DOT , _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  \
      ),

  [_ADJUST] =  LAYOUT( \
      KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, \
      HCOLEMK, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
      HUS    , _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, \
      HJP    , _______, _______, _______, _______, _______, RESET  , KC_PWR , KC_WAKE, _______, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, \
      _______, _______, _______, KC_CAPS, KC_NLCK, KC_SLCK, _______, _______, _______, RGBRST , RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD \
      ) 

};

int current_default_layer;

uint32_t default_layer_state_set_user(uint32_t state) {
    current_default_layer = biton32(state);
    return state;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* Left side encoder */
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    } else if (index == 1) { /* Right side encoder */
        if (clockwise) {
            tap_code(KC_DOWN);
        } else {
            tap_code(KC_UP);
        }
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (HOST_LAYOUT == Colemak && !translate_colemak_to_ansi(&keycode)) {
    if (record->event.pressed) {
      register_code16(keycode);
    } else {
      unregister_code16(keycode);
    }
    return false;
  }
  if (HOST_LAYOUT == JP && !process_record_user_a2j(keycode, record)) {
    return false;
  }
  switch (keycode) {
    case HCOLEMK:
      if (record->event.pressed) {
        HOST_LAYOUT = Colemak;
      }
      return false;
      break;
    case HUS:
      if (record->event.pressed) {
        HOST_LAYOUT = US;
      }
      return false;
      break;
    case HJP:
      if (record->event.pressed) {
        HOST_LAYOUT = JP;
      }
      return false;
      break;
    case NUMBER:
      if (record->event.pressed) {
        NUMBER_PRESSED ++;
        if (NUMBER_PRESSED == 1) {
          layer_on(_NUMBER);
        } else if (NUMBER_PRESSED == 2) {
          layer_on(_DOUBLE);
        }
      } else {
        NUMBER_PRESSED --;
        if (NUMBER_PRESSED == 0) {
          layer_off(_NUMBER);
        } else if (NUMBER_PRESSED == 1) {
          layer_off(_DOUBLE);
        }
      }
      return false;
      break;

    case EISU:
      if (record->event.pressed) {
        if (is_mac_mode()) {
          register_code(KC_LANG2);
        }else{
          tap_code16(LALT(KC_GRAVE));
        }
      } else {
        unregister_code(KC_LANG2);
      }
      return false;
      break;
    case KANA:
      if (record->event.pressed) {
        if (is_mac_mode()) {
          register_code(KC_LANG1);
        }else{
          tap_code16(LALT(KC_GRAVE));
        }
      } else {
        unregister_code(KC_LANG1);
      }
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
        }
      #endif
      break;
  }
  return true;
}


void matrix_init_user(void) {
    INIT_HELIX_OLED(); /* define in layer_number.h */
}