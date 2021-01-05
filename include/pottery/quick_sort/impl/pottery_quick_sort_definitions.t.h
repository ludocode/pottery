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
 * Implementation
 */

// The below implementation uses the Hoare partitioning method, except that it
// keeps the pivot at a fixed position outside of the loop during partitioning,
// that way we can compare it by reference without having to copy it to a
// temporary or track its position changes.

// Chooses a pivot and swaps it with the element at the start of the array.
static inline
void pottery_quick_sort_prepare_pivot(
        POTTERY_QUICK_SORT_ARGS
        size_t start_index, size_t end_index)
{
    if (end_index - start_index < 2) {
        // there are less than three elements so it doesn't matter which one
        // is the pivot. just use the first.
        return;
    }

    // choose elements for the median
    size_t middle_index = start_index + (end_index - start_index) / 2; // avoid overflow

    pottery_quick_sort_entry_t start_entry = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS start_index);
    pottery_quick_sort_entry_t middle_entry = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS middle_index);
    pottery_quick_sort_entry_t end_entry = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS end_index);

    pottery_quick_sort_ref_t start_ref = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL start_entry);
    pottery_quick_sort_ref_t middle_ref = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL middle_entry);
    pottery_quick_sort_ref_t end_ref = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL end_entry);

    // if there is at most some arbitrary threshold of elements, use median
    if (end_index - start_index + 1 <= 128) {
        pottery_quick_sort_ref_t median_ref = pottery_quick_sort_compare_median(
                POTTERY_QUICK_SORT_CONTEXT_VAL start_ref, middle_ref, end_ref);
        pottery_quick_sort_lifecycle_swap(POTTERY_QUICK_SORT_CONTEXT_VAL start_ref, median_ref);
        return;
    }

    // use ninther
    size_t third_offset = (end_index - start_index) / 3;
    size_t sixth_offset = (end_index - start_index) / 6;

    pottery_quick_sort_entry_t left_middle_entry = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS start_index + sixth_offset);
    pottery_quick_sort_entry_t left_end_entry = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS start_index + third_offset);
    pottery_quick_sort_entry_t right_start_entry = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS middle_index + third_offset);
    pottery_quick_sort_entry_t right_middle_entry = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS middle_index + third_offset + sixth_offset);

    pottery_quick_sort_ref_t left_middle_ref = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_middle_entry);
    pottery_quick_sort_ref_t left_end_ref = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_end_entry);
    pottery_quick_sort_ref_t right_start_ref = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_start_entry);
    pottery_quick_sort_ref_t right_middle_ref = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_middle_entry);

    pottery_quick_sort_ref_t median_ref = pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL
            pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL start_ref, left_middle_ref, left_end_ref),
            pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL left_end_ref, middle_ref, right_start_ref),
            pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL right_start_ref, right_middle_ref, end_ref));

    pottery_quick_sort_lifecycle_swap(POTTERY_QUICK_SORT_CONTEXT_VAL start_ref, median_ref);
}

// There are two implementations of partitioning here, one that moves through a
// temporary and one that swaps. Moving through a temporary is faster, but we
// need a move function and a value type to be able to declare a temporary.
//
// (Moving through a temporary results in roughly one third as many moves. This
// is only a marginal improvement when sorting small stuff like ints and
// pointers but can be huge when sorting large objects, especially ones that
// aren't bitwise movable.

#ifndef POTTERY_QUICK_SORT_USE_MOVE
    #if POTTERY_LIFECYCLE_CAN_MOVE && defined(POTTERY_QUICK_SORT_VALUE_TYPE)
        #define POTTERY_QUICK_SORT_USE_MOVE 1
    #else
        #define POTTERY_QUICK_SORT_USE_MOVE 0
    #endif
#endif

