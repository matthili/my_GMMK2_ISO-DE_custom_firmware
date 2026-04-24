/* Copyright 2024 – Custom ISO-DE Keymap for GMMK2 P65 ISO
 *
 * Features:
 *   - German (DE/AT) ISO layout with Umlaute (Ö, Ä, Ü, ß)
 *   - Z/Y swapped for QWERTZ
 *   - FN + 1…= → F1-F12
 *   - FN + Up/Down → Helligkeit +/–
 *   - FN + Left/Right → Basisfarbe wechseln (10 Presets + Aus)
 *   - FN + PgUp → Home  |  FN + PgDn → End
 *   - Selectable static base colour, persistent im EEPROM
 *   - ESC / ENTER trigger a reactive RED wave effect
 *   - Every other key flashes GREEN briefly on press
 *   - VIA-compatible (2 layers: Base + Function)
 */

#include QMK_KEYBOARD_H
#include "keymap_german.h"

/* ═══════════════════════════════════════════════════════════════════════
 *  Layer definitions
 * ═══════════════════════════════════════════════════════════════════════ */
enum layers {
    _BL = 0,
    _FL = 1
};

/* ═══════════════════════════════════════════════════════════════════════
 *  Custom keycodes
 * ═══════════════════════════════════════════════════════════════════════ */
enum custom_keycodes {
    RGB_BASE_NEXT = SAFE_RANGE,   /* FN + Right  →  next base colour  */
    RGB_BASE_PREV,                /* FN + Left   →  prev base colour  */
};

/* ═══════════════════════════════════════════════════════════════════════
 *  Base-colour presets (full brightness; scaled by rgb_matrix_get_val())
 * ═══════════════════════════════════════════════════════════════════════ */
typedef struct { uint8_t r; uint8_t g; uint8_t b; } rgb_color_t;

static const rgb_color_t BASE_COLORS[] = {
    {  0,   0, 255},   /* 0 – Blau      (Standard)  */
    {255, 200,   0},   /* 1 – Gelb                  */
    {148,   0, 211},   /* 2 – Violett               */
    {255,  20, 147},   /* 3 – Rosa                  */
    {  0, 200, 200},   /* 4 – Türkis                */
    {255,  80,   0},   /* 5 – Orange                */
    { 50, 255,  50},   /* 6 – Hellgrün              */
    {255, 255, 255},   /* 7 – Weiß                  */
    {255,   0,   0},   /* 8 – Rot                   */
    {  0,   0,   0},   /* 9 – Aus (nur Reaktiv-Effekte sichtbar) */
};
#define BASE_COLOR_COUNT ((uint8_t)(sizeof(BASE_COLORS) / sizeof(BASE_COLORS[0])))

/* ─── EEPROM persistence ──────────────────────────────────────────────
 * QMK provides one 32-bit "user" EEPROM slot.
 * We store the colour index in the lowest byte.
 * ────────────────────────────────────────────────────────────────────── */
typedef union {
    uint32_t raw;
    struct {
        uint8_t color_idx : 8;
    };
} user_config_t;

static user_config_t user_config;
static uint8_t       base_color_idx = 0;

void eeconfig_init_user(void) {
    user_config.raw       = 0;
    user_config.color_idx = 0;
    eeconfig_update_user(user_config.raw);
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    if (user_config.color_idx >= BASE_COLOR_COUNT) {
        user_config.color_idx = 0;
    }
    base_color_idx = user_config.color_idx;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  Keymap  –  LAYOUT_65_iso_blocker  (68 Tasten)
 *
 *  FN-Layer Übersicht:
 *    ESC          → QK_BOOT  (Bootloader)
 *    1…=          → F1–F12
 *    Del          → Ins
 *    PgUp         → Home
 *    PgDn         → End
 *    Z…M, ,, .    → Medientasten
 *    FN + Up      → RM_VALU  (Helligkeit +)
 *    FN + Down    → RM_VALD  (Helligkeit –)
 *    FN + Right   → RGB_BASE_NEXT
 *    FN + Left    → RGB_BASE_PREV
 * ═══════════════════════════════════════════════════════════════════════ */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* ──── Base Layer (_BL) ─── German QWERTZ ISO ──────────────────────── */
[_BL] = LAYOUT_65_iso_blocker(
  KC_ESC,   DE_1,     DE_2,     DE_3,     DE_4,     DE_5,     DE_6,     DE_7,     DE_8,     DE_9,     DE_0,     DE_SS,    DE_ACUT,  KC_BSPC,  KC_DEL,
  KC_TAB,   DE_Q,     DE_W,     DE_E,     DE_R,     DE_T,     DE_Z,     DE_U,     DE_I,     DE_O,     DE_P,     DE_UDIA,  DE_PLUS,            KC_PGUP,
  KC_CAPS,  DE_A,     DE_S,     DE_D,     DE_F,     DE_G,     DE_H,     DE_J,     DE_K,     DE_L,     DE_ODIA,  DE_ADIA,  DE_HASH,  KC_ENT,   KC_PGDN,
  KC_LSFT,  DE_LABK,  DE_Y,     DE_X,     DE_C,     DE_V,     DE_B,     DE_N,     DE_M,     DE_COMM,  DE_DOT,   DE_MINS,  KC_RSFT,  KC_UP,    KC_END,
  KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(_FL),  KC_LEFT,  KC_DOWN,  KC_RGHT),

/* ──── Function Layer (_FL) ──────────────────────────────────────────── */
[_FL] = LAYOUT_65_iso_blocker(
  QK_BOOT,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  KC_INS,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_HOME,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_END,
  _______,  _______,  KC_MPRV,  KC_MSTP,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLU,  KC_VOLD,  _______,  _______,  _______,  _______,  RM_VALU,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  RGB_BASE_PREV, RM_VALD, RGB_BASE_NEXT)
};

