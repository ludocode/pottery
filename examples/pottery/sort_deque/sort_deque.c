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

#include <stdio.h>



// First we define our deque. We use a small per-page count to test page
// boundary behaviour.
#define POTTERY_DEQUE_PREFIX     int_deque
#define POTTERY_DEQUE_VALUE_TYPE int
#define POTTERY_DEQUE_PER_PAGE 9
#define POTTERY_DEQUE_LIFECYCLE_BY_VALUE 1
#include "pottery/deque/pottery_deque_static.t.h"


// TODO this shouldn't be necessary once we fix BY_VALUE with abstract refs
static inline void swap_ints(int* x, int* y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

// Next we define our sort algorithm. We wrap as much of our deque as possible
// so there is quite a large configuration here.
#define POTTERY_QUICK_SORT_PREFIX sort_deque
#define POTTERY_QUICK_SORT_VALUE_TYPE int

// The context for our sort algorithm is a pointer to the deque.
#define POTTERY_QUICK_SORT_CONTEXT_TYPE int_deque_t*

// The ref type of the sort algorithm is the ref type of the deque.
// TODO Lifecycle should support a REF_VALUE configuration rather than
// de-referencing refs directly, BY_VALUE should still be possible even with
// non-trivial ref type (otherwise it won't be possible to MOVE at all since we
// can't create an abstract ref to a temporary)
#define POTTERY_QUICK_SORT_REF_TYPE int_deque_ref_t
#define POTTERY_QUICK_SORT_LIFECYCLE_SWAP(deque, x, y) \
    swap_ints(int_deque_ref_value(deque, x), int_deque_ref_value(deque, y))

// The deque is itself an array container, so we pass its begin, end and count.
// (Only one of end or count are needed, but passing them both could improve
// performance.)
#define POTTERY_QUICK_SORT_ARRAY_ACCESS_BEGIN int_deque_begin
#define POTTERY_QUICK_SORT_ARRAY_ACCESS_END int_deque_end
#define POTTERY_QUICK_SORT_ARRAY_ACCESS_COUNT int_deque_count

// We pass all of the other array access expressions in order to maximize
// performance. Only SELECT and INDEX are required, but NEXT and PREVIOUS are
// particularly important because they allow the sort algorithm to iterate
// through values without going through the deque's page ring.
#define POTTERY_QUICK_SORT_ARRAY_ACCESS_SELECT int_deque_select
#define POTTERY_QUICK_SORT_ARRAY_ACCESS_INDEX int_deque_index
#define POTTERY_QUICK_SORT_ARRAY_ACCESS_NEXT int_deque_next
#define POTTERY_QUICK_SORT_ARRAY_ACCESS_PREVIOUS int_deque_previous

// Just LESS would be sufficient but we also define EQUAL in order to ensure
// that the compare template can define all functions with a minimum number of
// operations.
#define POTTERY_QUICK_SORT_COMPARE_LESS(deque, x, y) *int_deque_ref_value(deque, x) < *int_deque_ref_value(deque, y)
#define POTTERY_QUICK_SORT_COMPARE_EQUAL(deque, x, y) *int_deque_ref_value(deque, x) == *int_deque_ref_value(deque, y)

#include "pottery/quick_sort/pottery_quick_sort_static.t.h"
// TODO change above to intro_sort once heap is converted to array_access



int main(void) {
    int_deque_t deque;
    int_deque_init(&deque);

    // Insert a bunch of values into the deque
    int values[] = {
        46, 45, 32, 3, 34, 24, 9, 21, 5, 40,
        29, 18, 19, 14, 23, 38, 30, 33, 26, 50,
        16, 41, 4, 37, 48, 11, 12, 36, 25, 42,
        31, 20, 39, 1, 22, 27, 28, 10, 44, 7,
        8, 6, 13, 2, 49, 35, 15, 43, 17, 47,
    };
    size_t i;
    for (i = 0; i < sizeof(values) / sizeof(*values); ++i)
        int_deque_insert_last(&deque, values[i]);

    // Sort the deque!
    sort_deque(&deque);

    // Print its contents
    for (i = 0; i < int_deque_count(&deque); ++i) {
        printf("%i\n", *int_deque_ref_value(&deque, int_deque_at(&deque, i)));
    }

    return EXIT_SUCCESS;
}