#if POTTERY_QUICK_SORT_USE_MOVE
static inline
size_t pottery_quick_sort_partition(
        POTTERY_QUICK_SORT_ARGS
        size_t start_index, size_t end_index)
{
    pottery_assert(end_index - start_index >= 1);
    pottery_quick_sort_entry_t pivot_entry = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS start_index);
    pottery_quick_sort_ref_t pivot = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL pivot_entry);

    size_t low_index = start_index;
    pottery_quick_sort_entry_t low = pivot_entry;

    size_t high_index = end_index;
    pottery_quick_sort_entry_t high = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS high_index);

    // Start by finding an element from the right that belongs on the left (or
    // is equal to the pivot. It's critical that we don't skip equal elements.)
    while (low_index < high_index && pottery_quick_sort_compare_less(
                POTTERY_QUICK_SORT_CONTEXT_VAL pivot,
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL high))) {
        high = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS --high_index);
        // TODO use next/previous here
        //high = pottery_quick_sort_array_access_previous(POTTERY_QUICK_SORT_VALS high);
        //--high_index;
    }

    // If we've found none, the pivot goes at the start. (Either this partition
    // is really short or we've chosen an unfortunate pivot.)
    if (low_index == high_index) {
        return start_index;
    }

    // Move the element out into a temporary, leaving a hole on the right at
    // this location
    size_t hole_index = high_index;
    pottery_quick_sort_entry_t hole = high;
    pottery_quick_sort_value_t temp;
    pottery_quick_sort_lifecycle_move_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
            &temp,
            pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL hole));

    // Now we alternate between low_index and high_index, finding elements on the wrong
    // side and moving them into the hole
    while (true) {

        // Scan from the left
        do {
            low = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS ++low_index);
            //low = pottery_quick_sort_array_access_next(POTTERY_QUICK_SORT_VALS low);
            //++low_index;
        } while (low_index < high_index && pottery_quick_sort_compare_greater(
                    POTTERY_QUICK_SORT_CONTEXT_VAL pivot,
                    pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL low)));

        if (low_index == high_index) {
            break;
        }

        // Put this element in the hole; the hole is now on the left.
        pottery_quick_sort_lifecycle_move_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL hole),
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL low));
        hole_index = low_index;
        hole = low;

        // Scan from the right
        do {
            high = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS --high_index);
            //high = pottery_quick_sort_array_access_previous(POTTERY_QUICK_SORT_VALS high);
            //--high_index;
        } while (low_index < high_index && pottery_quick_sort_compare_less(
                    POTTERY_QUICK_SORT_CONTEXT_VAL pivot,
                    pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL high)));

        if (low_index == high_index) {
            break;
        }

        // Put this element in the hole; the hole is now on the right.
        pottery_quick_sort_lifecycle_move_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL hole),
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL high));
        hole_index = high_index;
        hole = high;
    }

    // We've converged on the hole, and the temporary belongs on the left of
    // it. The pivot goes here and the temporary goes where the pivot was.
    (void)hole_index;
    pottery_assert(hole_index == high_index);
    pottery_quick_sort_lifecycle_move_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
            pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL hole),
            pivot);
    pottery_quick_sort_lifecycle_move_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
            pivot,
            &temp);
    return high_index;
}
#endif

