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

#include "search.h"

#include "common.h"
#include "position.h"
#include "generate.h"
#include "eval.h"

// how long we think between checking whether we should stop
#define STEPS_PER_STOP_CHECK 20000

// our guess as to how many moves are left in the game
#define GUESS_MOVES_LEFT 25

// maximum additional depth for a quiescence search
#define QUIESCENCE_DEPTH 6

typedef struct search_t {
    int max_depth;

    // the principle variation from the last depth iteration
    moves_t* pv;

    // whether we should keep going
    bool keep_going;

    // how many more steps until the next time we check whether to stop
    int steps_left;

    // stop when we reach the end time or when the callback returns false
    bool has_end_time;
    double end_time;
    bool (*keep_going_callback)(void* context);
    void* context;
} search_t;

typedef struct search_sort_t {
    const position_t* position;
    move_t pv_move;
} search_sort_t;

/**
 * Performs a three-way comparison between moves. More interesting moves come
 * before less interesting moves since they are more likely to trigger
 * alpha/beta cutoffs.
 */
static int search_compare_moves(search_sort_t context, move_t* left, move_t* right) {

    // Check for the pv move. It comes first.
    if (move_equal(left, &context.pv_move))
        return -1;
    if (move_equal(right, &context.pv_move))
        return 1;

    // Check for promotions. Prefer whichever promotes to the biggest piece.
    if (left->promotion != right->promotion)
        return abs(eval_piece(right->promotion)) - abs(eval_piece(left->promotion));

    // Check for captures. Prefer whichever captures the biggest piece.
    piece_t left_capture = position_piece_at(context.position, left->to);
    piece_t right_capture = position_piece_at(context.position, right->to);
    return abs(eval_piece(right_capture)) - abs(eval_piece(left_capture));
}

// Instantiate a sort algorithm to sort a move list in order to test the most
// interesting moves first. This improves alpha-beta pruning.
#define POTTERY_QUICK_SORT_PREFIX search_sort_moves
#define POTTERY_QUICK_SORT_VALUE_TYPE move_t
#define POTTERY_QUICK_SORT_CONTEXT_TYPE search_sort_t
#define POTTERY_QUICK_SORT_LIFECYCLE_BY_VALUE 1
#define POTTERY_QUICK_SORT_COMPARE_THREE_WAY search_compare_moves
#include "pottery/quick_sort/pottery_quick_sort_static.t.h"

static bool search_should_keep_going(search_t* search) {
    if (!search->keep_going)
        return false;

    if (search->steps_left == 0) {

        // call the callback
        if (search->keep_going_callback != NULL && !search->keep_going_callback(search->context)) {
            search->keep_going = false;
            return false;
        }

        // check the time
        if (search->has_end_time) {
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            if (search->end_time <= (double)ts.tv_sec + (double)ts.tv_nsec / 1.e9) {
                search->keep_going = false;
                return false;
            }
        }

        search->steps_left = STEPS_PER_STOP_CHECK;
    }

    --search->steps_left;
    return true;
}

// Some simple log macros to help us debug search
#if 0
#define log_depth(depth) do {for (size_t _d = 0; _d < (size_t)(depth); ++_d) printf("  ");} while(0)
#define log(...) printf(__VA_ARGS__)
#define log_move(move) move_print(move)
#else
#define log_depth(depth) ((void)0)
#define log(...) ((void)0)
#define log_move(move) ((void)0)
#endif

/**
 * Recursive function to perform one depth level of search.
 *
 * Returns false to immediately abandon the search.
 *
 * max_depth is the depth at which we switch to a quiescense search. depth can
 * go deeper than this.
 */
