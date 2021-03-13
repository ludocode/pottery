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

#include "position.h"

#include "move.h"
#include "common.h"
#include "generate.h"

void position_set_initial(position_t* position) {

    // clear the board
    memset(position->board, '.', sizeof(position->board));

    // add starting pieces
    position->board[0][0] = (position->board[0][7] = 'R');
    position->board[0][1] = (position->board[0][6] = 'N');
    position->board[0][2] = (position->board[0][5] = 'B');
    position->board[0][3] = 'Q';
    position->board[0][4] = 'K';
    position->board[7][0] = (position->board[7][7] = 'r');
    position->board[7][1] = (position->board[7][6] = 'n');
    position->board[7][2] = (position->board[7][5] = 'b');
    position->board[7][3] = 'q';
    position->board[7][4] = 'k';
    for (size_t col = 0; col < 8; ++col) {
        position->board[1][col] = 'P';
        position->board[6][col] = 'p';
    }

    // set up additional position
    position->en_passant = coords_make_null();
    position->white_to_move = true;
    position->castle_white_short = true;
    position->castle_white_long = true;
    position->castle_black_short = true;
    position->castle_black_long = true;
}

bool position_is_legal(const position_t* position) {
    coords_t white_king = coords_make_null();
    coords_t black_king = coords_make_null();

    for (signed char row = 0; row < 8; ++row) {
        for (signed char col = 0; col < 8; ++col) {
            coords_t coords = coords_make(row, col);
            char piece = position_piece_at(position, coords);
            if (piece == '.')
                continue;

            switch (piece) {
                case 'K':
                    if (!coords_is_null(white_king)) {
                        // two white kings
                        return false;
                    }
                    white_king = coords;
                    break;

                case 'k':
                    if (!coords_is_null(black_king)) {
                        // two black kings
                        return false;
                    }
                    black_king = coords;
                    break;

                case 'P':
                    if (row == 7) {
                        // unpromoted white pawn on the 8th rank
                        return false;
                    }
                    break;

                case 'p':
                    if (row == 0) {
                        // unpromoted black pawn on the 1st rank
                        return false;
                    }
                    break;
            }
        }
    }

    if (position_has_king_capture(position)) {
        // the player that just moved is in check
        return false;
    }

    position_state_t state = position_state(position);
    if (state == position_state_checkmate || state == position_state_stalemate) {
        // the player to move has no legal moves
        return false;
    }

    return true;
}

bool position_has_king_capture(const position_t* position) {
    moves_t moves;
    moves_init(&moves);
    generate_moves(position, &moves);

    bool has_king_capture = false;
    for (size_t i = 0; i < moves_count(&moves); ++i) {
        if (position_is_move_king_capture(position, moves_at(&moves, i))) {
            has_king_capture = true;
            break;
        }
    }

    moves_destroy(&moves);
    return has_king_capture;
}

position_state_t position_state(const position_t* position) {

    // First we see if the position is in check by looking for opposing king
    // captures.
    position_t flipped = *position;
    flipped.white_to_move = !flipped.white_to_move;
    bool in_check = position_has_king_capture(&flipped);

    // Next we look at each pseudo-legal move to see if it leaves us in check.
    // We need at least one legal move for it to not be game over.

    moves_t moves;
    moves_init(&moves);
    generate_moves(position, &moves);

    bool has_legal_move = false;
    for (size_t i = 0; i < moves_count(&moves); ++i) {
        position_t applied = *position;
        position_apply_move(&applied, moves_at(&moves, i));
        if (!position_has_king_capture(&applied)) {
            has_legal_move = true;
            break;
        }
    }

    moves_destroy(&moves);

    if (has_legal_move)
        return in_check ? position_state_check : position_state_normal;
    else
        return in_check ? position_state_checkmate : position_state_stalemate;
}

bool position_is_move_legal(const position_t* position, struct move_t* move) {
    moves_t moves;
    moves_init(&moves);
    generate_moves(position, &moves);

    bool found = false;
    for (size_t i = 0; i < moves_count(&moves); ++i) {
        if (move_equal(move, moves_at(&moves, i))) {
            found = true;
            break;
        }
    }

    moves_destroy(&moves);

    // The move must exist in the pseudo-legal move list
    if (!found)
        return false;

    // The move must also not put us in check.
    position_t applied = *position;
    position_apply_move(&applied, move);
    return !position_has_king_capture(&applied);
}

bool position_is_move_king_capture(const position_t* position, move_t* move) {

    // check if it's a direct king capture
    if (piece_make_black(position_piece_at(position, move->to)) == 'k')
        return true;

    // check if it's a capture of a king that castled through or out of check
    if (!coords_is_null(position->en_passant) &&
            move->to.row == position->en_passant.row &&
            (move->to.row == 0 || move->to.row == 7) &&
            (move->to.col == position->en_passant.col || move->to.col == 4))
        return true;

    return false;
}