#if !POTTERY_QUICK_SORT_USE_MOVE
static inline
size_t pottery_quick_sort_partition(
        POTTERY_QUICK_SORT_ARGS
        size_t start_index, size_t end_index)
{
    pottery_assert(end_index - start_index >= 1);

    pottery_quick_sort_entry_t pivot_entry = pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS start_index);
    pottery_quick_sort_ref_t pivot = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL pivot_entry);

    size_t low = start_index;
    size_t high = end_index + 1;
    while (true) {
        // Something to improve here later would be to eliminate some of these
        // redundant accesses. We should be able to hold on to the last access
        // to use in swap.
        // TODO we also need to use next/previous, not select each time

        // This is Hoare paritioning, except that we've kept the pivot outside
        // of the array so that we can refer to it in our comparisons.
        // Note that we can't skip elements equal to the pivot, otherwise we
        // wouldn't meet in the middle when all elements are equal.
        do {
            ++low;
        } while (low < high && pottery_quick_sort_compare_greater(
                    POTTERY_QUICK_SORT_CONTEXT_VAL pivot,
                    pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL
                        pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS low))));
        do {
            --high;
        } while (low < high && pottery_quick_sort_compare_less(
                    POTTERY_QUICK_SORT_CONTEXT_VAL pivot,
                    pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL
                        pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS high))));

        if (low >= high)
            break;

        pottery_quick_sort_lifecycle_swap_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL
                    pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS low)),
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL
                    pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS high)));
    }

    // It's possible that the above algorithm stopped on an element that was
    // never compared with anything. We compare it to the pivot to decide where
    // the pivot should go.
    if (low > end_index || pottery_quick_sort_compare_less(POTTERY_QUICK_SORT_CONTEXT_VAL
                pivot,
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL
                    pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS low))))
        --low;

    // Unlike in normal Hoare partitioning, we now know where the pivot element
    // goes, so we swap it into place and skip it in the recursion.
    if (low != start_index) {
        pottery_quick_sort_lifecycle_swap_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
                pivot,
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL
                    pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS low)));
    }

    return low;
}
#endif

#ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
// We noinline the depth fallback to keep it out of the main implementation
// since it should be rarely used.
pottery_noinline static
void pottery_quick_sort_depth_fallback(
        POTTERY_QUICK_SORT_ARGS
        size_t first, size_t count)
{
    POTTERY_QUICK_SORT_ARGS_UNUSED;

    POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK(
            POTTERY_QUICK_SORT_CONTEXT_VAL
            pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS first),
            count);
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
    #ifdef POTTERY_QUICK_SORT_COUNT_LIMIT_FALLBACK
    #ifdef POTTERY_QUICK_SORT_VALUE_TYPE
    // The limit can apparently be quite high for small elements (e.g. ints).
    // We assume large elements take more effort to compare and swap, so if we
    // know the element size, we make the limit dependent on it (within a sane
    // range) so that we avoid big numbers for big elements.
    size_t count_limit = pottery_clamp_s(64 * 4 / sizeof(pottery_quick_sort_value_t), 8, 64);
    #else
    // If we don't know the element size, we estimate conservatively.
    size_t count_limit = 8;
    #endif
    if (count <= count_limit) {
        POTTERY_QUICK_SORT_COUNT_LIMIT_FALLBACK(
                POTTERY_QUICK_SORT_CONTEXT_VAL
                pottery_quick_sort_array_access_select(POTTERY_QUICK_SORT_VALS first),
                count);
        return true;
    }
    #endif

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

        // choose a pivot, swapping it with the start index
        pottery_quick_sort_prepare_pivot(POTTERY_QUICK_SORT_VALS first, last);

        // do the partition. note that this places the pivot in its correct
        // final position so we exclude it from the resulting partitions.
        size_t pivot = pottery_quick_sort_partition(POTTERY_QUICK_SORT_VALS first, last);
        size_t left_count = pivot - first;
        size_t right_count = last - pivot;

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
        if (pivot - first < last - pivot) {
            stack[pos].first = pivot + 1;
            stack[pos].last = last;
            #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
            stack[pos].depth = depth;
            #endif

            if (left_count > 1) {
                ++pos;
                pottery_assert(pos != pottery_array_count(stack)); // not possible!

                stack[pos].first = first;
                stack[pos].last = pivot - 1;
                #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
                stack[pos].depth = depth;
                #endif
            }

        } else {
            stack[pos].first = first;
            stack[pos].last = pivot - 1;
            #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
            stack[pos].depth = depth;
            #endif

            if (right_count > 1) {
                ++pos;
                pottery_assert(pos != pottery_array_count(stack)); // not possible!

                stack[pos].first = pivot + 1;
                stack[pos].last = last;
                #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
                stack[pos].depth = depth;
                #endif
            }
        }
    }
}
