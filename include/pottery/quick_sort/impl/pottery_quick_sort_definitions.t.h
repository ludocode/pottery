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

#ifndef POTTERY_QUICK_SORT_IMPL
#error "This is an internal header. Do not include it."
#endif

/*
 * Chooses a pivot, returning its ref.
 *
 * We always_inline this function because only one implementation of
 * partitioning should survive optimization but they may not be culled until
 * after inlining is done.
 */
pottery_always_inline static
pottery_quick_sort_ref_t pottery_quick_sort_choose_pivot(
        POTTERY_QUICK_SORT_ARGS
        size_t start_index, size_t end_index)
{
    // Pivot elements are labeled a through i, where a is first and i is last.
    // Suffix _i is index, _e is entry, _r is ref.

    // Select values needed for median

    size_t half = (end_index - start_index) / 2;
    size_t a_i = start_index;
    size_t e_i = start_index + half; // avoid overflow
    size_t i_i = end_index;

    pottery_quick_sort_entry_t a_e = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS a_i);
    pottery_quick_sort_entry_t e_e = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS e_i);
    pottery_quick_sort_entry_t i_e = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS i_i);

    pottery_quick_sort_ref_t a_r = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL a_e);
    pottery_quick_sort_ref_t e_r = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL e_e);
    pottery_quick_sort_ref_t i_r = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL i_e);

    // If there is at most some arbitrary threshold of elements, use median
    // only. This prevents us from doing too many comparisons to choose a pivot
    // on a tiny partition.
    if (end_index - start_index + 1 <= 32) {
        pottery_quick_sort_ref_t m_r = pottery_quick_sort_compare_median(
                POTTERY_QUICK_SORT_CONTEXT_VAL a_r, e_r, i_r);
        return m_r;
    }

    // Select remaining values needed for median-of-medians

    size_t quarter = half / 2;
    size_t eighth = quarter / 2;
    size_t b_i = a_i + eighth;
    size_t c_i = a_i + quarter;
    size_t d_i = e_i - eighth;
    size_t f_i = e_i + eighth;
    size_t g_i = i_i - quarter;
    size_t h_i = i_i - eighth;

    pottery_quick_sort_entry_t b_e = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS b_i);
    pottery_quick_sort_entry_t c_e = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS c_i);
    pottery_quick_sort_entry_t d_e = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS d_i);
    pottery_quick_sort_entry_t f_e = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS f_i);
    pottery_quick_sort_entry_t g_e = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS g_i);
    pottery_quick_sort_entry_t h_e = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS h_i);

    pottery_quick_sort_ref_t b_r = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL b_e);
    pottery_quick_sort_ref_t c_r = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL c_e);
    pottery_quick_sort_ref_t d_r = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL d_e);
    pottery_quick_sort_ref_t f_r = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL f_e);
    pottery_quick_sort_ref_t g_r = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL g_e);
    pottery_quick_sort_ref_t h_r = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL h_e);

    pottery_quick_sort_ref_t m_r =
            pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL
                pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL a_r, b_r, c_r),
                pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL d_r, e_r, f_r),
                pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL g_r, h_r, i_r));

    return m_r;
}

// Instantiate our four means of partitioning. See partition.t.h for details.

#if POTTERY_LIFECYCLE_CAN_MOVE && POTTERY_CONTAINER_TYPES_HAS_VALUE
    #define POTTERY_QUICK_SORT_PARTITION_FAT 0
    #define POTTERY_QUICK_SORT_PARTITION_MOVE 1
    #define pottery_quick_sort_partition_impl pottery_quick_sort_partition_thin_move
    #include "pottery/quick_sort/impl/pottery_quick_sort_partition.t.h"

    #define POTTERY_QUICK_SORT_PARTITION_FAT 1
    #define POTTERY_QUICK_SORT_PARTITION_MOVE 1
    #define pottery_quick_sort_partition_impl pottery_quick_sort_partition_fat_move
    #include "pottery/quick_sort/impl/pottery_quick_sort_partition.t.h"
#endif

