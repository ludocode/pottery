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

#include <stdio.h>



// First we define our deque. We use a small per-page count to test page
// boundary behaviour.
#define POTTERY_DEQUE_PREFIX int_deque
#define POTTERY_DEQUE_VALUE_TYPE int
#define POTTERY_DEQUE_PER_PAGE 9
#define POTTERY_DEQUE_LIFECYCLE_BY_VALUE 1
#include "pottery/deque/pottery_deque_static.t.h"



// Next we define our sort algorithm. We wrap as much of our deque as possible
// so there is quite a large configuration here.

// Since we're sorting an int_deque, we'll call it sort_int_deque().
#define POTTERY_INTRO_SORT_PREFIX sort_int_deque

// The context for our sort algorithm is a pointer to the deque.
#define POTTERY_INTRO_SORT_CONTEXT_TYPE int_deque_t*

// The entry type of the sort algorithm is the entry type of the deque. We need
// to supply the deque's function for getting the value (ref) of an entry so
// that the sort algorithm can do by-value lifecycle and compare operations on
// deque values.
#define POTTERY_INTRO_SORT_VALUE_TYPE int
#define POTTERY_INTRO_SORT_ENTRY_TYPE int_deque_entry_t
#define POTTERY_INTRO_SORT_ENTRY_REF int_deque_entry_value
#define POTTERY_INTRO_SORT_LIFECYCLE_BY_VALUE 1
#define POTTERY_INTRO_SORT_COMPARE_BY_VALUE 1

// The deque is itself an array container, so we pass its begin, end and count.
// These are not necessary but they make the API nicer because now
// sort_int_deque() takes only the deque as argument. If we didn't configure
// these, we would simply pass begin and count to sort_int_deque() instead.
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_BEGIN int_deque_begin
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_END int_deque_end
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_COUNT int_deque_count

// SELECT and INDEX are required since our array is not contiguous.
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_SELECT int_deque_select
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_INDEX int_deque_index

// We also pass NEXT, PREVIOUS and EQUAL to improve performance. They allow the
// sort algorithm to iterate through values within pages without going through
// the deque's page ring.
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_EQUAL int_deque_entry_equal
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_NEXT int_deque_next
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_PREVIOUS int_deque_previous

#include "pottery/intro_sort/pottery_intro_sort_static.t.h"



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
    sort_int_deque(&deque);

    // Print its contents
    for (i = 0; i < int_deque_count(&deque); ++i) {
        printf("%i\n", *int_deque_entry_value(&deque, int_deque_at(&deque, i)));
    }

    return EXIT_SUCCESS;
}
