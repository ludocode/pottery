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

/*
 * Tests a sort function called sort_ints on an array of ints.
 *
 * This is used to test various sort implementations.
 */

#ifndef POTTERY_TEST_SORT_INT_PREFIX
#error "Misconfigured template"
#include "pottery_fatal"
#endif

#define POTTERY_TEST_SORT_INT_NAME(x) \
    POTTERY_CONCAT(POTTERY_CONCAT(POTTERY_TEST_SORT_INT_PREFIX, _), x)

#define POTTERY_TEST_SORT_INT(x) \
    POTTERY_TEST(POTTERY_TEST_SORT_INT_NAME(x))

#define test_sorted_ints POTTERY_TEST_SORT_INT_NAME(test_sorted_ints)

// Verify that all ints are in ascending order
static void test_sorted_ints(int* ints, size_t count) {
    size_t i;
    for (i = 1; i < count; ++i) {
        pottery_test_assert(ints[i] >= ints[i - 1]);
    }
}

POTTERY_TEST_SORT_INT(degenerate) {
    // Sorting an empty array should work with an arbitrary pointer. It
    // shouldn't be accessed.
    sort_ints(pottery_null, 0);
    sort_ints(pottery_reinterpret_cast(int*, -1), 0);

    // Single element
    int x = 5;
    sort_ints(&x, 1);
    pottery_test_assert(x == 5);

    // Two elements already in order
    int two_ints[] = {
        1, 2
    };
    sort_ints(two_ints, pottery_array_count(two_ints));
    test_sorted_ints(two_ints, pottery_array_count(two_ints));
}

POTTERY_TEST_SORT_INT(small) {
    int ints[] = {
        2, 1, 7, 3, 4, 8, 8, 9, 5,
    };
    sort_ints(ints, pottery_array_count(ints));
    test_sorted_ints(ints, pottery_array_count(ints));
}

POTTERY_TEST_SORT_INT(big) {
    // Generate a big array of ints by permuting half the array size by a big
    // prime. Each number from 0 to 999 will end up in the list twice.
    int ints[2000];
    int prime = 337;
    int x = prime;
    size_t i;
    for (i = 0; i < pottery_array_count(ints); ++i) {
        ints[i] = x;
        x = (x + prime) % 1000;
    }
    sort_ints(ints, pottery_array_count(ints));
    test_sorted_ints(ints, pottery_array_count(ints));
}
