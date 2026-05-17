/* Copyright 2024 – Custom ISO-DE Keymap for GMMK2 P96 ISO
 *
 * Features:
 *   - German (DE/AT) ISO layout with Umlaute (Ö, Ä, Ü, ß)
 *   - Z/Y swapped for QWERTZ
 *   - Selectable static base colour (10 presets incl. Off), cycled with
 *     FN + Left / FN + Right
 *   - Global brightness controlled by FN + Up / FN + Down
 *   - ESC / ENTER trigger a reactive RED wave effect
 *   - Every other key flashes GREEN briefly on press
 *   - Lock-key pulse indicators: Caps Lock, Num Lock, Insert
 *   - FN-layer key highlighting (only mapped keys glow while FN held)
 *   - VIA-compatible (2 layers: Base + Function)
 */

#include QMK_KEYBOARD_H
#include "keymap_german.h"

/* ═══════════════════════════════════════════════════════════════════════
 *  Layer definitions
 * ═══════════════════════════════════════════════════════════════════════ */
enum layers { _BL = 0, _FL = 1 };

/* ═══════════════════════════════════════════════════════════════════════
 *  Custom keycodes
 * ═══════════════════════════════════════════════════════════════════════ */
enum custom_keycodes {
    RGB_BASE_NEXT = SAFE_RANGE,
    RGB_BASE_PREV,
    CUS_INS,
};

/* ═══════════════════════════════════════════════════════════════════════
 *  Base-colour presets
 * ═══════════════════════════════════════════════════════════════════════ */
typedef struct { uint8_t r; uint8_t g; uint8_t b; } rgb_color_t;

static const rgb_color_t BASE_COLORS[] = {
    {  0,   0, 255},   /* 0 – Blau      */
    {255, 200,   0},   /* 1 – Gelb      */
    {148,   0, 211},   /* 2 – Violett   */
    {255,  20, 147},   /* 3 – Rosa      */
    {  0, 200, 200},   /* 4 – Türkis    */
    {255,  80,   0},   /* 5 – Orange    */
    { 50, 255,  50},   /* 6 – Hellgrün  */
    {255, 255, 255},   /* 7 – Weiß      */
    {255,   0,   0},   /* 8 – Rot       */
    {  0,   0,   0},   /* 9 – Aus       */
};
#define BASE_COLOR_COUNT ((uint8_t)(sizeof(BASE_COLORS) / sizeof(BASE_COLORS[0])))

/* ─── EEPROM persistence ─────────────────────────────────────────── */
typedef union {
    uint32_t raw;
    struct { uint8_t color_idx : 8; };
} user_config_t;

static user_config_t user_config;
static uint8_t       base_color_idx = 0;

/* ═══════════════════════════════════════════════════════════════════════
 *  Lock-key pulse animation
 * ═══════════════════════════════════════════════════════════════════════ */
#define PULSE_DURATION_MS  3000
#define PULSE_COUNT        5
#define INS_LONG_PRESS_MS  2000

typedef struct {
    uint32_t start_time;
    bool     active;
    bool     is_green;
    uint8_t  led_idx;
} pulse_anim_t;

static pulse_anim_t caps_pulse = {0};
static pulse_anim_t num_pulse  = {0};
static pulse_anim_t ins_pulse  = {0};

static bool     insert_mode_on = true;
static uint32_t ins_press_time = 0;
static bool     ins_key_held   = false;
static uint8_t  ins_row = 0, ins_col = 0;

static uint8_t calc_pulse_brightness(uint32_t elapsed) {
    if (elapsed >= PULSE_DURATION_MS) return 0;
    uint32_t cycle_len = PULSE_DURATION_MS / PULSE_COUNT;
    uint32_t cycle_pos = elapsed % cycle_len;
    uint16_t half = cycle_len / 2;
    if (cycle_pos < half)
        return (uint8_t)((cycle_pos * 255) / half);
    else
        return (uint8_t)(((cycle_len - cycle_pos) * 255) / half);
}

