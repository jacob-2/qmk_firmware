// Copyright 2023 Thomas Haukland (@tompi)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

// Countdown for the startup animation below; 0 means it has finished.
static uint8_t hue_countdown = 50;

// The user's own LED colour, so it can be restored once the animation is done.
static uint8_t stored_hue;
static uint8_t stored_saturation;
static uint8_t stored_value;

// Sweep the onboard LED through the hue wheel for about 2.5 seconds on plug-in.
// This runs on the deferred executor so the animation does not block typing.
static uint32_t flash_led(uint32_t trigger_time, void *cb_arg) {
    rgblight_sethsv_noeeprom(hue_countdown * 5, 230, 70);
    hue_countdown--;

    if (hue_countdown == 0) {
        // Finished, reset to the user's chosen LED colour
        rgblight_sethsv_noeeprom(stored_hue, stored_saturation, stored_value);
        return 0;
    }

    return 50;
}

void keyboard_post_init_kb(void) {
    stored_hue        = rgblight_get_hue();
    stored_saturation = rgblight_get_sat();
    stored_value      = rgblight_get_val();

    defer_exec(50, flash_led, NULL);

    keyboard_post_init_user();
}
