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

#ifndef MOVE_H
#define MOVE_H

#include "piece.h"
#include "coords.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A move.
 */
typedef struct move_t {
    coords_t from;
    coords_t to;

    // The promotion piece in the correct case (i.e. for the correct player.)
    // If the move is not a promotion, this is '.'.
    piece_t promotion;
} move_t;

/*
 * A vector of moves.
 */
#include "moves.t.h"
#include "pottery/vector/pottery_vector_declare.t.h"

static inline
bool move_equal(move_t* left, move_t* right) {
    return coords_equal(left->from, right->from) &&
            coords_equal(left->to, right->to) &&
            left->promotion == right->promotion;
}

// Makes a normal (non-promotion) move.
static inline
move_t move_make(coords_t from, coords_t to) {
    move_t move = {from, to, '.'};
    return move;
}

static inline
bool move_is_null(move_t* move) {
    return coords_is_null(move->from);
}

// Makes a clear (empty, invalid) move.
static inline
move_t move_make_null(void) {
    move_t move = {coords_make_null(), coords_make_null(), '.'};
    return move;
}

void move_format(move_t* move, char buf[6]);

/**
 * Parses a move.
 */
bool move_parse(move_t* move, string_t* string, bool white_to_move);

void move_print(move_t* move);

#ifdef __cplusplus
}
#endif

#endif
