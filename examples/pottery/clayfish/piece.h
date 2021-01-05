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

#ifndef PIECE_H
#define PIECE_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A piece.
 *
 * Pieces are represented by the ASCII character corresponding for the first
 * letter of their name (except the knight which is 'n'.) Uppercase is white,
 * lowercase is black and '.' is an empty square.
 */
typedef char piece_t;

// Returns true if the piece is white, false if the piece is black, and
// some undefined value otherwise.
static inline
bool piece_is_white(piece_t c) {
    return !(c & 0x20); // uppercase letter
}

// Makes the given piece (of any color) black.
// Returns some undefined value if it's an empty square or not a valid piece.
static inline
char piece_make_black(piece_t c) {
    return c | 0x20;
}

// Makes the given piece (of any color) white.
// Returns some undefined value if it's an empty square or not a valid piece.
static inline
char piece_make_white(piece_t c) {
    return c & ~0x20;
}

#ifdef __cplusplus
}
#endif

#endif
