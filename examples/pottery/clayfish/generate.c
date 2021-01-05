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

#include "generate.h"

#include "position.h"
#include "move.h"

// Generates a move with the given step if possible, adding it to the given
// moves vector. Returns true if the square was empty (allowing move generation
// to continue in a line.)
clayfish_always_inline static
bool generate_move(const position_t* position,
        coords_t coords, coords_t step,
        moves_t* moves)
{
    move_t move = move_make_null();
    move.from = coords;
    move.to = coords_add(coords, step);

    // The move must be on the board
    if (!coords_is_valid(move.to))
        return false;

    piece_t src = position_piece_at(position, coords);
    piece_t dest = position_piece_at(position, move.to);

    bool ret;

    // The move must move into an empty square...
    if (dest == '.') {
        // continue in a line
        ret = true;
    // or capture an enemy piece
    } else if (dest != '.' && piece_is_white(src) != piece_is_white(dest)) {
        // don't continue in a line
        ret = false;
    } else {
        return false;
    }

    moves_insert_last(moves, move);
    return ret;
}

// Generates the given list if steps as either moves or captures. If line is
// true, each item in the list is extended in a straight line.
clayfish_always_inline static
void generate_steps(const position_t* position,
        coords_t coords,
        const coords_t* steps, size_t steps_count,
        bool line,
        moves_t* moves)
{
    for (size_t i = 0; i < steps_count; ++i) {
        coords_t step = steps[i];
        while (generate_move(position, coords, step, moves) && line) {
            step = coords_add(step, steps[i]);
        }
    }
}

static void generate_knight_moves(const position_t* position, coords_t coords, moves_t* moves) {
    static const coords_t knight_steps[] = {{-1, -2}, {-2, -1}, {-1, 2}, {-2, 1}, {1, -2}, {2, -1}, {1, 2}, {2, 1}};
    generate_steps(position, coords, knight_steps, clayfish_array_count(knight_steps), false, moves);
}

static void generate_bishop_moves(const position_t* position, coords_t coords, moves_t* moves) {
    static const coords_t bishop_steps[] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    generate_steps(position, coords, bishop_steps, clayfish_array_count(bishop_steps), true, moves);
}

static void generate_rook_moves(const position_t* position, coords_t coords, moves_t* moves) {
    static const coords_t rook_steps[] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};
    generate_steps(position, coords, rook_steps, clayfish_array_count(rook_steps), true, moves);
}

static void generate_queen_moves(const position_t* position, coords_t coords, moves_t* moves) {
    generate_rook_moves(position, coords, moves);
    generate_bishop_moves(position, coords, moves);
}

static void generate_king_moves(const position_t* position, coords_t coords, moves_t* moves) {
    static const coords_t king_steps[] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    generate_steps(position, coords, king_steps, clayfish_array_count(king_steps), false, moves);

    // Generate castling

    // Note that we don't check if the king or rooks are in their appropriate
    // positions. If they've moved, castling rights should have been revoked.
    // We also don't check whether the king is castling out of or through
    // check. Upon castling, the square passed through becomes the en-passant
    // square so the king can be captured in these squares on the next ply.

    if (piece_is_white(position_piece_at(position, coords))) {
        if (position->castle_white_short &&
                position_piece_at(position, coords_make(0, 5)) == '.' &&
                position_piece_at(position, coords_make(0, 6)) == '.')
            moves_insert_last(moves, move_make(coords, coords_make(0, 6)));

        if (position->castle_white_long &&
                position_piece_at(position, coords_make(0, 3)) == '.' &&
                position_piece_at(position, coords_make(0, 2)) == '.' &&
                position_piece_at(position, coords_make(0, 1)) == '.')
            moves_insert_last(moves, move_make(coords, coords_make(0, 2)));

    } else {
        if (position->castle_black_short &&
                position_piece_at(position, coords_make(7, 5)) == '.' &&
                position_piece_at(position, coords_make(7, 6)) == '.')
            moves_insert_last(moves, move_make(coords, coords_make(7, 6)));

        if (position->castle_black_long &&
                position_piece_at(position, coords_make(7, 3)) == '.' &&
                position_piece_at(position, coords_make(7, 2)) == '.' &&
                position_piece_at(position, coords_make(7, 1)) == '.')
            moves_insert_last(moves, move_make(coords, coords_make(7, 2)));
    }
}

