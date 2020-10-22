/*
 * MIT License
 *
 * Copyright (c) 2020 Nicholas Fraser
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
 * Helpers to wrap configuration
 */

typedef struct pottery_quick_sort_state_t {
    #ifdef POTTERY_QUICK_SORT_CONTEXT_TYPE
    pottery_quick_sort_context_t context;
    #endif
    pottery_quick_sort_ref_t first;
} pottery_quick_sort_state_t;

// Gets the ref for the element at index.
static pottery_always_inline
pottery_quick_sort_ref_t pottery_quick_sort_access(pottery_quick_sort_state_t state, size_t index) {
    (void)state;

    #ifndef POTTERY_QUICK_SORT_ACCESS
        // With no defined access expression, it's a simple array.
        return state.first + index;
    #else
        return POTTERY_QUICK_SORT_ACCESS(POTTERY_QUICK_SORT_CONTEXT_VAL(state) state.first, index);
    #endif
}

/*
 * Implementation
 */

// The below implementation uses the Hoare partitioning method, except that it
// keeps the pivot at a fixed position outside of the loop during partitioning,
// that way we can compare it by reference without having to copy it to a
// temporary or track its position changes.

// Chooses a pivot and puts it at the start of the array, returning its index.
static
void pottery_quicksort_prepare_pivot(pottery_state_t state, size_t start_index, size_t end_index) {
    if (end_index - start_index < 2) {
        // there are less than three elements, so it doesn't matter which one
        // is the pivot.
        return;
    }

    // choose elements for the median
    // (slightly awkward average here to avoid overflow
    size_t middle_index = start_index + (end_index - start_index) / 2;
    pottery_quick_sort_ref_t start = pottery_access(state, start_index);
    pottery_quick_sort_ref_t middle = pottery_access(state, middle_index);
    pottery_quick_sort_ref_t end = pottery_access(state, end_index);

    // if there are at most some arbitrary threshold of elements, use median
    if (end_index - start_index + 1 <= 128) {
        pottery_quick_sort_ref_t median = pottery_quick_sort_compare_median(
                POTTERY_QUICK_SORT_CONTEXT_VAL(state) start, middle, end);
        if (median != start)
            pottery_quick_sort_lifecycle_swap(POTTERY_QUICK_SORT_CONTEXT_VAL(state) start, median);
        return;
    }

    // use ninther
    size_t third_offset = (end_index - start_index) / 3;
    size_t sixth_offset = (end_index - start_index) / 6;
    pottery_quick_sort_ref_t left_middle = pottery_access(state, start_index + sixth_offset);
    pottery_quick_sort_ref_t left_end = pottery_access(state, start_index + third_offset);
    pottery_quick_sort_ref_t right_start = pottery_access(state, middle_index + third_offset);
    pottery_quick_sort_ref_t right_middle = pottery_access(state, middle_index + third_offset + sixth_offset);

    pottery_quick_sort_ref_t median = pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL(state)
            pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL(state) start, left_middle, left_end),
            pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL(state) left_end, middle, right_start),
            pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL(state) right_start, right_middle, end));

    //if (median != pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL(state) start, middle, end))
    //    printf("found better median %i instead of %i\n", *median, *pottery_quick_sort_compare_median(POTTERY_QUICK_SORT_CONTEXT_VAL(state) start, middle, end));

    if (median != start)
        pottery_quick_sort_lifecycle_swap(POTTERY_QUICK_SORT_CONTEXT_VAL(state) start, median);
}

