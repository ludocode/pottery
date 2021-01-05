/*
 * MIT License
 *
 * Copyright (c) 2020-2021 11037650 Canada Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef COORDS_H
#define COORDS_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A pair of coordinates on the board (absolute coordinates), or an offset to a
 * pair of coordinates (relative coordinates).
 */
typedef struct coords_t {
    signed char row;
    signed char col;
} coords_t;

/**
 * Makes a pair of coordinates.
 *
 * Note that row is the y coordinate and col is the x coordinate, so y comes
 * first. This order matches the indices of the board array in position_t.
 */
static inline
coords_t coords_make(int row, int col) {
    coords_t coords = {(signed char)row, (signed char)col};
    return coords;
}

static inline
coords_t coords_make_null(void) {
    coords_t coords = {CHAR_MIN, CHAR_MIN};
    return coords;
}

/**
 * Adds two coordinates together. (Usually one is an offset to the other.)
 */
static inline
coords_t coords_add(coords_t left, coords_t right) {
    coords_t coords = {left.row + right.row, left.col + right.col};
    return coords;
}

/**
 * Returns true if the given pair of absolute coordinates is on the board.
 */
static inline
bool coords_is_valid(coords_t coords) {
    return coords.row >= 0 && coords.row < 8 &&
            coords.col >= 0 && coords.col < 8;
}

static inline
bool coords_is_null(coords_t coords) {
    return coords.row == CHAR_MIN;
}

static inline
bool coords_equal(coords_t left, coords_t right) {
    return left.row == right.row && left.col == right.col;
}

static inline
int coords_manhattan_distance(coords_t left, coords_t right) {
    return abs(left.row - right.row) +
           abs(left.col - right.col);
}

static inline
int coords_manhattan_center_distance(coords_t coords) {
    return abs(((coords.row + 1) >> 1) - 2) +
           abs(((coords.col + 1) >> 1) - 2);
}

#ifdef __cplusplus
}
#endif

#endif
