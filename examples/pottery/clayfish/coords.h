#ifndef COORDS_H
#define COORDS_H

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

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
