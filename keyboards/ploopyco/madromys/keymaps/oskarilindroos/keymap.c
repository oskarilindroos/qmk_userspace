#include QMK_KEYBOARD_H

// How long the hand key must be held before switching hands
#ifndef HAND_SWITCH_HOLD_MS
#    define HAND_SWITCH_HOLD_MS 1000
#endif

// Drag scroll tap = right click, if released within TAPPING_TERM
// and the ball moved less than this (sensor counts)
#ifndef SCROLL_TAP_MAX_MOVE
#    define SCROLL_TAP_MAX_MOVE 5
#endif

// ──────────────────────────────────────────────────────────────
// Custom keycodes
// ──────────────────────────────────────────────────────────────
enum custom_keycodes {
    M_SWITCH = SAFE_RANGE,
    HAND_RIGHT,
    HAND_LEFT,
};

// Tap = Mission Control, hold = App Exposé. LT(0, ...) is only used
// for QMK's tap/hold detection, the F24 itself is never sent.
#define M_EXPOSE LT(0, KC_F24)

// ──────────────────────────────────────────────────────────────
// Layers
// ──────────────────────────────────────────────────────────────
// LEFT comes first so it's the default after Bootmagic wipes EEPROM
enum layer_names {
    LEFT,
    RIGHT,
};

// ──────────────────────────────────────────────────────────────
// Keymaps
// ──────────────────────────────────────────────────────────────
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Left‑handed layer
    [LEFT] = LAYOUT(DRAG_SCROLL, M_SWITCH, HAND_RIGHT, M_EXPOSE, MS_BTN3, MS_BTN1),

    // Right‑handed layer
    [RIGHT] = LAYOUT(M_EXPOSE, HAND_LEFT, M_SWITCH, DRAG_SCROLL, MS_BTN1, MS_BTN3)};

// ──────────────────────────────────────────────────────────────
// Window switching: Cmd-Tab on macOS, Alt-Tab elsewhere. The
// modifier stays held while the key is, keeping the switcher open
// ──────────────────────────────────────────────────────────────
static uint8_t switch_mod = KC_LGUI;

static bool host_is_mac(void) {
    os_variant_t os = detected_host_os();
    return os == OS_MACOS || os == OS_IOS;
}

// ──────────────────────────────────────────────────────────────
// Drag scroll tap: the Ploopy code scrolls, this only tracks
// movement so a tap without rolling becomes a right click
// ──────────────────────────────────────────────────────────────
static bool     scroll_held  = false;
static int32_t  scroll_moved = 0;
static uint16_t scroll_timer = 0;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (scroll_held) {
        scroll_moved += abs(mouse_report.x) + abs(mouse_report.y);
        // Ploopy maps x straight to horizontal scroll, which runs opposite
        // to its vertical scroll. Flip it so both axes follow the ball.
        mouse_report.x = -mouse_report.x;
    }
    return mouse_report;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case M_EXPOSE:
            if (record->event.pressed) {
                tap_code16(record->tap.count ? LCTL(KC_UP) : LCTL(KC_DOWN));
            }
            return false;

        case M_SWITCH:
            if (record->event.pressed) {
                switch_mod = host_is_mac() ? KC_LGUI : KC_LALT;
                register_code(switch_mod);
                tap_code(KC_TAB);
            } else {
                unregister_code(switch_mod);
            }
            return false;

        case HAND_RIGHT:
        case HAND_LEFT: {
            static uint16_t hand_timer;
            if (record->event.pressed) {
                hand_timer = timer_read();
            } else if (timer_elapsed(hand_timer) >= HAND_SWITCH_HOLD_MS) {
                // Long press only, saved to EEPROM so the hand survives unplugging
                set_single_persistent_default_layer(keycode == HAND_LEFT ? LEFT : RIGHT);
            } else if (timer_elapsed(hand_timer) < TAPPING_TERM) {
                // Quick tap = Back
                tap_code16(host_is_mac() ? LGUI(KC_LBRC) : LALT(KC_LEFT));
            }
            return false;
        }

        case DRAG_SCROLL:
            if (record->event.pressed) {
                scroll_held  = true;
                scroll_moved = 0;
                scroll_timer = timer_read();
            } else {
                scroll_held = false;
                if (timer_elapsed(scroll_timer) < TAPPING_TERM && scroll_moved < SCROLL_TAP_MAX_MOVE) {
                    tap_code16(MS_BTN2);
                }
            }
            // Let the Ploopy code handle the drag scroll itself
            return true;

        default:
            return true;
    }

    return true;
}
