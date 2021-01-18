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

// TODO priority queue context is not fully implemented yet
//static const char* test_context = "test-p-pottery_pq_test-context";

#define POTTERY_PRIORITY_QUEUE_PREFIX int_priority_queue
#define POTTERY_PRIORITY_QUEUE_VALUE_TYPE int
#define POTTERY_PRIORITY_QUEUE_LIFECYCLE_BY_VALUE 1
#define POTTERY_PRIORITY_QUEUE_COMPARE_LESS(x, y) *x < *y
//#define POTTERY_PRIORITY_QUEUE_CONTEXT_TYPE const char*
#include "pottery/priority_queue/pottery_priority_queue_declare.t.h"

#define POTTERY_PRIORITY_QUEUE_PREFIX int_priority_queue
#define POTTERY_PRIORITY_QUEUE_VALUE_TYPE int
#define POTTERY_PRIORITY_QUEUE_LIFECYCLE_BY_VALUE 1
#define POTTERY_PRIORITY_QUEUE_COMPARE_LESS(x, y) *x < *y
//#define POTTERY_PRIORITY_QUEUE_CONTEXT_TYPE const char*
#include "pottery/priority_queue/pottery_priority_queue_define.t.h"

#include "pottery/unit/test_pottery_framework.h"

typedef struct foo_t {
    int value;
    size_t index;
} foo_t;

#define POTTERY_PRIORITY_QUEUE_PREFIX foo_priority_queue
#define POTTERY_PRIORITY_QUEUE_VALUE_TYPE foo_t
#define POTTERY_PRIORITY_QUEUE_LIFECYCLE_BY_VALUE 1
#define POTTERY_PRIORITY_QUEUE_COMPARE_LESS(left, right) left->value < right->value
#define POTTERY_PRIORITY_QUEUE_SET_INDEX(foo, position) foo->index = position
#include "pottery/priority_queue/pottery_priority_queue_static.t.h"

POTTERY_TEST(pottery_int_priority_queue_init_destroy) {
    int_priority_queue_t priority_queue;
    int_priority_queue_init(&priority_queue/*, test_context*/);
    pottery_test_assert(int_priority_queue_count(&priority_queue) == 0);
    int_priority_queue_destroy(&priority_queue);
}

POTTERY_TEST(pottery_int_priority_queue_single_element) {
    int_priority_queue_t priority_queue;
    int_priority_queue_init(&priority_queue/*, test_context*/);

    int_priority_queue_insert(&priority_queue, 5);
    pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == 5);

    int_priority_queue_destroy(&priority_queue);
}

POTTERY_TEST(pottery_int_priority_queue_small_number_of_elements) {
    int_priority_queue_t priority_queue;
    int_priority_queue_init(&priority_queue/*, test_context*/);

    int_priority_queue_insert(&priority_queue, 5);
    int_priority_queue_insert(&priority_queue, 1);
    int_priority_queue_insert(&priority_queue, 3);
    int_priority_queue_insert(&priority_queue, 4);
    int_priority_queue_insert(&priority_queue, 2);

    pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == 5);
    pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == 4);
    pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == 3);
    pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == 2);
    pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == 1);

    int_priority_queue_destroy(&priority_queue);
}

POTTERY_TEST(pottery_int_priority_queue_many) {
    int_priority_queue_t priority_queue;
    int_priority_queue_init(&priority_queue/*, test_context*/);

    int count = 10000;

    // insert scattered elements
    int i;
    for (i = 0; i < count; ++i) {
        int_priority_queue_insert(&priority_queue, (i * 31) % count);
    }

    // make sure they come out in order
    for (i = count; i > 0;) {
        --i;
        pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == i);
    }

    int_priority_queue_destroy(&priority_queue);
}

POTTERY_TEST(pottery_int_priority_queue_remove_arbitrary) {
    int_priority_queue_t priority_queue;
    int_priority_queue_init(&priority_queue/*, test_context*/);

    int count = 1000;

    // insert scattered elements
    int i;
    for (i = 0; i < count; ++i) {
        int_priority_queue_insert(&priority_queue, (i * 31) % count);
    }

    // remove some random elements
    for (i = 0; i < count / 10; ++i) {
        int_priority_queue_remove_at(&priority_queue, pottery_cast(size_t, (i * 487) %
                    pottery_cast(int, int_priority_queue_count(&priority_queue))));
    }

    // remove the rest, making sure they come out in order
    int last = INT_MAX;
    while (!int_priority_queue_is_empty(&priority_queue)) {
        int value = int_priority_queue_extract_first(&priority_queue);
        pottery_test_assert(value < last);
        last = value;
    }

    int_priority_queue_destroy(&priority_queue);
}

POTTERY_TEST(pottery_int_priority_queue_insert_bulk) {
    int_priority_queue_t priority_queue;
    int_priority_queue_init(&priority_queue/*, test_context*/);

    // insert in bulk. this should cause a bulk append and repair.
    int inserts[5] = {5, 1, 3, 4, 2};
    int_priority_queue_insert_bulk(&priority_queue, inserts, 5);

    // make sure they come out in order
    pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == 5);
    pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == 4);
    pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == 3);
    pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == 2);
    pottery_test_assert(int_priority_queue_extract_first(&priority_queue) == 1);

    int_priority_queue_destroy(&priority_queue);
}

static void test_foo_priority_queue_check(foo_priority_queue_t* priority_queue) {
    // Check that the index of every foo in the priority_queue has been updated
    // correctly
    size_t i;
    for (i = 0; i < foo_priority_queue_count(priority_queue); ++i) {
        //mlogD("checking at %i", (int)i);
        pottery_test_assert(foo_priority_queue_array_at(&priority_queue->array, i)->index == i);
    }
}

POTTERY_TEST(pottery_foo_priority_queue_set_index) {
    foo_priority_queue_t priority_queue;
    foo_priority_queue_init(&priority_queue);

    size_t max = 100;
    size_t i;
    for (i = 0; i < max; ++i) {
        foo_t foo = {pottery_cast(int, ((i+1) * 67) % max), 0};
        foo_priority_queue_insert(&priority_queue, foo);
        test_foo_priority_queue_check(&priority_queue);
    }

    int last = INT_MAX;
    while (!foo_priority_queue_is_empty(&priority_queue)) {
        int value = foo_priority_queue_extract_first(&priority_queue).value;
        pottery_test_assert(value <= last);
        last = value;

        test_foo_priority_queue_check(&priority_queue);
    }

    foo_priority_queue_destroy(&priority_queue);
}
