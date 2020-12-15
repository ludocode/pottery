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

#include "pottery/pottery_dependencies.h"

// We define an int deque as separate declare/define headers to make sure it
// works properly. We also give it a small page size to help test paging and
// bulk methods.
#define POTTERY_DEQUE_PREFIX     int_deque
#define POTTERY_DEQUE_VALUE_TYPE int
#define POTTERY_DEQUE_PER_PAGE 37
#define POTTERY_DEQUE_LIFECYCLE_BY_VALUE 1
#include "pottery/deque/pottery_deque_declare.t.h"
#define POTTERY_DEQUE_PREFIX     int_deque
#define POTTERY_DEQUE_VALUE_TYPE int
#define POTTERY_DEQUE_PER_PAGE 37
#define POTTERY_DEQUE_LIFECYCLE_BY_VALUE 1
#include "pottery/deque/pottery_deque_define.t.h"

#include "pottery/unit/test_pottery_framework.h"

POTTERY_TEST(pottery_deque_int_init_destroy) {
    int_deque_t deque;
    int_deque_init(&deque);
    pottery_test_assert(int_deque_count(&deque) == 0);
    int_deque_destroy(&deque);
}

// TODO most of this file appears to be copy-pasted from array. need better
// tests

static void test_pottery_deque_int_normal(int count) {
    int_deque_t deque;
    int_deque_init(&deque);

    int i;
    for (i = 0; i < count; ++i) {
        int_deque_insert_last(&deque, i);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(*int_deque_entry_value(&deque,
                    int_deque_at(&deque, pottery_cast(size_t, i))) == i);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(i == int_deque_extract_first(&deque));
    }

    int_deque_destroy(&deque);
}

POTTERY_TEST(pottery_deque_int_normal_many) {
    // enough to span many pages
    test_pottery_deque_int_normal(5000);
}

POTTERY_TEST(pottery_deque_int_normal_some) {
    // less than a page worth
    test_pottery_deque_int_normal(50);
}

static void test_pottery_deque_int_reverse(int count) {
    int_deque_t deque;
    int_deque_init(&deque);

    int i;
    for (i = 0; i < count; ++i) {
        int_deque_insert_first(&deque, i);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(*int_deque_entry_value(&deque,
                    int_deque_at(&deque, pottery_cast(size_t, i))) == count - i - 1);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(i == int_deque_extract_last(&deque));
    }

    int_deque_destroy(&deque);
}

POTTERY_TEST(pottery_deque_int_reverse_many) {
    // enough to span many pages
    test_pottery_deque_int_reverse(5000);
}

POTTERY_TEST(pottery_deque_int_reverse_some) {
    // less than a page worth
    test_pottery_deque_int_reverse(50);
}

static const int bulk_count = 200;

POTTERY_TEST(pottery_deque_bulk_insert) {
    int_deque_t deque;
    int_deque_init(&deque);

    int block[10000];

    int count, i;

    for (count = 1; count < bulk_count; ++count) {
        for (i = 0; i < count; ++i)
            block[i] = i;
        int_deque_insert_last_bulk(&deque, block, pottery_cast(size_t, count));

        //fprintf(stderr, "inserted bulk, now contains:\n");
        //for (i = 0; i < (int)int_deque_count(&deque); ++i)
            //fprintf(stderr, "%i ", *int_deque_entry_value(&deque, int_deque_at(&deque, pottery_cast(size_t)i)));
        //fprintf(stderr, "\n");
    }

    for (count = 1; count < bulk_count; ++count)
        for (i = 0; i < count; ++i)
            pottery_test_assert(i == int_deque_extract_first(&deque));

    int_deque_destroy(&deque);
}

POTTERY_TEST(pottery_deque_bulk_insert_first) {
    int_deque_t deque;
    int_deque_init(&deque);

    int block[10000];

    int count, i;

    for (count = 1; count < bulk_count; ++count) {
        for (i = 0; i < count; ++i)
            block[i] = i;
        int_deque_insert_first_bulk(&deque, block, pottery_cast(size_t, count));
    }

    // Each block is in ascending order, but we've inserted the blocks in reverse
    // order.
    for (count = bulk_count - 1; count > 0; --count)
        for (i = 0; i < count; ++i)
            pottery_test_assert(i == int_deque_extract_first(&deque));

    int_deque_destroy(&deque);
}

POTTERY_TEST(pottery_deque_bulk_extract_first) {
    int_deque_t deque;
    int_deque_init(&deque);

    int block[10000];

    int count, i;

    for (count = 1; count < bulk_count; ++count)
        for (i = 0; i < count; ++i)
            int_deque_insert_last(&deque, i);

    for (count = 1; count < bulk_count; ++count) {
        int_deque_extract_first_bulk(&deque, block, pottery_cast(size_t, count));
        for (i = 0; i < count; ++i)
            pottery_test_assert(i == block[i]);
    }

    int_deque_destroy(&deque);
}

POTTERY_TEST(pottery_deque_bulk_extract_last) {
    int_deque_t deque;
    int_deque_init(&deque);

    int block[10000];

    int count, i;

    for (count = 1; count < bulk_count; ++count)
        for (i = 0; i < count; ++i)
            int_deque_insert_last(&deque, i);

    // Each block is in ascending order, but we're extracting the blocks in
    // reverse order.
    for (count = bulk_count - 1; count > 0; --count) {
        int_deque_extract_last_bulk(&deque, block, pottery_cast(size_t, count));
        for (i = 0; i < count; ++i)
            pottery_test_assert(i == block[i]);
    }

    int_deque_destroy(&deque);
}
