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

#ifndef CLAYFISH_H
#define CLAYFISH_H

#include "move.h"

#ifdef __cplusplus
extern "C" {
#endif

struct position_t;

/**
 * Searches for the best move at the given position.
 *
 * The given out_candidate will be assigned as soon as Clayfish finds a
 * reasonable move and will change as Clayfish finds better moves. You can use
 * it before this is complete to show the best move found so far. When
 * complete, if true is returned, this contains the best move found in the
 * search.
 *
 * If non-zero times are given, Clayfish will think for an amount of time it
 * deems reasonable. Otherwise Clayfish will think until the search tree is
 * exhausted.
 *
 * keep_going(context) will be called periodically. Use it to update UI and/or
 * check for termination. Return false to stop Clayfish from thinking.
 *
 * The given position must be valid (i.e. both players must have kings, the
 * last player that moved cannot be in check, pawns cannot be on the first or
 * last rank, etc.)
 *
 * @return true if a valid move was assigned to out_move; false otherwise.
 */
void search(
        const struct position_t* position,
        int time_ms, int increment_ms, int movetime_ms,
        moves_t* out_pv, int* out_score, int* out_depth,
        bool (*keep_going)(void* context),
        void (*depth_complete)(void* context),
        void* context);

#ifdef __cplusplus
}
#endif

#endif
