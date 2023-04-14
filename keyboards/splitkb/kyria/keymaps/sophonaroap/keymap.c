/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
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

bool is_cmd_tab_active = false;

uint16_t cmd_tab_timer = 0;

enum layers {
   _DEFAULT = 0,
   _NUM_PUNCT,
   _FUNCTION,
   _WINDOW,
};

enum tap_dance {
   TAB_ESCAPE,
};

enum my_keycodes {
   AT_EXCLM = SAFE_RANGE,
   COL_TO_SEMI,
   UNDER_TO_HY,
};

tap_dance_action_t tap_dance_actions[] = {
   [TAB_ESCAPE] = ACTION_TAP_DANCE_DOUBLE(KC_TAB, KC_ESC),
};

/*
*  Cntrl / opt in bad spots
*  I feel like I want function and arrow keys on same layer?
*  fix number types
*  put punctuation only under numbers w/shift
*/

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
* Base Layer: Default
*
* ,-------------------------------------------.                              ,-------------------------------------------.
* |  @/!   |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  \ |   |
* |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
* |Tab /Esc|   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |  ' "   |
* |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
* | CMD    |   Z  |   X  |   C  |   V  |   B  |funct |window|  |num/p |home  |   N  |   M  | ,  < | . >  |      | /  ?   |
* `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
*                        |ENC   | cntrl| opt  | shift|enter |  | space| shift| del  | ` ~  | end  |
*                        |      |      |      |      |      |  |      |      |      |      |      |
*                        `----------------------------------'  `----------------------------------'
*/
   [_DEFAULT] = LAYOUT(
    AT_EXCLM      , KC_Q ,  KC_W ,    KC_E  ,   KC_R ,   KC_T ,                                                                   KC_Y,   KC_U,   KC_I ,   KC_O ,   KC_P     , KC_BSLS,
    TD(TAB_ESCAPE), KC_A ,  KC_S ,    KC_D  ,   KC_F ,   KC_G ,                                                                   KC_H,   KC_J,   KC_K ,   KC_L , COL_TO_SEMI, KC_QUOT,
    KC_LGUI       , KC_Z ,  KC_X ,    KC_C  ,   KC_V ,   KC_B ,  TT(_FUNCTION), TT(_WINDOW),     TT(_NUM_PUNCT), TO(_DEFAULT),    KC_N,   KC_M, KC_COMM, KC_DOT , UNDER_TO_HY, KC_SLSH,
                                   RSG(KC_4), KC_LCTL, KC_LOPT,  KC_LSFT      , KC_ENT     ,     KC_SPC,         KC_RSFT,      KC_BSPC, KC_GRV, RSG(KC_5)
   ),

/*
* Lower Layer: Num & Punct
*
* ,-------------------------------------------.                              ,-------------------------------------------.
* |   `    |  &   |  *   |  ~   |  (   |  )   |                              |   =  |    7 |  8   |   9  |   .  |        |
* |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
* |        |  $   |  %   |  ^   |  {   |  }   |                              |   =  |   4  |  5   |  6   |  /   |  -     |
* |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
* |        |  !   |  @   |  #   |  [   |  ]   |      |      |  |      |      |   0  |  1   |  2   |  3   |  *   |  +     |
* `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
*                        |      |      |      |      |      |  |      |      |      |      |      |
*                        |      |      |      |      |      |  |      |      |      |      |      |
*                        `----------------------------------'  `----------------------------------'
*/
   [_NUM_PUNCT] = LAYOUT(
     KC_GRV,  KC_AMPR, KC_ASTR,  KC_TILD, KC_LPRN,  KC_RPRN,                                        KC_PEQL,  KC_7,   KC_8, KC_9, KC_PDOT, _______,
     _______, KC_DLR,  KC_PERC,  KC_CIRC, KC_LCBR,  KC_RCBR,                                        KC_PEQL,  KC_4,   KC_5, KC_6, KC_SLSH, KC_PMNS,
     _______, KC_EXLM, KC_AT,    KC_HASH, KC_LBRC,  KC_RBRC, _______,  _______, _______,  _______,  KC_0,     KC_1,   KC_2, KC_3, KC_PAST, KC_PPLS,
                                 _______, _______,  _______, _______,  _______,  _______,  _______, _______,  _______, _______
   ),

