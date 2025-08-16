/* Copyright 2022 Gondolindrim
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

// RGB underglow configuration  
void keyboard_post_init_user(void) {
    // Enable RGB and set to soft apricot color
    rgblight_enable();
    rgblight_mode(1);  // Static light mode (mode 1)
    rgblight_sethsv(15, 200, 255);  // Soft apricot: H=15 (more orange), S=200 (more saturated), V=255
}

// Update RGB based on layer and caps lock state
void update_rgb_state(void) {
    uint8_t layer = get_highest_layer(layer_state);
    bool caps_on = host_keyboard_led_state().caps_lock;
    
    if (caps_on) {
        // Caps Lock is on - bright red (overrides layer colors)
        rgblight_sethsv_noeeprom(0, 255, 255);  // H=0 (red), S=255, V=255
    } else {
        // Set color based on active layer
        switch (layer) {
            case 0:
                // Layer 0 (base) - soft apricot
                rgblight_sethsv_noeeprom(15, 200, 255);  // H=15 (apricot), S=200, V=255
                break;
            case 1:
                // Layer 1 (Fn) - hot pink
                rgblight_sethsv_noeeprom(213, 255, 255);  // H=213 (hot pink), S=255, V=255
                break;
            case 2:
                // Layer 2 - electric blue
                rgblight_sethsv_noeeprom(170, 255, 255);  // H=170 (electric blue), S=255, V=255
                break;
            default:
                // Fallback to apricot
                rgblight_sethsv_noeeprom(15, 200, 255);
                break;
        }
    }
}

// Change RGB color based on Caps Lock state
bool led_update_user(led_t led_state) {
    update_rgb_state();
    return true;
}

// Change RGB color based on layer state
layer_state_t layer_state_set_user(layer_state_t state) {
    update_rgb_state();
    return state;
}

// Tap Dance declarations
enum {
    TD_CTRL_CAPS,
};

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_CTRL_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LCTL, KC_CAPS),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT_65_ansi_blocker( /* Base */
    QK_GESC, KC_1   , KC_2   , KC_3   , KC_4  , KC_5    , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS, KC_EQL , KC_BSPC, KC_DEL ,
    KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R  , KC_T    , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME,
    TD(TD_CTRL_CAPS), KC_A   , KC_S   , KC_D   , KC_F  , KC_G    , KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT, KC_ENT , KC_PGUP,
    KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V  , KC_B    , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT, KC_UP  , KC_PGDN,
    MO(2), KC_LGUI, KC_LALT,                            KC_SPC ,                   KC_RALT, MO(1)  , KC_LEFT, KC_DOWN, KC_RGHT
),
[1] = LAYOUT_65_ansi_blocker(
    QK_BOOT, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , KC_TRNS, KC_MUTE,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PSCR, KC_SCRL, KC_TRNS, KC_TRNS, KC_END,
    KC_CAPS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_TRNS, KC_VOLU,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_VOLD,
    KC_TRNS, AG_LNRM, AG_LSWP,                            KC_MPLY,                   RGB_TOG, KC_TRNS, KC_HOME, KC_PGDN, KC_END 
),
[2] = LAYOUT_65_ansi_blocker(
    QK_BOOT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_HOME, KC_UP  , KC_END , KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_LEFT, KC_DOWN, KC_RIGHT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, LCAG(KC_Z), KC_TRNS, LCAG(KC_V), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,                   KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_END 
)
};
