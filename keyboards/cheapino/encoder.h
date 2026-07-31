// Copyright 2023 Thomas Haukland (@tompi)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "matrix.h"

/**
 * Turn the encoder's raw matrix state into key taps, then clear the encoder row
 * so the rest of QMK never sees it as key presses.
 */
void fix_encoder_action(matrix_row_t current_matrix[]);