static void start_pulse(pulse_anim_t *p, uint8_t led, bool green) {
    p->start_time = timer_read32();
    p->active     = true;
    p->is_green   = green;
    p->led_idx    = led;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  EEPROM init / post-init
 * ═══════════════════════════════════════════════════════════════════════ */
void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.color_idx = 0;
    eeconfig_update_user(user_config.raw);
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    if (user_config.color_idx >= BASE_COLOR_COUNT) user_config.color_idx = 0;
    base_color_idx = user_config.color_idx;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  Keymap
 * ═══════════════════════════════════════════════════════════════════════ */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_BL] = LAYOUT(
  KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_DEL,   CUS_INS,  KC_PGUP,  KC_PGDN,
  DE_CIRC,  DE_1,     DE_2,     DE_3,     DE_4,     DE_5,     DE_6,     DE_7,     DE_8,     DE_9,     DE_0,     DE_SS,    DE_ACUT,  KC_BSPC,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
  KC_TAB,   DE_Q,     DE_W,     DE_E,     DE_R,     DE_T,     DE_Z,     DE_U,     DE_I,     DE_O,     DE_P,     DE_UDIA,  DE_PLUS,  KC_ENT,   KC_P7,    KC_P8,    KC_P9,    KC_PPLS,
  KC_CAPS,  DE_A,     DE_S,     DE_D,     DE_F,     DE_G,     DE_H,     DE_J,     DE_K,     DE_L,     DE_ODIA,  DE_ADIA,  DE_HASH,            KC_P4,    KC_P5,    KC_P6,
  KC_LSFT,  DE_LABK,  DE_Y,     DE_X,     DE_C,     DE_V,     DE_B,     DE_N,     DE_M,     DE_COMM,  DE_DOT,   DE_MINS,  KC_RSFT,  KC_UP,    KC_P1,    KC_P2,    KC_P3,    KC_PENT,
  KC_LCTL,  KC_LGUI,  KC_LALT,                      KC_SPC,                                 KC_RALT,  MO(_FL),  KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,    KC_PDOT),

[_FL] = LAYOUT(
  QK_BOOT,  KC_MYCM,  KC_WHOM,  KC_CALC,  KC_MSEL,  KC_MPRV,  KC_MRWD,  KC_MPLY,  KC_MSTP,  KC_MUTE,  KC_VOLU,  KC_VOLD,  _______,  _______,  _______,  _______,  KC_HOME,  KC_END,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  RM_VALU,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,                      _______,                                _______,  _______,  _______,  RGB_BASE_PREV, RM_VALD, RGB_BASE_NEXT, _______,  _______)
};

/* ═══════════════════════════════════════════════════════════════════════
 *  process_record_user
 * ═══════════════════════════════════════════════════════════════════════ */
#define MAX_EVENTS            16
#define REACTIVE_TIMEOUT_MS  500
#define WAVE_RADIUS           80

typedef struct {
    uint8_t  led_idx;
    uint32_t timestamp;
    bool     is_special;
    bool     active;
} key_event_t;

static key_event_t key_events[MAX_EVENTS];
static uint8_t     event_head = 0;