static score_t search_level(search_t* search, const position_t* position, int depth,
        score_t alpha, score_t beta, moves_t* out_pv)
{
    moves_remove_all(out_pv);

    // Quiescense search: if we're above the maximum depth, calculate the eval
    // of the node, but also continue to recurse through captures to see if we
    // can improve the score.
    bool quiesce = depth >= search->max_depth;
    if (quiesce) {

        // Evaluation. This is a negamax search so if we're searching for black
        // we negate the eval.
        score_t stand_pat = eval(position) * (position->white_to_move ? 1 : -1);

        log_depth(depth);
        log("In quiescence search. Stand-pat score %i on (%i,%i): ", stand_pat, alpha, beta);

        if (stand_pat >= beta) {
            log(" Fail high. Abandoning branch.\n");
            return beta;
        }

        if (stand_pat < alpha) {
            log(" Fail low. ");
        } else {
            log(" Pass. Raising alpha to %i. ", stand_pat);
            alpha = stand_pat;
        }

        if (depth >= search->max_depth + QUIESCENCE_DEPTH) {
            log("Stopping quiescence search due to depth.\n");
            return alpha;
        }

        log("Continuing quiescence search.\n");
    }

    moves_t moves;
    moves_init(&moves);
    generate_moves(position, &moves);

    // Sort moves, pv move first. (Note that we may not actually be on the
    // principal variation, so this move may not even exist in the move list.)
    search_sort_t context = {
        position,
        (moves_count(search->pv) > (size_t)depth) ? *moves_at(search->pv, (size_t)depth) : move_make_null(),
    };
    search_sort_moves(context, moves_begin(&moves), moves_count(&moves));

    moves_t pv;
    moves_init(&pv);

    for (size_t i = 0; i < moves_count(&moves) && search_should_keep_going(search); ++i) {
        move_t* move = moves_at(&moves, i);

        // Skip non-captures during quiescence search.
        if (quiesce && '.' == position_piece_at(position, move->to))
            continue;

        log_depth(depth);
        log("Testing move ");
        log_move(move);
        log(" on (%i,%i)\n", alpha, beta);

        // Check if this move captures the king. If so this is automatically
        // the best move. We can't search deeper because it would allow a king
        // recapture to restore the material balance.
        if (position_is_move_king_capture(position, move)) {
            moves_insert_last(out_pv, *move);

            // From the perspective of the maximizing player, this is a
            // positive mate, so it's by white regardless of who's turn it is
            // to play.
            score_t score = score_mate_by_white - depth;

            log_depth(depth);
            log("Move is a king capture! Score is %i (depth %i). ", score, depth);

            if (score > beta) {
                log("Fail high.\n");
                alpha = beta;
            } else if (score > alpha) {
                log("Pass.\n");
                alpha = score;
            } else {
                log("Fail low. Ignoring\n");
            }

            break;
        }

        position_t new_position = *position;
        position_apply_move(&new_position, move);

        // At the root, we need to check our moves for legality. Otherwise if
        // a pseudo-move puts us in check but gets our king captured at the
        // same depth as a move that doesn't, we might pick it instead, which
        // means we'll make an illegal move.
        // We do this in the simplest way possible: check all opponent's moves
        // for king captures. This is slow, but we only do it at the root so we
        // don't worry about it.
        if (depth == 0) {
            moves_t opponent_moves;
            moves_init(&opponent_moves);
            generate_moves(&new_position, &opponent_moves);

            bool king_captured = false;
            for (size_t j = 0; j < moves_count(&opponent_moves); ++j) {
                if (position_is_move_king_capture(&new_position, moves_at(&opponent_moves, j))) {
                    king_captured = true;
                    break;
                }
            }

            moves_destroy(&opponent_moves);
            if (king_captured) {
                log_depth(depth);
                log("Move puts us in check! Skipping.\n");
                continue;
            }
        }

        // Alpha-beta negamax recursion
        score_t score = -search_level(search, &new_position, depth + 1, -beta, -alpha, &pv);

        log_depth(depth);
        log("Move ");
        log_move(move);
        log(" scored %i on (%i,%i):", score, alpha, beta);

        if (score >= beta) {
            log(" Fail high. Abandoning branch.\n");
            alpha = beta;
            break;
        }

        if (score <= alpha) {
            log(" Fail low. Ignored.\n");
            continue;
        }

        alpha = score;
        log("  PASS. now (%i,%i)\n", alpha, beta);
        moves_remove_all(out_pv);
        moves_insert_last(out_pv, *move);
        moves_insert_last_vector(out_pv, &pv);
    }

    moves_destroy(&pv);
    moves_destroy(&moves);
    log_depth(depth);
    log("Returning alpha %i\n", alpha);
    return alpha;
}

void search(
        const position_t* position,
        int time_ms, int increment_ms, int movetime_ms,
        moves_t* out_pv, score_t* out_score, int* out_depth,
        bool (*keep_going)(void* context),
        void (*depth_complete)(void* context),
        void* context)
{
    moves_remove_all(out_pv);
    *out_score = 0;
    *out_depth = 0;

    if (!position_is_legal(position))
        return;

    search_t search;
    search.pv = out_pv;
    search.keep_going = true;
    search.steps_left = STEPS_PER_STOP_CHECK;
    search.keep_going_callback = keep_going;
    search.context = context;

    // calculate time we should spend on this move. very simple heuristic:
    // assume there are a fixed number of moves left, use that fraction of the
    // total time including expected increment.
    if (time_ms < 0 && increment_ms < 0 && movetime_ms < 0) {
        search.has_end_time = false;
    } else {
        search.has_end_time = true;
        if (movetime_ms < 0)
            movetime_ms = time_ms / GUESS_MOVES_LEFT + increment_ms;
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        search.end_time = (double)ts.tv_sec + (double)ts.tv_nsec / 1.e9 + (double)movetime_ms / 1.e3;
    }

    // iterative deepening
    for (int max_depth = 1; max_depth < 30; ++max_depth) {
        log("\n\n\nStarting depth %i\n\n", max_depth);
        search.max_depth = max_depth;

        // pv will be modified incrementally, so we need a temporary so as to
        // not destroy the last pv when we early exit
        moves_t pv;
        moves_init(&pv);

        int score = search_level(&search, position, 0, INT_MIN + 1, INT_MAX, &pv);
        if (!search_should_keep_going(&search))
            break;

        moves_steal(out_pv, &pv);
        moves_destroy(&pv);

        *out_score = score;
        *out_depth = max_depth;

        if (depth_complete != NULL)
            depth_complete(context);
    }
}
