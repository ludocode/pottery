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

#include "eval.h"

#include "common.h"
#include "position.h"
#include "move.h"
#include "generate.h"

// Basic piece values
#define CLAYFISH_PAWN_VALUE 100
#define CLAYFISH_KNIGHT_VALUE 300
#define CLAYFISH_BISHOP_VALUE 300
#define CLAYFISH_ROOK_VALUE 500
#define CLAYFISH_QUEEN_VALUE 850
#define CLAYFISH_KING_VALUE 100000

// Mobility bonuses
#define CLAYFISH_BONUS_MOBILITY_KING 0
#define CLAYFISH_BONUS_MOBILITY_QUEEN 3
#define CLAYFISH_BONUS_MOBILITY_ROOK 0
#define CLAYFISH_BONUS_MOBILITY_BISHOP 5
#define CLAYFISH_BONUS_MOBILITY_KNIGHT 10
#define CLAYFISH_BONUS_MOBILITY_PAWN 4

// Centering bonuses
// (These are actually calculated as penalties for distance from center)
#define CLAYFISH_BONUS_CENTERING_KING 10 // not in opening, ramps up to endgame
#define CLAYFISH_BONUS_CENTERING_QUEEN 0
#define CLAYFISH_BONUS_CENTERING_ROOK 0
#define CLAYFISH_BONUS_CENTERING_BISHOP 10
#define CLAYFISH_BONUS_CENTERING_KNIGHT 20
#define CLAYFISH_BONUS_CENTERING_PAWN 5

// Other bonuses
#define CLAYFISH_BISHOP_PAIR_BONUS 25
#define CLAYFISH_PAWN_ADVANCE_BONUS 6 // should be higher than pawn centering
#define CLAYFISH_KING_SAFETY_BONUS 50
#define CLAYFISH_CASTLING_RIGHTS_BONUS 8
#define CLAYFISH_ROOK_7TH_BONUS 25
#define CLAYFISH_KINGS_DISTANCE_BONUS_NUM 350 // per manhattan distance, numerator
#define CLAYFISH_KINGS_DISTANCE_BONUS_DENOM 10000 // denomenator

// Opening material threshold
// We set the opening phase to a bit less than all material (three knights, a
// bishop and four pawns off) so we can exchange a few pieces before we start
// losing king safety bonuses.
#define CLAYFISH_PHASE_OPENING ( \
        12 * CLAYFISH_PAWN_VALUE + \
        2 * CLAYFISH_ROOK_VALUE + \
        1 * CLAYFISH_KNIGHT_VALUE + \
        3 * CLAYFISH_BISHOP_VALUE + \
        2 * CLAYFISH_QUEEN_VALUE)

// Endgame material threshold
// Our endgame is rooks, some pawns and a knight each.
#define CLAYFISH_PHASE_ENDGAME ( \
        10 * CLAYFISH_PAWN_VALUE + \
        2 * CLAYFISH_KNIGHT_VALUE + \
        4 * CLAYFISH_ROOK_VALUE)

score_t eval_piece(piece_t piece) {
    switch (piece) {
        case 'P': case 'p': return CLAYFISH_PAWN_VALUE; break;
        case 'K': case 'k': return CLAYFISH_KING_VALUE; break;
        case 'N': case 'n': return CLAYFISH_KNIGHT_VALUE; break;
        case 'B': case 'b': return CLAYFISH_BISHOP_VALUE; break;
        case 'R': case 'r': return CLAYFISH_ROOK_VALUE; break;
        case 'Q': case 'q': return CLAYFISH_QUEEN_VALUE; break;
        default: break;
    }
    return 0;
}

clayfish_always_inline static
score_t eval_king_safety(const position_t* position, coords_t coords,
        coords_t step, bool white, score_t bonus)
{
    // we give the king safety bonus if a pawn is in position.
    coords = coords_add(coords, step);
    if (!coords_is_valid(coords))
        return 0;
    char piece = position_piece_at(position, coords);
    if (piece != (white ? 'P' : 'p'))
        return 0;
    return bonus;
}

