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

/*
 * Tests a sort function called sort_ints on an array of ints.
 *
 * This is used to test various sort implementations.
 */

#ifndef POTTERY_TEST_SORT_INT_PREFIX
#error "Misconfigured template"
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

POTTERY_TEST_SORT_INT(equal) {
    int ints[1000];
    size_t i;
    for (i = 0; i < pottery_array_count(ints); ++i)
        ints[i] = 42;
    sort_ints(ints, pottery_array_count(ints));
    test_sorted_ints(ints, pottery_array_count(ints));
}

POTTERY_TEST_SORT_INT(mostly_equal) {
    int ints[1000];
    size_t i;
    for (i = 0; i < pottery_array_count(ints); ++i)
        ints[i] = (i % 31 == 13) ? pottery_cast(int, 1000 - i - 1) : 42;
    sort_ints(ints, pottery_array_count(ints));
    test_sorted_ints(ints, pottery_array_count(ints));
}

POTTERY_TEST_SORT_INT(pre_sorted) {
    int ints[1000];
    size_t i;
    for (i = 0; i < pottery_array_count(ints); ++i)
        ints[i] = pottery_cast(int, i);
    sort_ints(ints, pottery_array_count(ints));
    test_sorted_ints(ints, pottery_array_count(ints));
}

POTTERY_TEST_SORT_INT(mostly_pre_sorted) {
    int ints[1000];
    size_t i;
    for (i = 0; i < pottery_array_count(ints); ++i)
        ints[i] = (i % 31 == 13) ? 42 : pottery_cast(int, 1000 - i - 1);
    sort_ints(ints, pottery_array_count(ints));
    test_sorted_ints(ints, pottery_array_count(ints));
}

POTTERY_TEST_SORT_INT(reverse_sorted) {
    int ints[1000];
    size_t i;
    for (i = 0; i < pottery_array_count(ints); ++i)
        ints[i] = pottery_cast(int, 1000 - i - 1);
    sort_ints(ints, pottery_array_count(ints));
    test_sorted_ints(ints, pottery_array_count(ints));
}

static inline void swap_ints(int* i, int* j) {
    int temp = *i;
    *i = *j;
    *j = temp;
}

#define PRINT_PERMUTATIONS 0

// Sorts a copy of the given array and tests that the sort is correct, leaving
// the original array intact.
static inline void sort_copy(const int* ints, size_t n) {
    #if PRINT_PERMUTATIONS
    size_t i;
    printf("Sorting ");
    for (i = 0; i < n; ++i)
        printf("%i ", ints[i]);
    printf("\n");
    #endif

    // We only support up to this size, otherwise we fail the test, this way we
    // don't have to allocate anything.
    int copy[16];
    pottery_test_assert(n <= pottery_array_count(copy));
    memcpy(copy, ints, sizeof(int) * n);
    sort_ints(copy, n);
    test_sorted_ints(copy, n);
}

// Generates all permutations of the given int array and sorts each one,
// verifying that the sort is correct.
static inline void sort_permutations(int* ints, size_t n) {
    // https://en.wikipedia.org/wiki/Permutation#Generation_in_lexicographic_order

    while (true) {
        sort_copy(ints, n);

        // Find the largest index k such that a[k] < a[k + 1].
        // If no such index exists, the permutation is the last permutation.
        size_t k = n;
        while (k > 1 && ints[k - 2] >= ints[k - 1])
            --k;
        if (k == 1)
            break;
        k -= 2;

        // Find the largest index l greater than k such that a[k] < a[l].
        size_t l = n - 1;
        while (ints[k] >= ints[l])
            --l;

        // Swap the value of a[k] with that of a[l].
        swap_ints(ints + k, ints + l);

        // Reverse the sequence from a[k + 1] up to and including the final element a[n].
        size_t i;
        for (i = 0; i < (n - k - 1) / 2; ++i) {
            swap_ints(ints + k + i + 1, ints + n - i - 1);
        }
    }
}

// Generates all possible permutations of all possible combinations of numbers
// up to a given size.
//
// This is a pretty exhaustive search of corner cases for sorting algorithms.
// Turn on PRINT_PERMUTATIONS to see them all or to debug which one is failing.
POTTERY_TEST_SORT_INT(permutations) {
    int ints[8];

    // number of elements
    size_t n;
    for (n = 1; n <= pottery_array_count(ints); ++n) {
        #if PRINT_PERMUTATIONS
        printf("%zi elements\n", n);
        #endif

        // bits that say whether we increment after adding an element
        // (generates all possible combinations of duplicates)
        size_t max_bits = (n > 1) ? 1 << (n - 1) : 1;
        size_t bits;
        for (bits = 0; bits < max_bits; ++bits) {
            int v = 1;
            size_t i;
            for (i = 0; i < n; ++i) {
                ints[i] = v;
                if (bits & (1 << i))
                    ++v;
            }

            #if PRINT_PERMUTATIONS
            printf("Increment bits %zx\n", bits);
            printf("Generating permutations of ");
            for (i = 0; i < n; ++i)
                printf("%i ", ints[i]);
            printf("\n");
            #endif

            sort_permutations(ints, i);
        }
    }
}