/*
* Window Movement
*
* ,-------------------------------------------.                              ,-------------------------------------------.
* |        |      |      |   ^  |      |      |                              | Full |  UL  |  TH  |  UR  |      |        |
* |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
* |        |      | <-   |      |  ->  |      |                              | PD   | LHqy   |Center| RH   |  ND  |        |
* |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
* |        |      |      |down  |      |      |      |      |  |      |      | Full | LL   | BH   | LR   |      |        |
* `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
*                        |      |      |      |      |      |  |      |      |      |      |      |
*                        |      |      |      |      |      |  |      |      |      |      |      |
*                        `----------------------------------'  `----------------------------------'
*/
   [_WINDOW] = LAYOUT(
     _______, _______, _______,   KC_UP,  _______, _______,                                     LAG(KC_F),      LCMD(C(KC_LEFT)),    LAG(KC_UP),    LCMD(C(KC_RGHT)),    _______,    _______,
     _______, _______, KC_LEFT, _______,  KC_RGHT, _______,                                     LCAG(KC_LEFT),  LAG(KC_LEFT),        LAG(KC_C),     LAG(KC_RGHT),        LCAG(KC_RGHT), _______,
     _______, _______, _______, KC_DOWN,  _______, _______, _______, _______, _______, _______, LAG(KC_F),      RCS(LCMD(KC_LEFT)),  LAG(KC_DOWN),  RCS(LCMD(KC_RGHT)),  _______, _______,
                                _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
   ),

/*
* Function Layer: Function & RGB
*
* ,-------------------------------------------.                              ,-------------------------------------------.
* | RGB TOG |      |      |      |      |      |                             |  F9  |  F10 |  F11 |  F12 |       |       |
* |---------+------+------+------+------+------|                             |------+------+------+------+---------------|
* |         |      |      |      |      |      |                             |  F5  |  F6  |  F7  |  F8  |       |       |
* |---------+------+------+------+------+------+-------------.  ,------------|------+------+------+------+---------------|
* |         |      |      |      |      |      |      |      |  |      |     |  F1  |  F2  |  F3  |  F4  |       |       |
* `----------------------+------+------+------+------+-------|  |------+-----|------+------+------+----------------------'
*                        |      |      |      |      |       |  |      |     |      |      |      |
*                        |      |      |      |      |       |  |      |     |      |      |      |
*                        `-----------------------------------'  `----------------------------------'
*/
   [_FUNCTION] = LAYOUT(
     RGB_TOG,   RGB_M_P,   RGB_M_B,   RGB_M_R,   RGB_M_SW,   RGB_M_SN,                                        KC_F9,   KC_F10,  KC_F11,  KC_F12, _______, _______,
     _______,   RGB_M_K,   RGB_M_X,   RGB_M_G,   RGB_M_T,    RGB_M_TW,                                        KC_F5,   KC_F6,   KC_F7,   KC_F8,  _______, _______,
     RGB_HUI,   RGB_HUD,   RGB_SAI,   RGB_SAD,   RGB_VAI,     RGB_VAD,  _______, _______,   _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,  _______, _______,
                                      _______,   _______,     _______,  _______, _______,   _______, _______, _______, _______,  _______
   ),


// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

uint8_t mod_state;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
   mod_state = get_mods();
   switch(keycode) {
       case AT_EXCLM: {
           if (record->event.pressed) {
               // This should be !
               if (mod_state & MOD_MASK_SHIFT) {
                   register_code16(S(KC_1));
                   set_mods(mod_state);
                   return false;
               }
               // This should be @ (default)
               else {
                   register_code16(S(KC_2));
                   set_mods(mod_state);
               }
           }
           else {
               if (mod_state & MOD_MASK_SHIFT) {
                   unregister_code16(KC_1);
                   return false;
               }
               else {
                   unregister_code16(KC_2);
               }
           }
       }
       break;

       case COL_TO_SEMI: {
           if (record->event.pressed) {
               // This should be semicolon
               if (mod_state & MOD_MASK_SHIFT) {
                   del_mods(MOD_MASK_SHIFT);
                   register_code16(KC_SCLN);
                   set_mods(mod_state);
                   return false;
               }
               // This should be colon (default)
               else {
                   // This has no weird behavior
                   register_code16(S(KC_SCLN));
                   set_mods(mod_state);
               }
           }
           else {
               unregister_code16(KC_SCLN);
               return false;
           }
       }
       break;

       case UNDER_TO_HY: {
           if (record->event.pressed) {
               // This should be hyphen
               if (mod_state & MOD_MASK_SHIFT) {
                   del_mods(MOD_MASK_SHIFT);
                   register_code16(KC_MINS);
                   set_mods(mod_state);
                   return false;
               }
               // This should be underscore (default)
               else {
                   register_code16(S(KC_MINS));
                   set_mods(mod_state);
               }
           }
           else {
               unregister_code16(KC_MINS);
               return false;
           }
       }
       break;
   }
   return true;
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
   if (!is_keyboard_master()) {
       return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
   }
   return rotation;
}

