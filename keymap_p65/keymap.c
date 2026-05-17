/* Copyright 2024 – Custom ISO-DE Keymap for GMMK2 P65 ISO
 *
 * Features:
 *   - German (DE/AT) ISO layout with Umlaute (Ö, Ä, Ü, ß)
 *   - Z/Y swapped for QWERTZ
 *   - Selectable static base colour (10 presets incl. Off)
 *   - Caps Lock pulse indicator (red/green)
 *   - FN-layer key highlighting (only mapped keys glow while FN held)
 *   - VIA-compatible (2 layers: Base + Function)
 */

#include QMK_KEYBOARD_H
#include "keymap_german.h"

enum layers { _BL = 0, _FL = 1 };

enum custom_keycodes {
    RGB_BASE_NEXT = SAFE_RANGE,
    RGB_BASE_PREV,
};

typedef struct { uint8_t r; uint8_t g; uint8_t b; } rgb_color_t;

static const rgb_color_t BASE_COLORS[] = {
    {  0,   0, 255}, {255, 200,   0}, {148,   0, 211}, {255,  20, 147},
    {  0, 200, 200}, {255,  80,   0}, { 50, 255,  50}, {255, 255, 255},
    {255,   0,   0}, {  0,   0,   0},
};
#define BASE_COLOR_COUNT ((uint8_t)(sizeof(BASE_COLORS) / sizeof(BASE_COLORS[0])))

typedef union {
    uint32_t raw;
    struct { uint8_t color_idx : 8; };
} user_config_t;

static user_config_t user_config;
static uint8_t       base_color_idx = 0;

/* ── Caps Lock pulse animation ────────────────────────────────────── */
#define PULSE_DURATION_MS  3000
#define PULSE_COUNT        5

typedef struct {
    uint32_t start_time; bool active; bool is_green; uint8_t led_idx;
} pulse_anim_t;

static pulse_anim_t caps_pulse = {0};

/* FN-layer highlight: suppressed once a cursor key is pressed while FN held */
static bool fn_highlight_active = true;

static uint8_t calc_pulse_brightness(uint32_t elapsed) {
    if (elapsed >= PULSE_DURATION_MS) return 0;
    uint32_t cycle_len = PULSE_DURATION_MS / PULSE_COUNT;
    uint32_t cycle_pos = elapsed % cycle_len;
    uint16_t half = cycle_len / 2;
    return (cycle_pos < half)
        ? (uint8_t)((cycle_pos * 255) / half)
        : (uint8_t)(((cycle_len - cycle_pos) * 255) / half);
}

static void start_pulse(pulse_anim_t *p, uint8_t led, bool green) {
    p->start_time = timer_read32(); p->active = true; p->is_green = green; p->led_idx = led;
}

void eeconfig_init_user(void) {
    user_config.raw = 0; user_config.color_idx = 0;
    eeconfig_update_user(user_config.raw);
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    if (user_config.color_idx >= BASE_COLOR_COUNT) user_config.color_idx = 0;
    base_color_idx = user_config.color_idx;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (layer_state_cmp(state, _FL)) {
        fn_highlight_active = true;
    }
    return state;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_BL] = LAYOUT_65_iso_blocker(
  KC_ESC,   DE_1,     DE_2,     DE_3,     DE_4,     DE_5,     DE_6,     DE_7,     DE_8,     DE_9,     DE_0,     DE_SS,    DE_ACUT,  KC_BSPC,  KC_DEL,
  KC_TAB,   DE_Q,     DE_W,     DE_E,     DE_R,     DE_T,     DE_Z,     DE_U,     DE_I,     DE_O,     DE_P,     DE_UDIA,  DE_PLUS,            KC_PGUP,
  KC_CAPS,  DE_A,     DE_S,     DE_D,     DE_F,     DE_G,     DE_H,     DE_J,     DE_K,     DE_L,     DE_ODIA,  DE_ADIA,  DE_HASH,  KC_ENT,   KC_PGDN,
  KC_LSFT,  DE_LABK,  DE_Y,     DE_X,     DE_C,     DE_V,     DE_B,     DE_N,     DE_M,     DE_COMM,  DE_DOT,   DE_MINS,  KC_RSFT,  KC_UP,    KC_END,
  KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(_FL),  KC_LEFT,  KC_DOWN,  KC_RGHT),

[_FL] = LAYOUT_65_iso_blocker(
  QK_BOOT,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  KC_INS,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_HOME,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_END,
  _______,  _______,  KC_MPRV,  KC_MSTP,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLU,  KC_VOLD,  _______,  _______,  _______,  _______,  RM_VALU,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  RGB_BASE_PREV, RM_VALD, RGB_BASE_NEXT)
};

/* ── Reactive events ──────────────────────────────────────────────── */
#define MAX_EVENTS            16
#define REACTIVE_TIMEOUT_MS  500
#define WAVE_RADIUS           80

typedef struct { uint8_t led_idx; uint32_t timestamp; bool is_special; bool active; } key_event_t;
static key_event_t key_events[MAX_EVENTS];
static uint8_t     event_head = 0;

