// Copyright 2024 Thomas Haukland (@tompi)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "matrix.h"

/**
 * Clear the known ghost keys out of a freshly scanned matrix.
 */
void fix_ghosting(matrix_row_t current_matrix[]);