void render_status(void) {
   oled_clear();
   // Host Keyboard Layer Status
   oled_write_P(PSTR("\nlayer: "), false);
   switch (get_highest_layer(layer_state)) {
       case _DEFAULT:
           oled_write_P(PSTR("default"), false);
           break;
       case _NUM_PUNCT:
           oled_write_P(PSTR("num & punct"), false);
           break;
       case _WINDOW:
           oled_write_P(PSTR("window control"), false);
           break;
       case _FUNCTION:
           oled_write_P(PSTR("function"), false);
           break;
       default:
           oled_write_P(PSTR("undefined"), false);
   }
}

void render_modifiers(void) {
   switch (get_mods()) {
       case MOD_BIT(KC_LOPT):
           oled_write_P(PSTR("\nalt active"), false);
           break;
       case MOD_BIT(KC_LCTL):
           oled_write_P(PSTR("\ncontrol active"), false);
           break;
       case MOD_BIT(KC_LSFT):
           oled_write_P(PSTR("\nL shift active"), false);
           break;
       case MOD_BIT(KC_RSFT):
           oled_write_P(PSTR("\nR shift active"), false);
           break;
       case MOD_BIT(KC_LGUI):
           oled_write_P(PSTR("\ncommand active"), false);
           break;
       default:
           oled_write_P(PSTR("\n"), false);
   }
}


bool oled_task_user(void) {
   if (is_keyboard_master()) {
       oled_write_P(PSTR("\n"), false);
       render_status();
       oled_write_P(PSTR("\n"), false);
       render_modifiers();
   }
   else {
       oled_write_P(PSTR("\n\n\nsophie is awesome"), false);
   }
   return false;
}
#endif

//#ifdef ENCODER_ENABLE
//bool encoder_update_user(uint8_t index, bool clockwise) {
//   if (index == 0) {
//       // tabbing
//       if (clockwise) {
//           tap_code16(C(KC_TAB));
//       } else {
//           tap_code16(S(C(KC_TAB)));
//       }
//   }
//   else if (index == 1) {
//       // app movement
//       if (clockwise) {
//           if (!is_cmd_tab_active) {
//               is_cmd_tab_active = true;
//               register_code(KC_RGUI);
//           }
//           cmd_tab_timer = timer_read();
//           tap_code16(KC_TAB);
//       }
//       else {
//           if (!is_cmd_tab_active) {
//               is_cmd_tab_active = true;
//               register_code(KC_RGUI);
//           }
//           cmd_tab_timer = timer_read();
//           tap_code16(S(KC_TAB));
//       }
//   }
//
////   else if (IS_LAYER_ON(_WINDOW)) {
////       if (index == 0) {
////           // volume
////           if (clockwise) {
////               tap_code16(KC_VOLU);
////           }
////           else {
////               tap_code16(KC_VOLD);
////           }
////       }
////       else {
////           // brightness
////           if (clockwise) {
////               tap_code16(KC_BRIU);
////           }
////           else {
////               tap_code16(KC_BRID);
////           }
////       }
////   }
//   return false;
//}
//
//void matrix_scan_user(void) {
//   if (is_cmd_tab_active) {
//       if (timer_elapsed(cmd_tab_timer) > 1250) {
//           unregister_code(KC_RGUI);
//           is_cmd_tab_active = false;
//       }
//   }
//}
//#endif

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_DEFAULT] =   { ENCODER_CCW_CW(C(KC_TAB), S(C(KC_TAB))), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)  },
    [_NUM_PUNCT] =  { ENCODER_CCW_CW(RGB_HUD, RGB_HUI),           ENCODER_CCW_CW(RGB_SAD, RGB_SAI)  },
    [_FUNCTION] =  { ENCODER_CCW_CW(RGB_VAD, RGB_VAI),           ENCODER_CCW_CW(RGB_SPD, RGB_SPI)  },
    [_WINDOW] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD),          ENCODER_CCW_CW(KC_RIGHT, KC_LEFT) },
};
#endif