static
size_t pottery_quicksort_partition(pottery_state_t state, size_t start_index, size_t end_index) {
    #if 0
    printf("starting partition at %zi-%zi:", start_index, end_index);
    for (size_t i = start_index; i != end_index + 1; ++i)
        printf(" %i", *pottery_access(state, i));
    printf("\n");
    #endif

    pottery_assert(end_index - start_index >= 1);

    pottery_quicksort_prepare_pivot(state, start_index, end_index);
    pottery_quick_sort_ref_t pivot = pottery_access(state, start_index);

    #if 0
    printf("chose pivot %i at %zi\n", *pivot, start_index);
    #endif

    size_t low = start_index;
    size_t high = end_index + 1;
    while (true) {
        // Something to improve here later would be to eliminate some of these
        // redundant accesses. We should be able to hold on to the last access
        // to use in swap.

        // This is Hoare paritioning, except that we've kept the pivot outside
        // of the array so that we can refer to it in our comparisons.
        // Note that we can't skip elements equal to the pivot, otherwise we
        // wouldn't meet in the middle when all elements are equal.
        do {
            ++low;
        } while (low < high && pottery_quick_sort_compare_less(
                    POTTERY_QUICK_SORT_CONTEXT_VAL(state) pottery_access(state, low), pivot));
        do {
            --high;
        } while (low < high && pottery_quick_sort_compare_less(
                    POTTERY_QUICK_SORT_CONTEXT_VAL(state) pivot, pottery_access(state, high)));

        if (low >= high)
            break;

        #if 0
        printf("swapping index %zi (%i) with index %zi (%i)\n", low,
                *pottery_access(state, low), high, *pottery_access(state, high));
        #endif

        pottery_quick_sort_lifecycle_swap(POTTERY_QUICK_SORT_CONTEXT_VAL(state) pottery_access(state, low), pottery_access(state, high));
    }

    // It's possible that the above algorithm stopped on an element that was
    // never compared with anything. We compare it to the pivot to decide where
    // the pivot should go.
    #if 0
    printf("finished loop, resulting low: %zi\n", low);
    #endif
    if (low > end_index || pottery_quick_sort_compare_less(POTTERY_QUICK_SORT_CONTEXT_VAL(state) pivot, pottery_access(state, low)))
        --low;

    // Unlike in normal Hoare partitioning, we now know where the pivot element
    // goes, so we swap it into place and skip it in the recursion.
    if (low != start_index) {
        #if 0
        printf("swapping pivot %zi with %zi\n", start_index, high);
        #endif
        pottery_quick_sort_lifecycle_swap(POTTERY_QUICK_SORT_CONTEXT_VAL(state) pivot, pottery_access(state, low));
    }

    #if 0
    printf("resulting partition:");
    for (size_t i = start_index; i != end_index + 1; ++i)
        printf(" %i", *pottery_access(state, i));
    printf("\n");
    #endif

    return low;
}

static
void pottery_quicksort_impl(pottery_state_t state, size_t start, size_t end, size_t depth_limit) {
    pottery_assert(end > start);
    size_t count = end - start + 1;
    (void)count;

    // Switch to the count limit fallback if we don't have enough elements
    #ifdef POTTERY_QUICK_SORT_COUNT_LIMIT_FALLBACK

    #ifdef POTTERY_QUICK_SORT_VALUE_TYPE
    // The limit can apparently be quite high for small elements (e.g. ints).
    // We assume large elements take more effort to compare and swap, so if we
    // know the element size, we make the limit dependent on it (within a sane
    // range) so that we avoid big numbers for big elements.
    size_t count_limit = pottery_clamp_s(4 * 64 / sizeof(pottery_quick_sort_value_t), 8, 64);
    #else
    // If we don't know the element size, we estimate conservatively.
    size_t count_limit = 8;
    #endif

    if (count <= count_limit) {
        POTTERY_QUICK_SORT_COUNT_LIMIT_FALLBACK(
                POTTERY_QUICK_SORT_CONTEXT_VAL(state)
                pottery_quick_sort_access(state, start),
                count
                );
        return;
    }
    #endif

    // Switch to the depth limit fallback if we're too deep
    #ifdef POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK
    if (depth_limit == 0) {
        POTTERY_QUICK_SORT_DEPTH_LIMIT_FALLBACK(
                POTTERY_QUICK_SORT_CONTEXT_VAL(state)
                pottery_quick_sort_access(state, start),
                count
                );
        return;
    }
    #endif

    size_t pivot = pottery_quicksort_partition(state, start, end);

    // Note that we are using a variant of the Hoare partitioning scheme where
    // we place the pivot in its correct position afterwards, so we can exclude
    // it from recursion.

    --depth_limit;

    if (start + 1 < pivot)
        pottery_quicksort_impl(state, start, pivot - 1, depth_limit);
    if (pivot + 1 < end)
        pottery_quicksort_impl(state, pivot + 1, end, depth_limit);
}

/*
 * External functions
 */

POTTERY_QUICK_SORT_EXTERN
void pottery_quick_sort(
        #ifdef POTTERY_QUICK_SORT_CONTEXT_TYPE
        pottery_quick_sort_context_t context,
        #endif
        pottery_quick_sort_ref_t first,
        size_t count
) {
    pottery_quick_sort_state_t state = {
        #ifdef POTTERY_QUICK_SORT_CONTEXT_TYPE
        context,
        #endif
        first,
    };

    if (count <= 1)
        return;

    size_t n = 1;
    size_t depth_limit = 2;
    while (n < count) {
        n *= 2;
        depth_limit += 2;
    }

    pottery_quicksort_impl(state, 0, count - 1, depth_limit);

    #if 0
    printf("done! result:");
    for (size_t i = 0; i < count; ++i)
        printf(" %i", *pottery_access(state, i));
    printf("\n");
    #endif
}
