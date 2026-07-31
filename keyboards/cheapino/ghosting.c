// Copyright 2024 Thomas Haukland (@tompi)
// SPDX-License-Identifier: GPL-2.0-or-later

/*
 * The duplex matrix leaves some sneak paths that the diodes do not fully block,
 * so a few key combinations report a third key that was never pressed. For
 * example, on a QWERTY layout holding a+s+e also reports q.
 *
 * The ghosts follow a fixed pattern: within one hand, a pair of keys held in one
 * row ("the cause") makes a specific pair in another row report an extra key.
 * The tables at the bottom list every observed pattern, and each is checked
 * against all row pairs on both hands.
 */

#include "ghosting.h"
#include "quantum.h"

/*
 * The patterns below were read straight off the QMK console, which prints the
 * most significant bit first. This flips them round so they can be written down
 * the same way they were observed.
 */
#define rev(b)                          \
    ((((b) & (1 << 0)) << 15) |         \
        (((b) & (1 << 1)) << 13) |      \
        (((b) & (1 << 2)) << 11) |      \
        (((b) & (1 << 3)) << 9) |       \
        (((b) & (1 << 4)) << 7) |       \
        (((b) & (1 << 5)) << 5) |       \
        (((b) & (1 << 6)) << 3) |       \
        (((b) & (1 << 7)) << 1) |       \
        (((b) & (1 << 8)) >> 1) |       \
        (((b) & (1 << 9)) >> 3) |       \
        (((b) & (1 << 10)) >> 5) |      \
        (((b) & (1 << 11)) >> 7) |      \
        (((b) & (1 << 12)) >> 9) |      \
        (((b) & (1 << 13)) >> 11) |     \
        (((b) & (1 << 14)) >> 13) |     \
        ((b) >> 15))

// Left hand rows start 4 rows and 6 columns after the right hand ones.
#define LEFT_ROW_OFFSET 4
#define LEFT_COL_SHIFT 6

#define ROWS_PER_HAND 3

static bool bit_pattern_set(uint16_t number, uint16_t bit_pattern) {
    return !(~number & bit_pattern);
}

static void fix_ghosting_instance(matrix_row_t current_matrix[], uint8_t row_num_with_possible_error_cause, uint16_t possible_error_cause, uint8_t row_num_with_possible_error, uint16_t possible_error, uint16_t error_fix) {
    if (bit_pattern_set(current_matrix[row_num_with_possible_error_cause], possible_error_cause)) {
        if (bit_pattern_set(current_matrix[row_num_with_possible_error], possible_error)) {
            current_matrix[row_num_with_possible_error] ^= error_fix;
        }
    }
}

static void fix_ghosting_column(matrix_row_t matrix[], uint16_t possible_error_cause, uint16_t possible_error, uint16_t error_fix) {
    // First the right side...
    for (uint8_t i = 0; i < ROWS_PER_HAND; i++) {
        fix_ghosting_instance(matrix, i, possible_error_cause, (i + 1) % ROWS_PER_HAND, possible_error, error_fix);
        fix_ghosting_instance(matrix, i, possible_error_cause, (i + 2) % ROWS_PER_HAND, possible_error, error_fix);
    }

    // ...then exactly the same procedure on the left side
    for (uint8_t i = 0; i < ROWS_PER_HAND; i++) {
        uint8_t cause_row = i + LEFT_ROW_OFFSET;
        fix_ghosting_instance(matrix, cause_row, possible_error_cause << LEFT_COL_SHIFT, LEFT_ROW_OFFSET + ((i + 1) % ROWS_PER_HAND), possible_error << LEFT_COL_SHIFT, error_fix << LEFT_COL_SHIFT);
        fix_ghosting_instance(matrix, cause_row, possible_error_cause << LEFT_COL_SHIFT, LEFT_ROW_OFFSET + ((i + 2) % ROWS_PER_HAND), possible_error << LEFT_COL_SHIFT, error_fix << LEFT_COL_SHIFT);
    }
}

void fix_ghosting(matrix_row_t matrix[]) {
    fix_ghosting_column(matrix, rev(0B0110000000000000), rev(0B1010000000000000), rev(0B0010000000000000));
    fix_ghosting_column(matrix, rev(0B0110000000000000), rev(0B0101000000000000), rev(0B0100000000000000));

    fix_ghosting_column(matrix, rev(0B0001100000000000), rev(0B0010100000000000), rev(0B0000100000000000));
    fix_ghosting_column(matrix, rev(0B0001100000000000), rev(0B0001010000000000), rev(0B0001000000000000));

    fix_ghosting_column(matrix, rev(0B1000010000000000), rev(0B1000100000000000), rev(0B1000000000000000));
    fix_ghosting_column(matrix, rev(0B1000010000000000), rev(0B0100010000000000), rev(0B0000010000000000));

    fix_ghosting_column(matrix, rev(0B1001000000000000), rev(0B0101000000000000), rev(0B0001000000000000));
    fix_ghosting_column(matrix, rev(0B1001000000000000), rev(0B1010000000000000), rev(0B1000000000000000));

    fix_ghosting_column(matrix, rev(0B0100100000000000), rev(0B0100010000000000), rev(0B0100000000000000));
    fix_ghosting_column(matrix, rev(0B0100100000000000), rev(0B1000100000000000), rev(0B0000100000000000));
}
