#include QMK_KEYBOARD_H
#include "print.h"

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        uprintf("Key pressed: 0x%04X\n", keycode);
    } else {
        uprintf("Key released: 0x%04X\n", keycode);
    }
    return true;
}

// enum custom_keycodes {
//     LED_LEVEL = SAFE_RANGE,
// };
//
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Layer 0 - General
    LAYOUT(
        KC_KB_MUTE, A(C(KC_UP)),  A(C(KC_Z)), A(C(KC_Z)),
        A(C(KC_E)),    A(C(KC_U)), A(C(KC_I)), A(C(KC_G)),
        A(C(KC_LEFT)),    A(C(KC_J)), A(C(KC_K)), A(C(KC_RGHT)),
        RGB_TOG,  A(C(KC_DOWN)),  A(C(KC_G)),   TO(1)
    ),
    // Layer 1 - Rive
    LAYOUT(
        KC_KB_MUTE, KC_9,    KC_U,    KC_SPC,
        KC_GRAVE,    LALT(KC_COMM), LALT(KC_DOT), KC_LEFT_ALT,
        KC_TAB,    LGUI(KC_COMM), LGUI(KC_DOT), KC_LEFT_GUI,
        TO(2),   KC_COMM,  KC_DOT, TO(2)
    ),
    // Layer 2 - After Effects
    LAYOUT(
        KC_KB_MUTE, KC_9,    KC_U,    KC_SPC,
        KC_GRAVE,    LALT(KC_LEFT), LALT(KC_RGHT), KC_LEFT_ALT,
        S(KC_F3),    KC_J, KC_K, KC_LEFT_GUI,
        TO(3),   LGUI(KC_LEFT), LGUI(KC_RGHT), TO(3)
    ),
    // Layer 3 - Premiere Pro
    LAYOUT(
        KC_KB_MUTE, KC_9,    KC_U,    KC_SPC,
        KC_GRAVE,    C(KC_LEFT), C(KC_RIGHT), KC_LEFT_ALT,
        S(KC_F3),    KC_UP, KC_DOWN, KC_LEFT_GUI,
        TO(0),   KC_LEFT, KC_RGHT, TO(0)
    ),
};

typedef union {
    uint32_t raw;
    struct {
        uint8_t led_level : 3;
    };
} work_louder_config_t;

work_louder_config_t work_louder_config;

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),           ENCODER_CCW_CW(A(G(C(KC_LEFT))), A(G(C(KC_LEFT)))) },
    { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),           ENCODER_CCW_CW(KC_COMM, KC_DOT)   },
    { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),           ENCODER_CCW_CW(LGUI(KC_LEFT), LGUI(KC_RGHT))  },
    { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),           ENCODER_CCW_CW(KC_LEFT, KC_RGHT )},
};
#endif

void eeconfig_init_user(void) {
    work_louder_config.raw = 0;
    work_louder_config.led_level = 1;
    eeconfig_update_user(work_louder_config.raw);
}

void matrix_init_user(void) {
    work_louder_config.raw = eeconfig_read_user();
    work_louder_micro_led_all_set((uint8_t)(work_louder_config.led_level * 255 / 4));
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (layer_state_cmp(state, 0)) {
        rgblight_sethsv_noeeprom(0, 255, 128);
    } else if (layer_state_cmp(state, 1)) {
        rgblight_sethsv_noeeprom(85, 255, 128);
    } else if (layer_state_cmp(state, 2)) {
        rgblight_sethsv_noeeprom(171, 255, 128);
    } else if (layer_state_cmp(state, 3)) {
        rgblight_sethsv_noeeprom(255, 255, 128);
    }

    layer_state_cmp(state, 1) ? work_louder_micro_led_1_on(): work_louder_micro_led_1_off();
    layer_state_cmp(state, 2) ? work_louder_micro_led_2_on(): work_louder_micro_led_2_off();
    layer_state_cmp(state, 3) ? work_louder_micro_led_3_on(): work_louder_micro_led_3_off();

    return state;
}
