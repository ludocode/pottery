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

#ifndef POSITION_H
#define POSITION_H

#include "piece.h"
#include "coords.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct move_t;

/**
 * A game position.
 *
 * This includes roughly the equivalent of a FEN string, plus the extra state
 * needed to capture a king that castles through or out of check.
 */
typedef struct position_t {
    piece_t board[8][8];
    coords_t en_passant; // also used for castling king capture
    bool white_to_move;
    bool castle_white_short;
    bool castle_white_long;
    bool castle_black_short;
    bool castle_black_long;
} position_t;

void position_set_initial(position_t* position);

/**
 * Returns true if the given chess position is legal.
 *
 * A legal position has:
 * - exactly one king per color;
 * - no pawns at the end of their files;
 * - the player that last moved not in check; and
 * - at least one legal move available to the player to move.
 */
bool position_is_legal(const position_t* position);

typedef enum position_state_t {
    position_state_normal,
    position_state_check,
    position_state_checkmate,
    position_state_stalemate,
} position_state_t;

position_state_t position_state(const position_t* position);

/**
 * Returns true if the given move is legal for the given position.
 */
bool position_is_move_legal(const position_t* position, struct move_t* move);

static inline
piece_t position_piece_at(const position_t* position, coords_t coords) {
    return position->board[coords.row][coords.col];
}

/**
 * Applies the given move to the position.
 */
void position_apply_move(position_t* position, struct move_t* move);

/**
 * Returns true if the given move captures the king.
 */
bool position_is_move_king_capture(const position_t* position, struct move_t* move);

/**
 * Returns true if any pseudo-legal move captures the king.
 */
bool position_has_king_capture(const position_t* position);

/**
 * Formats a position into the given string.
 * 
 * The position will contain newlines but not a trailing newline.
 */
void position_format(const position_t* position, string_t* string);

#ifdef __cplusplus
}
#endif

#endif
