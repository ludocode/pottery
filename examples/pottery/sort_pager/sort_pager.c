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



// First we define our pager. We use a small per-page count to test page
// boundary behaviour.
#define POTTERY_PAGER_PREFIX int_pager
#define POTTERY_PAGER_VALUE_TYPE int
#define POTTERY_PAGER_PER_PAGE 9
#define POTTERY_PAGER_LIFECYCLE_BY_VALUE 1
#include "pottery/pager/pottery_pager_static.t.h"



// Next we define our sort algorithm. We wrap as much of our pager as possible
// so there is quite a large configuration here.

// Since we're sorting an int_pager, we'll call it sort_int_pager().
#define POTTERY_INTRO_SORT_PREFIX sort_int_pager

// The context for our sort algorithm is a pointer to the pager.
#define POTTERY_INTRO_SORT_CONTEXT_TYPE int_pager_t*

// The value type is int. It can be moved/copied by assignment and compared by
// the built-in comparison operators so we declare these to be BY_VALUE.
#define POTTERY_INTRO_SORT_VALUE_TYPE int
#define POTTERY_INTRO_SORT_LIFECYCLE_BY_VALUE 1
#define POTTERY_INTRO_SORT_COMPARE_BY_VALUE 1

// The entry type for accessing the array is the entry type of the pager. We
// need to supply the pager's function for getting a pointer to the value
// (a.k.a the ref) of an entry.
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_ENTRY_TYPE int_pager_entry_t
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_ENTRY_REF int_pager_entry_ref

// The pager is itself an array container, so we pass its begin, end and count.
// These are not necessary but they make the API nicer because now
// sort_int_pager() takes only the pager as argument. If we didn't configure
// these, we would simply pass begin and count to sort_int_pager() instead.
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_BEGIN int_pager_begin
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_END int_pager_end
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_COUNT int_pager_count

// SELECT and INDEX are required since our array is not contiguous.
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_SELECT int_pager_select
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_INDEX int_pager_index

// We also pass NEXT, PREVIOUS and EQUAL to improve performance. They allow the
// sort algorithm to iterate through values within pages without going through
// the pager's page ring.
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_EQUAL int_pager_entry_equal
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_NEXT int_pager_next
#define POTTERY_INTRO_SORT_ARRAY_ACCESS_PREVIOUS int_pager_previous

#include "pottery/intro_sort/pottery_intro_sort_static.t.h"



int main(void) {
    int_pager_t pager;
    int_pager_init(&pager);

    // Insert a bunch of values into the pager
    int values[] = {
        46, 45, 32, 3, 34, 24, 9, 21, 5, 40,
        29, 18, 19, 14, 23, 38, 30, 33, 26, 50,
        16, 41, 4, 37, 48, 11, 12, 36, 25, 42,
        31, 20, 39, 1, 22, 27, 28, 10, 44, 7,
        8, 6, 13, 2, 49, 35, 15, 43, 17, 47,
    };
    size_t i;
    for (i = 0; i < sizeof(values) / sizeof(*values); ++i)
        int_pager_insert_last(&pager, values[i]);

    // Sort the pager!
    sort_int_pager(&pager);

    // Print its contents
    for (i = 0; i < int_pager_count(&pager); ++i) {
        printf("%i\n", *int_pager_entry_ref(&pager, int_pager_at(&pager, i)));
    }

    return EXIT_SUCCESS;
}
