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
 * Tests an array of int called int_array.
 *
 * This is used to test various array implementations.
 */

#ifndef POTTERY_TEST_ARRAY_INT_PREFIX
#error "Misconfigured template"
#include "pottery_fatal"
#endif

#define POTTERY_TEST_ARRAY_INT_NAME(x) \
    POTTERY_CONCAT(POTTERY_CONCAT(POTTERY_TEST_ARRAY_INT_PREFIX, _), x)

#define POTTERY_TEST_ARRAY_INT(x) \
    POTTERY_TEST(POTTERY_TEST_ARRAY_INT_NAME(x))

POTTERY_TEST_ARRAY_INT(init_destroy) {
    int_array_t array;
    int_array_init(&array);
    pottery_test_assert(int_array_count(&array) == 0);
    int_array_destroy(&array);
}

POTTERY_TEST_ARRAY_INT(single_element) {
    int_array_t array;
    int_array_init(&array);

    int_array_insert_last(&array, 5);
    pottery_test_assert(int_array_count(&array) == 1);

    pottery_test_assert(*int_array_at(&array, 0) == 5);

    int_array_remove_at(&array, 0);
    pottery_test_assert(int_array_count(&array) == 0);

    int_array_destroy(&array);
}

POTTERY_TEST_ARRAY_INT(insert) {
    int_array_t array;
    int_array_init(&array);

    int_array_insert_at(&array, 0, 5);
    pottery_test_assert(int_array_count(&array) == 1);
    pottery_test_assert(*int_array_at(&array, 0) == 5);

    int_array_insert_first(&array, 12);
    pottery_test_assert(int_array_count(&array) == 2);
    pottery_test_assert(*int_array_at(&array, 0) == 12);
    pottery_test_assert(*int_array_at(&array, 1) == 5);

    int_array_insert_at(&array, 1, 7);
    pottery_test_assert(int_array_count(&array) == 3);
    pottery_test_assert(*int_array_at(&array, 0) == 12);
    pottery_test_assert(*int_array_at(&array, 1) == 7);
    pottery_test_assert(*int_array_at(&array, 2) == 5);

    int_array_insert_last(&array, 9);
    pottery_test_assert(int_array_count(&array) == 4);
    pottery_test_assert(*int_array_at(&array, 0) == 12);
    pottery_test_assert(*int_array_at(&array, 1) == 7);
    pottery_test_assert(*int_array_at(&array, 2) == 5);
    pottery_test_assert(*int_array_at(&array, 3) == 9);

    int_array_insert_at(&array, 2, -5);
    pottery_test_assert(int_array_count(&array) == 5);
    pottery_test_assert(*int_array_at(&array, 0) == 12);
    pottery_test_assert(*int_array_at(&array, 1) == 7);
    pottery_test_assert(*int_array_at(&array, 2) == -5);
    pottery_test_assert(*int_array_at(&array, 3) == 5);
    pottery_test_assert(*int_array_at(&array, 4) == 9);

    int_array_destroy(&array);
}

POTTERY_TEST_ARRAY_INT(extract) {
    int_array_t array;
    int_array_init(&array);

    int_array_insert_last(&array, 1);
    int_array_insert_last(&array, 2);
    int_array_insert_last(&array, 3);

    pottery_test_assert(2 == int_array_extract_at(&array, 1));
    pottery_test_assert(1 == int_array_extract_first(&array));
    pottery_test_assert(3 == int_array_extract_last(&array));

    int_array_destroy(&array);
}

POTTERY_TEST_ARRAY_INT(many_elements) {
    int_array_t array;
    int_array_init(&array);

    int i;
    for (i = 0; i < 1000; ++i) {
        int_array_insert_last(&array, i);
    }

    for (i = 0; i < 1000; ++i) {
        pottery_test_assert(*int_array_at(&array, (size_t)i) == i);
    }

    for (i = 0; i < 1000; ++i) {
        int_array_remove_at(&array, 0);
    }

    int_array_destroy(&array);
}

