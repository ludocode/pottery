#ifndef GENERATE_H
#define GENERATE_H

#include "coords.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct position_t;
struct moves_t;

/**
 * Generates all pseudo-legal moves for the player to move in the given
 * position, appending them to the given moves vector.
 *
 * The king may be vulnerable to capture after these moves, but they will
 * otherwise be valid. A separate test is needed to ensure that the move does
 * not leave the king in check.
 *
 * The given position must be legal. If it is not, the behaviour is undefined.
 */
void generate_moves(const struct position_t* position, struct moves_t* moves);

/**
 * Generates all pseudo-legal moves for a piece at the given coordinates
 * (regardless of whether it's their turn to move), appending them to the given
 * moves vector.
 *
 * The given position must be legal. If it is not, the behaviour is undefined.
 */
void generate_moves_at(const struct position_t* position, coords_t coords, struct moves_t* moves);

#ifdef __cplusplus
}
#endif

#endif
