/* Custom config for GMMK2 P65 ISO – German (AT/DE) Layout */

#pragma once

/* ─── Key Chatter / Debouncing ─────────────────────────────────────────
 * 25ms sicher gegen prellende Schalter (QMK-Standard wäre 5ms).
 */
#define DEBOUNCE 25

/* ─── EEPROM / VIA Dynamic Keymap ──────────────────────────────────────
 * Standard wear-leveling (2048 Bytes) ist zu klein für VIA-Dynamic-Keymaps.
 * Erhöht auf 8 KB; Layer auf 2 begrenzt.
 */
#undef  WEAR_LEVELING_BACKING_SIZE
#define WEAR_LEVELING_BACKING_SIZE 8192
#define DYNAMIC_KEYMAP_LAYER_COUNT 2

/* ─── RGB Matrix ────────────────────────────────────────────────────────
 * Startmodus: Solid Colour (wird durch eigene Rendering-Funktion ersetzt)
 */
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_HUE  160
#define RGB_MATRIX_DEFAULT_SAT  255
#define RGB_MATRIX_DEFAULT_VAL  100

#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_MAX_KEY_EVENTS 16
#define RGB_MATRIX_REACTIVE_TIMEOUT 600