/* ═══════════════════════════════════════════════════════════════════════
 *  process_record_user
 * ═══════════════════════════════════════════════════════════════════════ */

/* ─── Reactive event ring-buffer ──────────────────────────────────── */
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
    if (record->event.pressed) {
        switch (keycode) {
            case RGB_BASE_NEXT:
                base_color_idx = (base_color_idx + 1) % BASE_COLOR_COUNT;
                user_config.color_idx = base_color_idx;
                eeconfig_update_user(user_config.raw);
                return false;

            case RGB_BASE_PREV:
                base_color_idx = (base_color_idx == 0)
                                 ? (BASE_COLOR_COUNT - 1)
                                 : (base_color_idx - 1);
                user_config.color_idx = base_color_idx;
                eeconfig_update_user(user_config.raw);
                return false;

            default: {
                uint8_t row = record->event.key.row;
                uint8_t col = record->event.key.col;
                uint8_t led = g_led_config.matrix_co[row][col];
                if (led != NO_LED) {
                    bool special = (keycode == KC_ESC  ||
                                    keycode == KC_ENT  ||
                                    keycode == KC_PENT);
                    record_event(led, special);
                }
                break;
            }
        }
    }
    return true;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  rgb_matrix_indicators_advanced_user – per-frame render loop
 * ═══════════════════════════════════════════════════════════════════════ */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    /* ── Brightness-scaled base colour ────────────────────────────── */
    uint8_t val   = rgb_matrix_get_val();
    rgb_color_t bc = BASE_COLORS[base_color_idx];
    uint8_t base_r = (uint8_t)((uint16_t)bc.r * val / 255);
    uint8_t base_g = (uint8_t)((uint16_t)bc.g * val / 255);
    uint8_t base_b = (uint8_t)((uint16_t)bc.b * val / 255);

    /* ── 1. Paint every LED with the base colour ──────────────────── */
    for (uint8_t i = led_min; i < led_max; i++) {
        rgb_matrix_set_color(i, base_r, base_g, base_b);
    }

    /* ── 2. Overlay reactive effects ──────────────────────────────── */
    for (uint8_t e = 0; e < MAX_EVENTS; e++) {
        if (!key_events[e].active) continue;

        uint32_t elapsed = timer_elapsed32(key_events[e].timestamp);
        if (elapsed > REACTIVE_TIMEOUT_MS) {
            key_events[e].active = false;
            continue;
        }

        uint8_t brightness = (uint8_t)(255 - (elapsed * 255) / REACTIVE_TIMEOUT_MS);
        uint8_t src_led    = key_events[e].led_idx;

        if (key_events[e].is_special) {
            /* ── Red wave radiating from ESC / ENTER ─────────────── */
            uint8_t src_x      = g_led_config.point[src_led].x;
            uint8_t src_y      = g_led_config.point[src_led].y;
            uint8_t wave_front = (uint8_t)((elapsed * WAVE_RADIUS) / REACTIVE_TIMEOUT_MS);
            uint8_t wave_width = 14;

            for (uint8_t i = led_min; i < led_max; i++) {
                uint8_t  dx   = (uint8_t)abs((int16_t)g_led_config.point[i].x - src_x);
                uint8_t  dy   = (uint8_t)abs((int16_t)g_led_config.point[i].y - src_y);
                uint16_t dist = dx + dy;

                if (dist <= (uint16_t)(wave_front + wave_width) &&
                    dist + wave_width >= wave_front) {
                    int16_t diff = (int16_t)wave_front - (int16_t)dist;
                    if (diff < 0) diff = -diff;
                    uint8_t lb = (diff < wave_width)
                        ? (uint8_t)((uint16_t)brightness * (wave_width - diff) / wave_width)
                        : 0;
                    if (lb > 0) rgb_matrix_set_color(i, lb, 0, 0);
                }
            }
        } else {
            /* ── Green flash on the single pressed key ───────────── */
            if (src_led >= led_min && src_led < led_max) {
                rgb_matrix_set_color(src_led, 0, brightness, 0);
            }
        }
    }

    return false;
}