static void record_event(uint8_t led, bool special) {
    key_events[event_head].led_idx    = led;
    key_events[event_head].timestamp  = timer_read32();
    key_events[event_head].is_special = special;
    key_events[event_head].active     = true;
    event_head = (event_head + 1) % MAX_EVENTS;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    uint8_t led = g_led_config.matrix_co[row][col];

    if (record->event.pressed) {
        switch (keycode) {
            case RGB_BASE_NEXT:
                base_color_idx = (base_color_idx + 1) % BASE_COLOR_COUNT;
                user_config.color_idx = base_color_idx;
                eeconfig_update_user(user_config.raw);
                return false;
            case RGB_BASE_PREV:
                base_color_idx = (base_color_idx == 0) ? (BASE_COLOR_COUNT - 1) : (base_color_idx - 1);
                user_config.color_idx = base_color_idx;
                eeconfig_update_user(user_config.raw);
                return false;
            case KC_CAPS:
                if (led != NO_LED) {
                    bool will_be_on = !host_keyboard_led_state().caps_lock;
                    start_pulse(&caps_pulse, led, !will_be_on);
                }
                break;
            case KC_NUM:
                if (led != NO_LED) {
                    bool will_be_on = !host_keyboard_led_state().num_lock;
                    start_pulse(&num_pulse, led, will_be_on);
                }
                break;
            case CUS_INS:
                ins_press_time = timer_read32();
                ins_key_held   = true;
                ins_row = row; ins_col = col;
                if (led != NO_LED) record_event(led, false);
                return false;
            default:
                if (led != NO_LED) {
                    bool special = (keycode == KC_ESC || keycode == KC_ENT || keycode == KC_PENT);
                    record_event(led, special);
                }
                break;
        }
    } else {
        if (keycode == CUS_INS && ins_key_held) {
            uint8_t ins_led = g_led_config.matrix_co[ins_row][ins_col];
            uint32_t hold_time = timer_elapsed32(ins_press_time);
            if (hold_time >= INS_LONG_PRESS_MS) {
                insert_mode_on = true;
                if (ins_led != NO_LED) start_pulse(&ins_pulse, ins_led, true);
            } else {
                insert_mode_on = !insert_mode_on;
                tap_code(KC_INS);
                if (ins_led != NO_LED) start_pulse(&ins_pulse, ins_led, insert_mode_on);
            }
            ins_key_held = false;
            return false;
        }
    }
    return true;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  rgb_matrix_indicators_advanced_user
 * ═══════════════════════════════════════════════════════════════════════ */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t val = rgb_matrix_get_val();
    rgb_color_t bc = BASE_COLORS[base_color_idx];
    uint8_t base_r = (uint8_t)((uint16_t)bc.r * val / 255);
    uint8_t base_g = (uint8_t)((uint16_t)bc.g * val / 255);
    uint8_t base_b = (uint8_t)((uint16_t)bc.b * val / 255);
    bool backlight_on = (base_r > 0 || base_g > 0 || base_b > 0);

    /* ── FN-layer highlighting ────────────────────────────────────── */
    if (layer_state_is(_FL)) {
        for (uint8_t i = led_min; i < led_max; i++)
            rgb_matrix_set_color(i, 0, 0, 0);
        for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
            for (uint8_t c = 0; c < MATRIX_COLS; c++) {
                uint8_t led_i = g_led_config.matrix_co[r][c];
                if (led_i == NO_LED || led_i < led_min || led_i >= led_max) continue;
                uint16_t fn_kc = keymap_key_to_keycode(_FL, (keypos_t){.row = r, .col = c});
                if (fn_kc != KC_TRNS && fn_kc != KC_NO) {
                    if (backlight_on)
                        rgb_matrix_set_color(led_i, base_r, base_g, base_b);
                    else
                        rgb_matrix_set_color(led_i, 0, 255, 0);
                }
            }
        }
        return false;
    }

    /* ── 1. Paint every LED with the base colour ──────────────────── */
    for (uint8_t i = led_min; i < led_max; i++)
        rgb_matrix_set_color(i, base_r, base_g, base_b);

    /* ── 1b. Lock-key persistent state ────────────────────────────── */
    if (!caps_pulse.active && host_keyboard_led_state().caps_lock) {
        uint8_t cl = caps_pulse.led_idx;
        if (cl != NO_LED && cl >= led_min && cl < led_max)
            rgb_matrix_set_color(cl, 0, 0, 0);
    }
    {
        uint8_t nl = g_led_config.matrix_co[4][6];
        if (!num_pulse.active && nl != NO_LED && nl >= led_min && nl < led_max) {
            if (!host_keyboard_led_state().num_lock)
                rgb_matrix_set_color(nl, 0, 0, 0);
        }
    }
    if (!ins_pulse.active && !insert_mode_on) {
        uint8_t il = ins_pulse.led_idx;
        if (il != NO_LED && il >= led_min && il < led_max)
            rgb_matrix_set_color(il, 0, 0, 0);
    }

    /* ── 1c. Lock-key pulse animations ────────────────────────────── */
    pulse_anim_t *pulses[] = { &caps_pulse, &num_pulse, &ins_pulse };
    for (uint8_t p = 0; p < 3; p++) {
        if (!pulses[p]->active) continue;
        uint32_t elapsed = timer_elapsed32(pulses[p]->start_time);
        if (elapsed >= PULSE_DURATION_MS) { pulses[p]->active = false; continue; }
        uint8_t br = calc_pulse_brightness(elapsed);
        uint8_t pl = pulses[p]->led_idx;
        if (pl >= led_min && pl < led_max) {
            if (pulses[p]->is_green)
                rgb_matrix_set_color(pl, 0, br, 0);
            else
                rgb_matrix_set_color(pl, br, 0, 0);
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
            uint8_t src_x = g_led_config.point[src_led].x;
            uint8_t src_y = g_led_config.point[src_led].y;
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