POTTERY_TEST_ARRAY_INT(bulk) {
    int_array_entry_t entry;
    int_array_t array;
    int_array_init(&array);

    pottery_test_assert(POTTERY_OK == int_array_emplace_at_bulk(&array, 0, 100, &entry));
    size_t i;
    for (i = 0; i < 100; ++i)
        *int_array_at(&array, i) = 7;

    pottery_test_assert(POTTERY_OK == int_array_emplace_at_bulk(&array, 50, 100, &entry));
    for (i = 50; i < 150; ++i)
        *int_array_at(&array, i) = 9;

    for (i = 0; i < 50; ++i)
        pottery_test_assert(*int_array_at(&array, i) == 7);
    for (i = 50; i < 150; ++i)
        pottery_test_assert(*int_array_at(&array, i) == 9);
    for (i = 150; i < 200; ++i)
        pottery_test_assert(*int_array_at(&array, i) == 7);

    int_array_remove_at_bulk(&array, 75, 100);

    for (i = 0; i < 50; ++i)
        pottery_test_assert(*int_array_at(&array, i) == 7);
    for (i = 50; i < 75; ++i)
        pottery_test_assert(*int_array_at(&array, i) == 9);
    for (i = 75; i < 100; ++i)
        pottery_test_assert(*int_array_at(&array, i) == 7);

    int_array_remove_at_bulk(&array, 0, 100);
    int_array_destroy(&array);
}

POTTERY_TEST_ARRAY_INT(remove) {
    int_array_t array;
    int_array_init(&array);

    int i;
    for (i = 0; i < 9; ++i)
        int_array_insert_last(&array, i);
    pottery_test_assert(int_array_count(&array) == 9);

    int_array_remove_at_bulk(&array, 2, 3);

    pottery_test_assert(int_array_count(&array) == 6);
    pottery_test_assert(*int_array_at(&array, 0) == 0);
    pottery_test_assert(*int_array_at(&array, 1) == 1);
    pottery_test_assert(*int_array_at(&array, 2) == 5);
    pottery_test_assert(*int_array_at(&array, 3) == 6);
    pottery_test_assert(*int_array_at(&array, 4) == 7);
    pottery_test_assert(*int_array_at(&array, 5) == 8);

    int_array_destroy(&array);
}

POTTERY_TEST_ARRAY_INT(insert_grow_move) {
    int_array_t array;
    int_array_init(&array);

    int_array_insert_last(&array, 1);
    int_array_insert_last(&array, 2);
    int_array_insert_last(&array, 3);
    int_array_insert_last(&array, 4);

    // make sure the array will grow at least once
    pottery_test_assert(int_array_capacity(&array) < 1000);

    size_t i;
    for (i = 0; i < 1000; ++i) {

        // insert an element. sometimes this will grow, other times it will
        // move subsequent elements in place
        int_array_insert_at(&array, 2, -1);

        // make sure the ends were moved correctly
        pottery_test_assert(*int_array_at(&array, 0) == 1);
        pottery_test_assert(*int_array_at(&array, 1) == 2);
        pottery_test_assert(*int_array_at(&array, int_array_count(&array) - 2) == 3);
        pottery_test_assert(*int_array_at(&array, int_array_count(&array) - 1) == 4);
    }

    int_array_destroy(&array);
}

POTTERY_TEST_ARRAY_INT(remove_shrink_move) {
    int_array_t array;
    int_array_init(&array);

    int_array_insert_last(&array, 1);
    int_array_insert_last(&array, 2);
    size_t i;
    for (i = 0; i < 1000; ++i)
        int_array_insert_last(&array, -1);
    int_array_insert_last(&array, 3);
    int_array_insert_last(&array, 4);

    while (int_array_count(&array) > 4) {

        // remove an element. sometimes this will shrink, other times it will
        // move subsequent elements in place
        int_array_remove_at(&array, 2);

        // make sure the ends were moved correctly
        pottery_test_assert(*int_array_at(&array, 0) == 1);
        pottery_test_assert(*int_array_at(&array, 1) == 2);
        pottery_test_assert(*int_array_at(&array, int_array_count(&array) - 2) == 3);
        pottery_test_assert(*int_array_at(&array, int_array_count(&array) - 1) == 4);
    }

    // make sure the array shrunk at least once
    // TODO currently disabled, array doesn't shrink yet!!
    //pottery_test_assert(int_array_capacity(&array) < 1000);

    int_array_destroy(&array);
}

#undef POTTERY_TEST_ARRAY_INT

#undef POTTERY_TEST_ARRAY_INT_HAS_CAPACITY
#undef POTTERY_TEST_ARRAY_INT_PREFIX