#define POTTERY_QUICK_SORT_PARTITION_FAT 0
#define POTTERY_QUICK_SORT_PARTITION_MOVE 0
#define pottery_quick_sort_partition_impl pottery_quick_sort_partition_thin_swap
#include "pottery/quick_sort/impl/pottery_quick_sort_partition.t.h"

#define POTTERY_QUICK_SORT_PARTITION_FAT 1
#define POTTERY_QUICK_SORT_PARTITION_MOVE 0
#define pottery_quick_sort_partition_impl pottery_quick_sort_partition_fat_swap
#include "pottery/quick_sort/impl/pottery_quick_sort_partition.t.h"

static inline
void pottery_quick_sort_partition(
        POTTERY_QUICK_SORT_ARGS
        size_t* first_index, size_t* last_index)
{
    // There must be at least two values in the partition
    pottery_assert(*last_index - *first_index >= 1);

    // We use fat partitioning if we have a user-defined three-way comparison
    // expression or if we are comparing by value (and the type is trivial or
    // spaceship-comparable in C++.)

    if (false
        // If the user has explicitly declared whether to use fat partitioning,
        // we obey the user's wishes.
        #ifdef POTTERY_QUICK_SORT_USE_FAT_PARTITION
            #if POTTERY_QUICK_SORT_USE_FAT_PARTITION
                || true
            #endif
        #else
            // A user-defined three-way comparison expression always enables
            // fat partitioning.
            #if POTTERY_COMPARE_HAS_USER_THREE_WAY
            || true
            #endif

            // If we're comparing by value, we may be able to enable fat
            // partitioning...
            #if POTTERY_COMPARE_BY_VALUE
                #ifdef __cplusplus
                    // In C++, enable fat partitioning if the type is trivial
                    || std::is_trivial<pottery_quick_sort_value_t>::value
                    #if __cplusplus > 202002L
                        // Or if it has a custom spaceship operator
                        // This is not yet implemented.
                        //|| pottery::has_spaceship_operator<pottery_quick_sort_value_t>::value
                    #endif
                    // Otherwise we don't enable it. We don't want fat
                    // partitioning when sorting e.g. std::string pre-C++20
                    // even though we're sorting by value because redundant
                    // ordering comparisons are expensive!
                #else
                    // We always enable fat partitioning for by-value
                    // comparisons in C.
                    || true
                #endif
            #endif
        #endif
    ) {
        #if POTTERY_LIFECYCLE_CAN_MOVE && POTTERY_CONTAINER_TYPES_HAS_VALUE
            pottery_quick_sort_partition_fat_move(POTTERY_QUICK_SORT_VALS first_index, last_index);
        #else
            pottery_quick_sort_partition_fat_swap(POTTERY_QUICK_SORT_VALS first_index, last_index);
        #endif
    } else {
        #if POTTERY_LIFECYCLE_CAN_MOVE && POTTERY_CONTAINER_TYPES_HAS_VALUE
            pottery_quick_sort_partition_thin_move(POTTERY_QUICK_SORT_VALS first_index, last_index);
        #else
            pottery_quick_sort_partition_thin_swap(POTTERY_QUICK_SORT_VALS first_index, last_index);
        #endif
    }
}

#ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
// We noinline the depth fallback to keep it out of the main implementation
// since it should be rarely used.
pottery_noinline static
void pottery_quick_sort_depth_fallback(
        POTTERY_QUICK_SORT_ARGS
        size_t first, size_t count)
{
    // We have to expand POTTERY_QUICK_SORT_VALS manually because the
    // fallback might be a macro.
    #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
        #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
            POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK(context, base, first, count);
        #else
            POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK(context, first, count);
        #endif
    #else
        #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
            POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK(base, first, count);
        #else
            POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK(first, count);
        #endif
    #endif
}
#endif

