// Copyright 2012 Jun Wako <wakojun@gmail.com>
// Copyright 2023 Thomas Haukland (@tompi)
// SPDX-License-Identifier: GPL-2.0-or-later

/*
 * The Cheapino wires each of its six column pins to two logical columns: one is
 * read with the pin as an input while a row drives the matrix, the other is read
 * with the pin driving and the rows as inputs. That halves the pin count, but it
 * means the stock matrix scan cannot be used, hence this custom one.
 *
 * Even logical columns are the "column drives, rows read" pass, odd logical
 * columns are the "row drives, columns read" pass.
 */

#include <stdbool.h>
#include <stdint.h>

#include "matrix.h"
#include "encoder.h"
#include "ghosting.h"
#include "quantum.h"
#include "wait.h"

// How long the scanning code waits for changed IO to settle. Raised from the
// default 30us to weigh up for the extra time spent ghost hunting below; the
// RP2040 has cycles to spare for it.
#define MATRIX_IO_DELAY 25

#define COL_SHIFTER ((uint16_t)1)

static const pin_t row_pins[] = MATRIX_ROW_PINS;
static const pin_t col_pins[] = MATRIX_COL_PINS;

static matrix_row_t previous_matrix[MATRIX_ROWS];

static void select_row(uint8_t row) {
    gpio_set_pin_output(row_pins[row]);
    gpio_write_pin_low(row_pins[row]);
}

static void unselect_row(uint8_t row) {
    gpio_set_pin_input_high(row_pins[row]);
}

static void unselect_rows(void) {
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        gpio_set_pin_input_high(row_pins[x]);
    }
}

static void select_col(uint8_t col) {
    gpio_set_pin_output(col_pins[col]);
    gpio_write_pin_low(col_pins[col]);
}

static void unselect_col(uint8_t col) {
    gpio_set_pin_input_high(col_pins[col]);
}

static void unselect_cols(void) {
    for (uint8_t x = 0; x < MATRIX_COLS / 2; x++) {
        gpio_set_pin_input_high(col_pins[x * 2]);
    }
}

static void read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Select row and wait for row selection to stabilize
    select_row(current_row);
    wait_us(MATRIX_IO_DELAY);

    // For each col...
    for (uint8_t col_index = 0; col_index < MATRIX_COLS / 2; col_index++) {
        uint16_t column_index_bitmask = COL_SHIFTER << ((col_index * 2) + 1);
        // Check col pin state
        if (gpio_read_pin(col_pins[col_index * 2])) {
            // Pin HI, clear col bit
            current_matrix[current_row] &= ~column_index_bitmask;
        } else {
            // Pin LO, set col bit
            current_matrix[current_row] |= column_index_bitmask;
        }
    }

    unselect_row(current_row);
}

static void read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col) {
    // Select col and wait for col selection to stabilize
    select_col(current_col * 2);
    wait_us(MATRIX_IO_DELAY);

    uint16_t column_index_bitmask = COL_SHIFTER << (current_col * 2);
    // For each row...
    for (uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++) {
        // Check row pin state
        if (gpio_read_pin(row_pins[row_index])) {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~column_index_bitmask;
        } else {
            // Pin LO, set col bit
            current_matrix[row_index] |= column_index_bitmask;
        }
    }

    unselect_col(current_col * 2);
}

static void store_old_matrix(matrix_row_t current_matrix[]) {
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        previous_matrix[i] = current_matrix[i];
    }
}

static bool has_matrix_changed(matrix_row_t current_matrix[]) {
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        if (previous_matrix[i] != current_matrix[i]) {
            return true;
        }
    }
    return false;
}

void matrix_init_custom(void) {
    unselect_cols();
    unselect_rows();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    store_old_matrix(current_matrix);

    // Set row, read cols
    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        read_cols_on_row(current_matrix, current_row);
    }

    // Set col, read rows
    for (uint8_t current_col = 0; current_col < MATRIX_COLS / 2; current_col++) {
        read_rows_on_col(current_matrix, current_col);
    }

    fix_encoder_action(current_matrix);
    fix_ghosting(current_matrix);

    return has_matrix_changed(current_matrix);
}
