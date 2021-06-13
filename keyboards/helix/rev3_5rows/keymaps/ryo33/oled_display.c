/* Copyright 2020 mtei
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
#include <stdio.h>
#ifdef CONSOLE_ENABLE
  #include "print.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif
#include <string.h>
#include "layer_number.h"

extern int current_default_layer;

void init_helix_oled(void) {
#ifdef SSD1306OLED
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    iota_gfx_init(!has_usb());   // turns on the display
#endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#if defined(SSD1306OLED) || defined(OLED_DRIVER_ENABLE)

#    if defined(OLED_DRIVER_ENABLE)
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_0;
    } else {
        return OLED_ROTATION_180;
    }
}
#    else
#    define oled_write(data,flag)    matrix_write(matrix, data)
#    define oled_write_P(data,flag)  matrix_write_P(matrix, data)
#    endif

#    ifdef SSD1306OLED
void matrix_scan_user(void) {
    iota_gfx_task();  // this is what updates the display continuously
}

void matrix_update(struct CharacterMatrix *dest,
                   const struct CharacterMatrix *source) {
    if (memcmp(dest->display, source->display, sizeof(dest->display))) {
        memcpy(dest->display, source->display, sizeof(dest->display));
        dest->dirty = true;
    }
}
#    endif

#    ifdef SSD1306OLED
static void render_logo(struct CharacterMatrix *matrix) {
#    else
static void render_logo(void) {
#    endif

    static const char helix_logo[] PROGMEM = {
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,
        0};
    oled_write_P(helix_logo, false);
#    ifdef RGBLIGHT_ENABLE
    char buf[30];
    if (RGBLIGHT_MODES > 1 && rgblight_is_enabled()) {
        snprintf(buf, sizeof(buf), " LED %2d: %d,%d,%d ",
                 rgblight_get_mode(),
                 rgblight_get_hue()/RGBLIGHT_HUE_STEP,
                 rgblight_get_sat()/RGBLIGHT_SAT_STEP,
                 rgblight_get_val()/RGBLIGHT_VAL_STEP);
        oled_write(buf, false);
#        ifndef SSD1306OLED
    } else {
        oled_write_P( PSTR("\n"), false);
#        endif
    }
#    endif
}

// static const char Qwerty_name[]  PROGMEM = " Qwerty";
static const char Colemak_name[] PROGMEM = "Colemak";
static const char Number_name[]   PROGMEM = ":Number";
static const char Double_name[]   PROGMEM = ":Double";
static const char Adjust_name[]  PROGMEM = ":Adjust";
static const char US_name[] PROGMEM = "US";
static const char JP_name[] PROGMEM = "JIS";

static const char *layer_names[] = {
    [_COLEMAK] = Colemak_name,
    [_NUMBER]  = Number_name,
    [_DOUBLE]  = Double_name,
    [_ADJUST] = Adjust_name
};

static const char *host_layout_name[] = {
    [Colemak] = Colemak_name,
    [US] = US_name,
    [JP] = JP_name,
};

#    ifdef SSD1306OLED
void render_status(struct CharacterMatrix *matrix) {
#    else
void render_status(void) {
#    endif

    // Render to mode icon
    static const char os_logo[][2][3] PROGMEM ={{{0x95,0x96,0},{0xb5,0xb6,0}},{{0x97,0x98,0},{0xb7,0xb8,0}}};
    if(keymap_config.swap_lalt_lgui==false){
        oled_write_P(os_logo[0][0], false);
        oled_write_P(PSTR("\n"), false);
        oled_write_P(os_logo[0][1], false);
    } else {
        oled_write_P(os_logo[1][0], false);
        oled_write_P(PSTR("\n"), false);
        oled_write_P(os_logo[1][1], false);
    }

    // Define layers here, Have not worked out how to have text displayed for each layer. Copy down the number you see and add a case for it below
    oled_write_P(PSTR(" "), false);
    oled_write_P(layer_names[current_default_layer], false);
    oled_write_P(PSTR("/"), false);
    oled_write_P(host_layout_name[HOST_LAYOUT], false);
    oled_write_P(PSTR("\n"), false);

    if (NUMBER_PRESSED == 1) {
        oled_write_P(layer_names[_NUMBER], false);
    }
    if (NUMBER_PRESSED == 2) {
        oled_write_P(layer_names[_DOUBLE], false);
    }
    if (layer_state >> _ADJUST & 1) {
        oled_write_P(layer_names[_ADJUST], false);
    }

    // Host Keyboard LED Status
    char led[40];
    snprintf(led, sizeof(led), "\n%s  %s  %s",
             (host_keyboard_leds() & (1<<USB_LED_NUM_LOCK)) ? "NUMLOCK" : "       ",
             (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK)) ? "CAPS" : "    ",
             (host_keyboard_leds() & (1<<USB_LED_SCROLL_LOCK)) ? "SCLK" : "    ");
    oled_write(led, false);
}

#    ifdef SSD1306OLED
#        if OLED_UPDATE_INTERVAL > 0
uint16_t oled_update_timeout;
#        endif

void iota_gfx_task_user(void) {
    struct CharacterMatrix matrix;

#        if DEBUG_TO_SCREEN
    if (debug_enable) {
        return;
    }
#        endif

#if      OLED_UPDATE_INTERVAL > 0
    if (timer_elapsed(oled_update_timeout) < OLED_UPDATE_INTERVAL) {
        return;
    }
    oled_update_timeout = timer_read();
#endif
    matrix_clear(&matrix);
    if (is_keyboard_master()) {
        render_status(&matrix);
    } else {
        render_logo(&matrix);
    }
    matrix_update(&display, &matrix);
}
#    else
void oled_task_user(void) {

#        if DEBUG_TO_SCREEN
    if (debug_enable) {
        return;
    }
#        endif

    if(is_keyboard_master()){
        render_status();
    }else{
        render_logo();
    }
}
#    endif

#endif