static inline
bool pottery_quick_sort_fallback(
        POTTERY_QUICK_SORT_ARGS
        size_t first, size_t count
        #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
        , size_t depth, size_t depth_limit
        #endif
) {
    POTTERY_QUICK_SORT_ARGS_UNUSED;
    (void)first;
    (void)count;

    // Switch to the count limit fallback if we don't have enough elements
    size_t count_limit = POTTERY_QUICK_SORT_COUNT_LIMIT;
    if (count <= count_limit) {
        // We have to expand POTTERY_QUICK_SORT_VALS manually because
        // the fallback might be a macro.
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
                POTTERY_QUICK_SORT_COUNT_LIMIT_FALLBACK(context, base, first, count);
            #else
                POTTERY_QUICK_SORT_COUNT_LIMIT_FALLBACK(context, first, count);
            #endif
        #else
            #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
                POTTERY_QUICK_SORT_COUNT_LIMIT_FALLBACK(base, first, count);
            #else
                POTTERY_QUICK_SORT_COUNT_LIMIT_FALLBACK(first, count);
            #endif
        #endif
        return true;
    }

    // Switch to the depth limit fallback if we're too deep
    #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
    if (pottery_unlikely(depth == depth_limit)) {
        pottery_quick_sort_depth_fallback(
                POTTERY_QUICK_SORT_VALS
                first, count);
        return true;
    }
    #endif

    return false;
}

POTTERY_QUICK_SORT_EXTERN
void pottery_quick_sort_range(
        POTTERY_QUICK_SORT_ARGS
        size_t offset,
        size_t range_count
) {
    POTTERY_QUICK_SORT_ARGS_UNUSED;

    if (range_count <= 1)
        return;

    // Set up the stack
    // We push the small partition, so each new stack element is at most half
    // the size of its parent. Since the maximum count is the range of a
    // size_t, it's not possible to use more than sizeof(size_t)*CHAR_BIT
    // entries.
    struct {
        size_t first;
        size_t last;
        #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
        size_t depth;
        #endif
    } stack[sizeof(size_t) * CHAR_BIT];
    size_t pos = 0;
    stack[0].first = offset;
    stack[0].last = offset + range_count - 1;
    #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
    stack[0].depth = 0;
    #endif

    // set up depth limit for switching to fallback
    #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
    size_t n = 1;
    size_t depth_limit = 2;
    while (n < range_count) {
        n *= 2;
        depth_limit += 2;
    }
    #endif

    while (true) {
        size_t first = stack[pos].first;
        size_t last = stack[pos].last;
        pottery_assert(last > first);
        size_t step_count = last - first + 1;

        // see if we should use any fallbacks
        if (pottery_quick_sort_fallback(
                POTTERY_QUICK_SORT_VALS
                first, step_count
                #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
                , stack[pos].depth, depth_limit
                #endif
                )) {
            if (pos == 0)
                break;
            --pos;
            continue;
        }

        // do the three-way partition. the center partition (equal to the
        // pivot) is in its correct final position; we loop around for the left
        // and right partitions.
        size_t equal_first = first;
        size_t equal_last = last;
        pottery_quick_sort_partition(POTTERY_QUICK_SORT_VALS &equal_first, &equal_last);
        size_t left_count = equal_first - first;
        size_t right_count = last - equal_last;

        // if both sides are degenerate, pop
        if (left_count <= 1 && right_count <= 1) {
            if (pos == 0)
                break;
            --pos;
            continue;
        }

        // replace the top stack entry with the long side, push the small side
        #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
        size_t depth = stack[pos].depth + 1;
        #endif
        if (left_count < right_count) {
            stack[pos].first = last - right_count + 1;
            stack[pos].last = last;
            #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
            stack[pos].depth = depth;
            #endif

            if (left_count > 1) {
                ++pos;
                pottery_assert(pos != pottery_array_count(stack)); // not possible!

                stack[pos].first = first;
                stack[pos].last = first + left_count - 1;
                #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
                stack[pos].depth = depth;
                #endif
            }

        } else {
            stack[pos].first = first;
            stack[pos].last = first + left_count - 1;
            #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
            stack[pos].depth = depth;
            #endif

            if (right_count > 1) {
                ++pos;
                pottery_assert(pos != pottery_array_count(stack)); // not possible!

                stack[pos].first = last - right_count + 1;
                stack[pos].last = last;
                #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
                stack[pos].depth = depth;
                #endif
            }
        }
    }
}