static void generate_pawn_promotions(const position_t* position,
        move_t move, moves_t* moves)
{
    bool white = position->white_to_move;
    if (move.to.row == (white ? 7 : 0)) {
        move.promotion = white ? 'Q' : 'q';
        moves_insert_last(moves, move);
        move.promotion = white ? 'R' : 'r';
        moves_insert_last(moves, move);
        move.promotion = white ? 'B' : 'b';
        moves_insert_last(moves, move);
        move.promotion = white ? 'N' : 'n';
        moves_insert_last(moves, move);
    } else {
        moves_insert_last(moves, move);
    }
}

static void generate_pawn_moves(const position_t* position,
        coords_t coords, moves_t* moves)
{
    piece_t src = position_piece_at(position, coords);
    bool white = piece_is_white(src);
    signed char dir = white ? 1 : -1;
    move_t move = move_make_null();
    move.from = coords;

    // en-passant capture
    // (this might capture a pawn that moved two squares or a king that has
    // castled by the square it moved through)
    if (coords_is_valid(position->en_passant) &&
            position->en_passant.row == coords.row + dir &&
            abs(position->en_passant.col - coords.col) == 1)
        moves_insert_last(moves, move_make(coords, position->en_passant));

    // capture a king that castled out of check
    if ((position->en_passant.row == 0 || position->en_passant.row == 7) &&
            position->en_passant.row == coords.row + dir &&
            abs(4 - coords.col) == 1) {
        move.to = coords_make(position->en_passant.row, 4);
        moves_insert_last(moves, move);
    }

    // ordinary captures (with possible promotion)
    int sides[] = {-1, 1};
    for (size_t i = 0; i < 2; ++i) {
        move.to = coords_add(coords, coords_make(dir, sides[i]));
        if (coords_is_valid(move.to)) {
            piece_t dest = position_piece_at(position, move.to);

            #if 0
            if (dest != '.' && clayfish_debug) {
                move_print(&move);
                printf("    <-- pawn capture\n");
            }
            #endif

            if (dest != '.' && white != piece_is_white(dest)) {
                generate_pawn_promotions(position, move, moves);
            }
        }
    }

    // single step (with possible promotion)
    move.to = coords;
    move.to.row += dir;
    // Note that we don't check if the position is in bounds here; if pawns are
    // on their last rank, the position would not be legal.
    if (position_piece_at(position, move.to) == '.') {
        generate_pawn_promotions(position, move, moves);

        // double step
        if (coords.row == (white ? 1 : 6)) {
            move.to.row += dir;
            if (coords_is_valid(move.to) && position_piece_at(position, move.to) == '.') {
                moves_insert_last(moves, move);
            }
        }
    }
}

void generate_moves_at(const position_t* position, coords_t coords, moves_t* moves) {
    switch (position_piece_at(position, coords)) {
        case 'P': case 'p': generate_pawn_moves(position, coords, moves); break;
        case 'N': case 'n': generate_knight_moves(position, coords, moves); break;
        case 'K': case 'k': generate_king_moves(position, coords, moves); break;
        case 'R': case 'r': generate_rook_moves(position, coords, moves); break;
        case 'B': case 'b': generate_bishop_moves(position, coords, moves); break;
        case 'Q': case 'q': generate_queen_moves(position, coords, moves); break;
        default:
            break;
    }
}

void generate_moves(const position_t* position, moves_t* moves) {
    for (signed char row = 0; row < 8; ++row) {
        for (signed char col = 0; col < 8; ++col) {
            piece_t piece = position->board[row][col];
            if (piece_is_white(piece) == position->white_to_move) {
                coords_t coords = {row, col};
                generate_moves_at(position, coords, moves);
            }
        }
    }
}