static void record_event(uint8_t led, bool special) {
    key_events[event_head] = (key_event_t){led, timer_read32(), special, true};
    event_head = (event_head + 1) % MAX_EVENTS;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    uint8_t led = g_led_config.matrix_co[row][col];

    if (record->event.pressed) {
        switch (keycode) {
            case RGB_BASE_NEXT:
                fn_highlight_active = false;
                base_color_idx = (base_color_idx + 1) % BASE_COLOR_COUNT;
                user_config.color_idx = base_color_idx; eeconfig_update_user(user_config.raw);
                return false;
            case RGB_BASE_PREV:
                fn_highlight_active = false;
                base_color_idx = (base_color_idx == 0) ? (BASE_COLOR_COUNT - 1) : (base_color_idx - 1);
                user_config.color_idx = base_color_idx; eeconfig_update_user(user_config.raw);
                return false;
            case RM_VALU:
            case RM_VALD:
                fn_highlight_active = false;
                break;
            case KC_CAPS:
                if (led != NO_LED) start_pulse(&caps_pulse, led, !!host_keyboard_led_state().caps_lock);
                break;
            default:
                if (led != NO_LED) {
                    bool special = (keycode == KC_ESC || keycode == KC_ENT || keycode == KC_PENT);
                    record_event(led, special);
                }
                break;
        }
    }
    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t val = rgb_matrix_get_val();
    rgb_color_t bc = BASE_COLORS[base_color_idx];
    uint8_t base_r = (uint8_t)((uint16_t)bc.r * val / 255);
    uint8_t base_g = (uint8_t)((uint16_t)bc.g * val / 255);
    uint8_t base_b = (uint8_t)((uint16_t)bc.b * val / 255);
    bool backlight_on = (base_r > 0 || base_g > 0 || base_b > 0);

    /* ── FN-layer highlighting ────────────────────────────────────── */
    if (layer_state_is(_FL) && fn_highlight_active) {
        for (uint8_t i = led_min; i < led_max; i++) rgb_matrix_set_color(i, 0, 0, 0);
        for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
            for (uint8_t c = 0; c < MATRIX_COLS; c++) {
                uint8_t led_i = g_led_config.matrix_co[r][c];
                if (led_i == NO_LED || led_i < led_min || led_i >= led_max) continue;
                uint16_t fn_kc = keymap_key_to_keycode(_FL, (keypos_t){.row = r, .col = c});
                if (fn_kc != KC_TRNS && fn_kc != KC_NO) {
                    if (backlight_on) rgb_matrix_set_color(led_i, base_r, base_g, base_b);
                    else              rgb_matrix_set_color(led_i, 0, 255, 0);
                }
            }
        }
        return false;
    }

    /* ── 1. Paint all LEDs ────────────────────────────────────────── */
    for (uint8_t i = led_min; i < led_max; i++)
        rgb_matrix_set_color(i, base_r, base_g, base_b);

    /* ── Caps Lock persistent state ───────────────────────────────── */
    if (!caps_pulse.active && host_keyboard_led_state().caps_lock) {
        uint8_t cl = caps_pulse.led_idx;
        if (cl != NO_LED && cl >= led_min && cl < led_max) rgb_matrix_set_color(cl, 0, 0, 0);
    }

    /* ── Caps Lock pulse animation ────────────────────────────────── */
    if (caps_pulse.active) {
        uint32_t elapsed = timer_elapsed32(caps_pulse.start_time);
        if (elapsed >= PULSE_DURATION_MS) { caps_pulse.active = false; }
        else {
            uint8_t br = calc_pulse_brightness(elapsed);
            uint8_t cl = caps_pulse.led_idx;
            if (cl >= led_min && cl < led_max)
                rgb_matrix_set_color(cl, caps_pulse.is_green ? 0 : br, caps_pulse.is_green ? br : 0, 0);
        }
    }

    /* ── 2. Overlay reactive effects ──────────────────────────────── */
    for (uint8_t e = 0; e < MAX_EVENTS; e++) {
        if (!key_events[e].active) continue;
        uint32_t elapsed = timer_elapsed32(key_events[e].timestamp);
        if (elapsed > REACTIVE_TIMEOUT_MS) { key_events[e].active = false; continue; }
        uint8_t brightness = (uint8_t)(255 - (elapsed * 255) / REACTIVE_TIMEOUT_MS);
        uint8_t src_led = key_events[e].led_idx;
        if (key_events[e].is_special) {
            uint8_t src_x = g_led_config.point[src_led].x, src_y = g_led_config.point[src_led].y;
            uint8_t wave_front = (uint8_t)((elapsed * WAVE_RADIUS) / REACTIVE_TIMEOUT_MS);
            uint8_t wave_width = 14;
            for (uint8_t i = led_min; i < led_max; i++) {
                uint8_t dx = (uint8_t)abs((int16_t)g_led_config.point[i].x - src_x);
                uint8_t dy = (uint8_t)abs((int16_t)g_led_config.point[i].y - src_y);
                uint16_t dist = dx + dy;
                if (dist <= (uint16_t)(wave_front + wave_width) && dist + wave_width >= wave_front) {
                    int16_t diff = (int16_t)wave_front - (int16_t)dist;
                    if (diff < 0) diff = -diff;
                    uint8_t lb = (diff < wave_width) ? (uint8_t)((uint16_t)brightness * (wave_width - diff) / wave_width) : 0;
                    if (lb > 0) rgb_matrix_set_color(i, lb, 0, 0);
                }
            }
        } else {
            if (src_led >= led_min && src_led < led_max)
                rgb_matrix_set_color(src_led, 0, brightness, 0);
        }
    }
    return false;
}
