#ifndef EVAL_H
#define EVAL_H

#include "piece.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct position_t;

/**
 * Returns the (positive) value of the given piece.
 *
 * The value is the same for black and white pieces.
 */
score_t eval_piece(piece_t piece);

/**
 * Evaluates the given position, returning its score.
 */
score_t eval(const struct position_t* position);

#ifdef __cplusplus
}
#endif

#endif
