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
