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

#include "pottery/pottery_dependencies.h"

// We define an int pager as separate declare/define headers to make sure it
// works properly. We also give it a small page size to help test paging and
// bulk methods.
#define POTTERY_PAGER_PREFIX     int_pager
#define POTTERY_PAGER_VALUE_TYPE int
#define POTTERY_PAGER_PER_PAGE 37
#define POTTERY_PAGER_LIFECYCLE_BY_VALUE 1
#include "pottery/pager/pottery_pager_declare.t.h"
#define POTTERY_PAGER_PREFIX     int_pager
#define POTTERY_PAGER_VALUE_TYPE int
#define POTTERY_PAGER_PER_PAGE 37
#define POTTERY_PAGER_LIFECYCLE_BY_VALUE 1
#include "pottery/pager/pottery_pager_define.t.h"

#include "pottery/unit/test_pottery_framework.h"

POTTERY_TEST(pottery_pager_int_init_destroy) {
    int_pager_t pager;
    int_pager_init(&pager);
    pottery_test_assert(int_pager_count(&pager) == 0);
    int_pager_destroy(&pager);
}

// TODO most of this file appears to be copy-pasted from array. need better
// tests

static void test_pottery_pager_int_normal(int count) {
    int_pager_t pager;
    int_pager_init(&pager);

    int i;
    for (i = 0; i < count; ++i) {
        int_pager_insert_last(&pager, i);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(*int_pager_entry_ref(&pager,
                    int_pager_at(&pager, pottery_cast(size_t, i))) == i);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(i == int_pager_extract_first(&pager));
    }

    int_pager_destroy(&pager);
}

POTTERY_TEST(pottery_pager_int_normal_many) {
    // enough to span many pages
    test_pottery_pager_int_normal(5000);
}

POTTERY_TEST(pottery_pager_int_normal_some) {
    // less than a page worth
    test_pottery_pager_int_normal(50);
}

static void test_pottery_pager_int_reverse(int count) {
    int_pager_t pager;
    int_pager_init(&pager);

    int i;
    for (i = 0; i < count; ++i) {
        int_pager_insert_first(&pager, i);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(*int_pager_entry_ref(&pager,
                    int_pager_at(&pager, pottery_cast(size_t, i))) == count - i - 1);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(i == int_pager_extract_last(&pager));
    }

    int_pager_destroy(&pager);
}

POTTERY_TEST(pottery_pager_int_reverse_many) {
    // enough to span many pages
    test_pottery_pager_int_reverse(5000);
}

POTTERY_TEST(pottery_pager_int_reverse_some) {
    // less than a page worth
    test_pottery_pager_int_reverse(50);
}

static const int bulk_count = 200;

POTTERY_TEST(pottery_pager_bulk_insert) {
    int_pager_t pager;
    int_pager_init(&pager);

    int block[10000];

    int count, i;

    for (count = 1; count < bulk_count; ++count) {
        for (i = 0; i < count; ++i)
            block[i] = i;
        int_pager_insert_last_bulk(&pager, block, pottery_cast(size_t, count));

        //fprintf(stderr, "inserted bulk, now contains:\n");
        //for (i = 0; i < (int)int_pager_count(&pager); ++i)
            //fprintf(stderr, "%i ", *int_pager_entry_ref(&pager, int_pager_at(&pager, pottery_cast(size_t)i)));
        //fprintf(stderr, "\n");
    }

    for (count = 1; count < bulk_count; ++count)
        for (i = 0; i < count; ++i)
            pottery_test_assert(i == int_pager_extract_first(&pager));

    int_pager_destroy(&pager);
}

POTTERY_TEST(pottery_pager_bulk_insert_first) {
    int_pager_t pager;
    int_pager_init(&pager);

    int block[10000];

    int count, i;

    for (count = 1; count < bulk_count; ++count) {
        for (i = 0; i < count; ++i)
            block[i] = i;
        int_pager_insert_first_bulk(&pager, block, pottery_cast(size_t, count));
    }

    // Each block is in ascending order, but we've inserted the blocks in reverse
    // order.
    for (count = bulk_count - 1; count > 0; --count)
        for (i = 0; i < count; ++i)
            pottery_test_assert(i == int_pager_extract_first(&pager));

    int_pager_destroy(&pager);
}

POTTERY_TEST(pottery_pager_bulk_extract_first) {
    int_pager_t pager;
    int_pager_init(&pager);

    int block[10000];

    int count, i;

    for (count = 1; count < bulk_count; ++count)
        for (i = 0; i < count; ++i)
            int_pager_insert_last(&pager, i);

    for (count = 1; count < bulk_count; ++count) {
        int_pager_extract_first_bulk(&pager, block, pottery_cast(size_t, count));
        for (i = 0; i < count; ++i)
            pottery_test_assert(i == block[i]);
    }

    int_pager_destroy(&pager);
}

POTTERY_TEST(pottery_pager_bulk_extract_last) {
    int_pager_t pager;
    int_pager_init(&pager);

    int block[10000];

    int count, i;

    for (count = 1; count < bulk_count; ++count)
        for (i = 0; i < count; ++i)
            int_pager_insert_last(&pager, i);

    // Each block is in ascending order, but we're extracting the blocks in
    // reverse order.
    for (count = bulk_count - 1; count > 0; --count) {
        int_pager_extract_last_bulk(&pager, block, pottery_cast(size_t, count));
        for (i = 0; i < count; ++i)
            pottery_test_assert(i == block[i]);
    }

    int_pager_destroy(&pager);
}

// This should be moved to test_pottery_unit_array_ufo but pager doesn't use it
// yet! See for_each in test_pottery_unit_map_ufo for the map equivalent
#if POTTERY_HAS_FULL_FOR_EACH
POTTERY_TEST(pottery_pager_for_each) {
    int_pager_t pager;
    int_pager_init(&pager);

    for (int i = 0; i < 32; ++i) {

        // test that POTTERY_FOR_EACH() visits all elements in the correct order
        int count = 0;
        int* ref;
        POTTERY_FOR_EACH(ref, int_pager, &pager) {
            pottery_test_assert(*ref == count);
            ++count;
        }

        // test that we visited all elements
        pottery_test_assert(count == i);
        pottery_test_assert(count == pottery_cast(int, int_pager_count(&pager)));

        // add an element
        int_pager_insert_last(&pager, i);
    }

    int_pager_destroy(&pager);
}
#endif