void position_apply_move(position_t* position, move_t* move) {

    // promotion
    piece_t piece = position_piece_at(position, move->from);
    piece_t new_piece = (move->promotion != '.') ? move->promotion : piece;

    if (!coords_is_null(position->en_passant)) {

        // en-passant capture
        coords_t en_passant = position->en_passant;
        position->en_passant = coords_make_null();
        if (coords_equal(move->to, en_passant)) {
            if (move->to.row == 0 || move->to.row == 7) {
                // to capture a king en-passant a pawn must move diagonally. other
                // pieces need only occupy the square.
                if ((piece != 'P' && piece != 'p') || move->to.col != move->from.col) {
                    // remove captured king
                    position->board[move->to.row][(move->to.col == 3) ? 2 : 6] = '.';
                }
            } else {
                // only pawns may capture pawns en-passant
                if (piece == 'P' || piece == 'p') {
                    // remove captured pawn
                    position->board[en_passant.row + (position->white_to_move ? -1 : 1)][en_passant.col] = '.';
                }
            }
        }

        // capture of king that castled out of check
        // (Note that we don't check whether this is a pawn that stepped one step
        // forward without capturing because the previous position wouldn't have
        // been legal anyway.)
        if ((en_passant.row == 0 || en_passant.row == 7) &&
                coords_equal(move->to, coords_make(en_passant.row, 4))) {
            piece_t king = (en_passant.row == 0) ? 'K' : 'k';
            coords_t short_pos = coords_make(en_passant.row, 6);
            coords_t long_pos = coords_make(en_passant.row, 2);
            if (position_piece_at(position, short_pos) == king)
                position->board[short_pos.row][short_pos.col] = '.';
            else
                position->board[long_pos.row][long_pos.col] = '.';
        }
    }

    // double pawn move
    if ((piece == 'P' || piece == 'p') &&
            move->to.col == move->from.col &&
            abs(move->to.row - move->from.row) == 2)
    {
        position->en_passant = coords_make(move->to.row + (position->white_to_move ? -1 : 1), move->to.col);
    }

    // castling
    // Note that we only move the rook if it exists, since castling with a
    // non-existent rook is allowed when giving rook odds.
    if ((piece == 'K' || piece == 'k') && move->from.col == 4) {
        char rook = position->white_to_move ? 'R' : 'r';

        if (move->to.col == 6) {
            // short castle
            if (position->board[move->to.row][7] == rook) {
                position->board[move->to.row][7] = '.';
                position->board[move->to.row][5] = rook;
            }
            position->en_passant = coords_make(move->to.row, 5);
        } else if (move->to.col == 2) {
            // long castle
            if (position->board[move->to.row][0] == rook) {
                position->board[move->to.row][0] = '.';
                position->board[move->to.row][3] = rook;
            }
            position->en_passant = coords_make(move->to.row, 3);
        }

        // if none of the above, then it's not castling
    }

    // lose castling rights
    switch (piece) {
        case 'K':
            position->castle_white_short = false;
            position->castle_white_long = false;
            break;
        case 'k':
            position->castle_black_short = false;
            position->castle_black_long = false;
            break;
        case 'R':
            if (move->from.row == 0 && move->from.col == 0)
                position->castle_white_long = false;
            else if (move->from.row == 0 && move->from.col == 7)
                position->castle_white_short = false;
            break;
        case 'r':
            if (move->from.row == 7 && move->from.col == 0)
                position->castle_black_long = false;
            else if (move->from.row == 7 && move->from.col == 7)
                position->castle_black_short = false;
            break;
    }

    // move piece
    position->board[move->to.row][move->to.col] = new_piece;
    position->board[move->from.row][move->from.col] = '.';

    // flip player to move
    position->white_to_move = !position->white_to_move;
}

void position_format(const position_t* position, string_t* string) {
    for (size_t row = 8; row > 0;) {
        --row;
        string_append_format(string, "%c  %c %c %c %c %c %c %c %c",
                (char)(row + '1'),
                position->board[row][0], position->board[row][1],
                position->board[row][2], position->board[row][3],
                position->board[row][4], position->board[row][5],
                position->board[row][6], position->board[row][7]);

        switch (row) {
            case 1:
                string_append_format(string, "  %s to move", position->white_to_move ? "white" : "black");
                break;
            case 2:
                if (coords_is_valid(position->en_passant) &&
                        // don't print en-passant castling squares
                        position->en_passant.row != 0 && position->en_passant.row != 7)
                    string_append_format(string, "  en-passant square: %c%c",
                            position->en_passant.col + 'a',
                            position->en_passant.row + '1');
                break;
            case 3:
                string_append_format(string, "  white short castling: %sallowed",
                        position->castle_white_short ? "" : "not ");
                break;
            case 4:
                string_append_format(string, "  white long castling: %sallowed",
                        position->castle_white_long ? "" : "not ");
                break;
            case 5:
                string_append_format(string, "  black short castling: %sallowed",
                        position->castle_black_short ? "" : "not ");
                break;
            case 6:
                string_append_format(string, "  black long castling: %sallowed",
                        position->castle_black_long ? "" : "not ");
                break;
        }

        string_append_char(string, '\n');
    }
    string_append_cstr(string, "\n   a b c d e f g h");
}
