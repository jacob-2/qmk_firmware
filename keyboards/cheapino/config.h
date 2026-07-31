// Copyright 2023 Thomas Haukland (@tompi)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// The RP2040 WS2812 driver defaults to PIO0, which is already claimed on this
// board. Move the LED output to PIO1.
#define WS2812_PIO_USE_PIO1

// The single onboard LED expects plain RGB rather than the WS2812 default GRB.
#define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_RGB