score_t eval(const position_t* position) {
    score_t score = 0;

    // Material for each player (not counting kings)
    score_t white_material = 0;
    score_t black_material = 0;

    // Number of bishops for bishop pair bonus
    int black_bishop_count = 0;
    int white_bishop_count = 0;

    // King locations for king safety bonuses
    coords_t white_king = coords_make_null();
    coords_t black_king = coords_make_null();

    // A scratch buffer for mobility bonuses
    moves_t scratch_moves;
    moves_init(&scratch_moves);

    for (signed char row = 0; row < 8; ++row) {
        for (signed char col = 0; col < 8; ++col) {
            coords_t coords = coords_make(row, col);
            char piece = position_piece_at(position, coords);
            if (piece == '.')
                continue;

            score_t mobility_bonus = 0;
            int center_distance = coords_manhattan_center_distance(coords);

            // material and piece-specific bonuses
            switch (piece) {

                // bonus for advancing pawns
                case 'P':
                    white_material += CLAYFISH_PAWN_VALUE;
                    score += (coords.row - 1) * CLAYFISH_PAWN_ADVANCE_BONUS;
                    mobility_bonus = CLAYFISH_BONUS_MOBILITY_PAWN;
                    score -= center_distance * CLAYFISH_BONUS_CENTERING_PAWN;
                    break;
                case 'p':
                    black_material += CLAYFISH_PAWN_VALUE;
                    score -= (6 - coords.row) * CLAYFISH_PAWN_ADVANCE_BONUS;
                    mobility_bonus = -CLAYFISH_BONUS_MOBILITY_PAWN;
                    score += center_distance * CLAYFISH_BONUS_CENTERING_PAWN;
                    break;

                // record king positions for king safety bonus
                case 'K':
                    score += CLAYFISH_KING_VALUE;
                    white_king = coords;
                    mobility_bonus = CLAYFISH_BONUS_MOBILITY_KING;
                    break;
                case 'k':
                    score -= CLAYFISH_KING_VALUE;
                    black_king = coords;
                    mobility_bonus = -CLAYFISH_BONUS_MOBILITY_KING;
                    break;

                // record bishop counts for bishop pair bonus
                case 'B':
                    ++white_bishop_count;
                    white_material += CLAYFISH_BISHOP_VALUE;
                    mobility_bonus = CLAYFISH_BONUS_MOBILITY_BISHOP;
                    score -= center_distance * CLAYFISH_BONUS_CENTERING_BISHOP;
                    break;
                case 'b':
                    ++black_bishop_count;
                    black_material += CLAYFISH_BISHOP_VALUE;
                    mobility_bonus = -CLAYFISH_BONUS_MOBILITY_BISHOP;
                    score += center_distance * CLAYFISH_BONUS_CENTERING_BISHOP;
                    break;

                // bonus for rooks on the 7th
                case 'R':
                    if (row == 6)
                        score += CLAYFISH_ROOK_7TH_BONUS;
                    white_material += CLAYFISH_ROOK_VALUE;
                    mobility_bonus = CLAYFISH_BONUS_MOBILITY_ROOK;
                    score -= center_distance * CLAYFISH_BONUS_CENTERING_ROOK;
                    break;
                case 'r':
                    if (row == 1)
                        score -= CLAYFISH_ROOK_7TH_BONUS;
                    black_material += CLAYFISH_ROOK_VALUE;
                    mobility_bonus = -CLAYFISH_BONUS_MOBILITY_ROOK;
                    score += center_distance * CLAYFISH_BONUS_CENTERING_ROOK;
                    break;

                case 'N':
                    white_material += CLAYFISH_KNIGHT_VALUE;
                    mobility_bonus = CLAYFISH_BONUS_MOBILITY_KNIGHT;
                    score -= center_distance * CLAYFISH_BONUS_CENTERING_KNIGHT;
                    break;
                case 'n':
                    black_material += CLAYFISH_KNIGHT_VALUE;
                    mobility_bonus = -CLAYFISH_BONUS_MOBILITY_KNIGHT;
                    score += center_distance * CLAYFISH_BONUS_CENTERING_KNIGHT;
                    break;

                case 'Q':
                    white_material += CLAYFISH_QUEEN_VALUE;
                    mobility_bonus = CLAYFISH_BONUS_MOBILITY_QUEEN;
                    score -= center_distance * CLAYFISH_BONUS_CENTERING_QUEEN;
                    break;
                case 'q':
                    black_material += CLAYFISH_QUEEN_VALUE;
                    mobility_bonus = -CLAYFISH_BONUS_MOBILITY_QUEEN;
                    score += center_distance * CLAYFISH_BONUS_CENTERING_QUEEN;
                    break;

                default:
                    break;
            }

            if (mobility_bonus != 0) {
                moves_remove_all(&scratch_moves);
                generate_moves_at(position, coords, &scratch_moves);
                score += (score_t)moves_count(&scratch_moves) * mobility_bonus;
            }
        }
    }

    moves_destroy(&scratch_moves);

    // material balance
    score += white_material - black_material;

    // bishop pair bonus
    if (white_bishop_count >= 2) score += CLAYFISH_BISHOP_PAIR_BONUS;
    if (black_bishop_count >= 2) score -= CLAYFISH_BISHOP_PAIR_BONUS;

    (void)black_king;
    (void)white_king;
    #if 1
    // calculate game phase based on total material.
    // phase_factor/phase_divisor is 1 for opening and 0 for endgame.
    score_t phase_divisor = CLAYFISH_PHASE_OPENING - CLAYFISH_PHASE_ENDGAME;
    score_t phase_factor = white_material + black_material - CLAYFISH_PHASE_ENDGAME;
    if (phase_factor < 0)
        phase_factor = 0;
    else if (phase_factor > phase_divisor)
        phase_factor = phase_divisor;
    //printf("f %i d %i %% %f\n",phase_factor,phase_divisor,(double)phase_factor/(double)phase_divisor);

    // bonuses before endgame
    if (phase_factor > 0) {

        // bonus to preserve castling rights
        score_t castling_bonus = CLAYFISH_CASTLING_RIGHTS_BONUS * phase_factor / phase_divisor;
        if (position->castle_white_short) score += castling_bonus;
        if (position->castle_white_long)  score += castling_bonus;
        if (position->castle_black_short) score -= castling_bonus;
        if (position->castle_black_long)  score -= castling_bonus;

        // king safety bonus
        // gives a bonus for each pawn in the two rows in front of the king,
        // but only if the king is on the back rank and not in the middle
        score_t king_safety_bonus = CLAYFISH_KING_SAFETY_BONUS * phase_factor / phase_divisor;
        if (king_safety_bonus > 0) {
            if (white_king.row == 0 && white_king.col != 3 && white_king.col != 4 && white_king.col != 5) {
                static const coords_t guards[] = {{1, -1}, {1, 0}, {1, 1}, {2, -1}, {2, 0}, {2, 1}};
                for (size_t i = 0; i < clayfish_array_count(guards); ++i)
                    score += eval_king_safety(position, white_king, guards[i], true, king_safety_bonus);
            }
            if (black_king.row == 0 && black_king.col != 3 && black_king.col != 4 && white_king.col != 5) {
                static const coords_t guards[] = {{-1, -1}, {-1, 0}, {-1, 1}, {-2, -1}, {-2, 0}, {-2, 1}};
                for (size_t i = 0; i < clayfish_array_count(guards); ++i)
                    score -= eval_king_safety(position, black_king, guards[i], false, king_safety_bonus);
            }
        }
    }

    // bonuses after opening
    if (phase_factor < phase_divisor) {

        // King centering bonus, ramps up towards endgame
        score_t king_centering_bonus = CLAYFISH_BONUS_CENTERING_KING * (phase_divisor - phase_factor) / phase_divisor;
        //printf("king centering score change %i %i\n",coords_manhattan_center_distance(white_king) * king_centering_bonus,coords_manhattan_center_distance(black_king) * king_centering_bonus);
        score -= coords_manhattan_center_distance(white_king) * king_centering_bonus;
        score += coords_manhattan_center_distance(black_king) * king_centering_bonus;

        // King attack/defend based on material balance. The player with more
        // material wants to minimize distance between kings; the player with less
        // wants the opposite.
        score_t kings_distance_bonus = (white_material - black_material) *
                CLAYFISH_KINGS_DISTANCE_BONUS_NUM / CLAYFISH_KINGS_DISTANCE_BONUS_DENOM;
        //printf("white mat %i black mat %i distance %i bonus %i phased %i\n", white_material, black_material, coords_manhattan_distance(white_king, black_king), kings_distance_bonus, kings_distance_bonus * (phase_divisor - phase_factor) / phase_divisor);
        kings_distance_bonus = kings_distance_bonus * (phase_divisor - phase_factor) / phase_divisor;
        score -= coords_manhattan_distance(white_king, black_king) * kings_distance_bonus;
        //printf("king distance score change %i\n",coords_manhattan_distance(white_king, black_king) * kings_distance_bonus);
    }
    #endif

    return score;
}
