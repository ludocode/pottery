#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>

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
