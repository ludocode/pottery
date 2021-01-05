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

#include "move.h"

#include "moves.t.h"
#include "pottery/vector/pottery_vector_define.t.h"

void move_format(move_t* move, char buf[6]) {
    if (coords_is_null(move->from)) {
        strcpy(buf, "0000");
        return;
    }

    buf[0] = (char)(move->from.col + 'a'),
    buf[1] = (char)(move->from.row + '1'),
    buf[2] = (char)(move->to.col + 'a'),
    buf[3] = (char)(move->to.row + '1');
    if (move->promotion != '.') {
        buf[4] = piece_make_black(move->promotion);
        buf[5] = '\0';
    } else {
        buf[4] = '\0';
    }
}

void move_print(move_t* move) {
    char buf[6];
    move_format(move, buf);
    fputs(buf, stdout);
}

static inline bool move_parse_coord(char input, char base, signed char* out) {
    if (input < base || input > base + 7)
        return false;
    *out = (signed char)(input - base);
    return true;
}

bool move_parse(move_t* move, string_t* string, bool white_to_move) {
    const char* cstr = string_cstr(string);
    size_t len = string_length(string);
    if (len != 4 && len != 5)
        return false;

    if (!move_parse_coord(cstr[0], 'a', &move->from.col) ||
            !move_parse_coord(cstr[1], '1', &move->from.row) ||
            !move_parse_coord(cstr[2], 'a', &move->to.col) ||
            !move_parse_coord(cstr[3], '1', &move->to.row))
        return false;

    if (len == 5) {
        piece_t promotion = piece_make_black(cstr[4]);
        if (promotion != 'q' && promotion != 'r' &&
                promotion != 'b' && promotion != 'n')
            return false;
        move->promotion = white_to_move ? piece_make_white(promotion) : promotion;
    } else {
        move->promotion = '.';
    }

    return true;
}
