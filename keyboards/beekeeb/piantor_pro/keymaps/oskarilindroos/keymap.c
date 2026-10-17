// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum custom_keycodes {
    M_ALT_TAB = SAFE_RANGE,
    M_CMD_TAB,
    M_CTRL_TAB
};

const uint16_t PROGMEM TAB_combo[] = {KC_W, KC_E, COMBO_END};
const uint16_t PROGMEM ESC_combo[] = {KC_I, KC_O, COMBO_END};
// const uint16_t PROGMEM CW_combo[] = {KC_F, KC_J, COMBO_END};
combo_t key_combos[] = {
    COMBO(TAB_combo, KC_TAB), // I + O = ESC
    COMBO(ESC_combo, KC_ESC), // W + E = TAB
    // COMBO(CW_combo, CW_TOGG), // Left shift + Right shift = CAPS WORD TOGGLE
};

// Layer names
enum layer_names {
    BASE,
    SYM,
    NAV,
    FUN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // BASE layer
    [BASE] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        XXXXXXX,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  XXXXXXX,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        XXXXXXX, LGUI_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F), RALT_T(KC_G),   KC_H, RSFT_T(KC_J), RCTL_T(KC_K), LALT_T(KC_L), RGUI_T(KC_SEMICOLON), XXXXXXX,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        XXXXXXX,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMMA,  KC_DOT, KC_QUOTE, XXXXXXX,
    //|--------+--------+--------+--------+--------+--------+-----------------|  |--------+-----------------+--------+--------+--------+--------+--------|
                                            XXXXXXX,   MO(SYM),  MEH_T(KC_SPACE),   KC_ENTER,  LT(NAV, KC_BACKSPACE), XXXXXXX
                                        //`-----------------------------------'  `----------------------------------'
    ),

    // SYMbol layer
    [SYM] = LAYOUT_split_3x6_3(
        //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        XXXXXXX, KC_LBRC, KC_RBRC, KC_LPRN, KC_RPRN, XXXXXXX,                         KC_GRV,  KC_7,    KC_8,    KC_9,    KC_0,    XXXXXXX,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_RALT,                         KC_MINS, KC_4,    KC_5,    KC_6,    KC_EQL,  XXXXXXX,
        //|--------+--------+--------+-----------+--------------+--------|                    |--------+--------+--------+--------+--------+--------|
        XXXXXXX, CW_TOGG, XXXXXXX, LSFT(KC_LBRC), LSFT(KC_RBRC), XXXXXXX,                         KC_BSLS, KC_1,    KC_2,    KC_3,    KC_SLSH, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            XXXXXXX, _______, _______,           _______, KC_BSPC, XXXXXXX
                                        //`--------------------------'  `--------------------------'
    ),

    // NAVigation layer
    [NAV] = LAYOUT_split_3x6_3(
        //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        XXXXXXX, KC_VOLU, M_CTRL_TAB, M_ALT_TAB, M_CMD_TAB, KC_BRIU,                  KC_COPY, KC_PASTE,  KC_HOME,  KC_PGUP, KC_INS, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_BRID,                         KC_LEFT, KC_DOWN, KC_UP,  KC_RGHT, XXXXXXX,  XXXXXXX,
        //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        XXXXXXX, KC_VOLD, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX,                         KC_UNDO,  KC_CUT, KC_END, KC_PGDN,  KC_DEL, XXXXXXX,
        //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            XXXXXXX, QK_LAYER_LOCK, _______,           _______, _______, XXXXXXX
                                        //`--------------------------'  `--------------------------'
    ),

    // FUNction layer
    [FUN] = LAYOUT_split_3x6_3(
        //,---------------------------------------------------------------.                    ,---------------------------------------------------------------.
        XXXXXXX,      QK_BOOT,      XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,                          XXXXXXX,      KC_F7,        KC_F8,        KC_F9,        KC_F10,       XXXXXXX,
        //|-------------+-------------+-------------+-------------+-------------+-------------|                    |-------------+-------------+-------------+-------------+-------------+-------------|
        XXXXXXX,      KC_LGUI,      KC_LALT,      KC_LCTL,      KC_LSFT,      KC_RALT,                          KC_F12,       KC_F4,        KC_F5,        KC_F6,        XXXXXXX,      XXXXXXX,
        //|-------------+-------------+-------------+-------------+-------------+-------------|                    |-------------+-------------+-------------+-------------+-------------+-------------|
        XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,      XXXXXXX,                          KC_F11,       KC_F1,        KC_F2,        KC_F3,        XXXXXXX,      XXXXXXX,
        //|-------------+-------------+-------------+-------------+-------------+-------------+-------------|  |-------------+-------------+-------------+-------------+-------------+-------------+-------------|
                                                    XXXXXXX,      _______,      _______,                     _______,      _______,      XXXXXXX
                                                //`---------------------------------------'  `---------------------------------------'
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case M_ALT_TAB:
            if (record->event.pressed) {
                // On key press: Hold Alt and press Tab
                register_code(KC_LALT);  // Hold Alt
                tap_code(KC_TAB);        // Press and release Tab
            } else {
                // On key release: Release Alt
                unregister_code(KC_LALT);
            }
            return false;

        case M_CMD_TAB:
                    if (record->event.pressed) {
                        register_code(KC_LGUI);  // Hold Command
                        tap_code(KC_TAB);        // Press and release Tab
                    } else {
                        unregister_code(KC_LGUI);  // Release Command
                    }
                    return false;

        case M_CTRL_TAB:
                    if (record->event.pressed) {
                        register_code(KC_LCTL);  // Hold Command
                        tap_code(KC_TAB);        // Press and release Tab
                    } else {
                        unregister_code(KC_LCTL);  // Release Command
                    }
                    return false;
            }
    return true;  // Process other keys normally
}
