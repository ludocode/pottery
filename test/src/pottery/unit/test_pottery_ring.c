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

// We define an int ring as separate declare/define headers to make sure it
// works properly. We also give it a small page size to help test paging and
// bulk methods.
#define POTTERY_RING_PREFIX     int_ring
#define POTTERY_RING_VALUE_TYPE int
#define POTTERY_RING_LIFECYCLE_MOVE_BY_VALUE 1
#include "pottery/ring/pottery_ring_declare.t.h"
#define POTTERY_RING_PREFIX     int_ring
#define POTTERY_RING_VALUE_TYPE int
#define POTTERY_RING_LIFECYCLE_MOVE_BY_VALUE 1
#include "pottery/ring/pottery_ring_define.t.h"

// We also define a ring of UFOs to make sure we can use ring twice in one
// translation unit
#include "pottery/common/test_pottery_ufo.h"
#define POTTERY_RING_PREFIX ufo_ring
#define POTTERY_RING_VALUE_TYPE ufo_t
#define POTTERY_RING_LIFECYCLE_INIT_COPY ufo_init_copy
#define POTTERY_RING_LIFECYCLE_MOVE ufo_move
#define POTTERY_RING_LIFECYCLE_DESTROY ufo_destroy
#include "pottery/ring/pottery_ring_static.t.h"

#include "pottery/unit/test_pottery_framework.h"

POTTERY_TEST(pottery_ring_int_init_destroy) {
    int_ring_t ring;
    int_ring_init(&ring);
    pottery_test_assert(int_ring_count(&ring) == 0);
    int_ring_destroy(&ring);
}

static void test_pottery_ring_int_normal(int count) {
    int_ring_t ring;
    int_ring_init(&ring);

    int i;
    for (i = 0; i < count; ++i) {
        int_ring_insert_last(&ring, i);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(*int_ring_at(&ring, pottery_cast(size_t, i)) == i);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(i == int_ring_extract_first(&ring));
    }

    int_ring_destroy(&ring);
}

POTTERY_TEST(pottery_ring_int_normal_many) {
    // enough to span many pages
    test_pottery_ring_int_normal(5000);
}

POTTERY_TEST(pottery_ring_int_normal_some) {
    // less than a page worth
    test_pottery_ring_int_normal(50);
}

static void test_pottery_ring_int_reverse(int count) {
    int_ring_t ring;
    int_ring_init(&ring);

    int i;
    for (i = 0; i < count; ++i) {
        int_ring_insert_first(&ring, i);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(*int_ring_at(&ring, pottery_cast(size_t, i)) == count - i - 1);
    }

    for (i = 0; i < count; ++i) {
        pottery_test_assert(i == int_ring_extract_last(&ring));
    }

    int_ring_destroy(&ring);
}

POTTERY_TEST(pottery_ring_int_reverse_many) {
    // enough to span many pages
    test_pottery_ring_int_reverse(5000);
}

POTTERY_TEST(pottery_ring_int_reverse_some) {
    // less than a page worth
    test_pottery_ring_int_reverse(50);
}

static const int bulk_count = 200; // TODO 200

POTTERY_TEST(pottery_ring_bulk_insert) {
    int_ring_t ring;
    int_ring_init(&ring);

    int block[10000];

    int count, i;
    for (count = 1; count < bulk_count; ++count) {
        for (i = 0; i < count; ++i)
            block[i] = i;
        int_ring_insert_last_bulk(&ring, block, pottery_cast(size_t, count));

        //fprintf(stderr, "inserted bulk, now contains:\n");
        //for (int i = 0; i < (int)int_ring_count(&ring); ++i)
            //fprintf(stderr, "%i ", *int_ring_at(&ring, (size_t)i));
        //fprintf(stderr, "\n");
    }

    for (count = 1; count < bulk_count; ++count)
        for (i = 0; i < count; ++i)
            pottery_test_assert(i == int_ring_extract_first(&ring));

    int_ring_destroy(&ring);
}

POTTERY_TEST(pottery_ring_bulk_insert_first) {
    int_ring_t ring;
    int_ring_init(&ring);

    int block[10000];

    int count, i;
    for (count = 1; count < bulk_count; ++count) {
        for (i = 0; i < count; ++i)
            block[i] = i;
//fprintf(stderr,"*************************** insert first bulk %i\n",count);
        int_ring_insert_first_bulk(&ring, block, pottery_cast(size_t, count));
    }

    // Each block is in ascending order, but we've inserted the blocks in reverse
    // order.
    for (count = bulk_count - 1; count > 0; --count)
        for (i = 0; i < count; ++i)
//(fprintf(stderr,"====\n")),
//(fprintf(stderr,"count %i expected %i actual %i\n",count, i,*int_ring_at(&ring,0))),
            pottery_test_assert(i == int_ring_extract_first(&ring));

    int_ring_destroy(&ring);
}

POTTERY_TEST(pottery_ring_bulk_extract) {
    int_ring_t ring;
    int_ring_init(&ring);

    int block[10000];

    int count, i;
    for (count = 1; count < bulk_count; ++count)
        for (i = 0; i < count; ++i)
            int_ring_insert_last(&ring, i);

    for (count = 1; count < bulk_count; ++count) {
        int_ring_extract_first_bulk(&ring, block, pottery_cast(size_t, count));
        for (i = 0; i < count; ++i)
            pottery_test_assert(i == block[i]);
    }

    int_ring_destroy(&ring);
}

POTTERY_TEST(pottery_ring_bulk_extract_last) {
    int_ring_t ring;
    int_ring_init(&ring);

    int block[10000];

    int count, i;
    for (count = 1; count < bulk_count; ++count)
        for (i = 0; i < count; ++i)
//(fprintf(stderr,"%i\n",i)),
            int_ring_insert_last(&ring, i);

    // Each block is in ascending order, but we're extractping the blocks in
    // reverse order.
    for (count = bulk_count - 1; count > 0; --count) {
        int_ring_extract_last_bulk(&ring, block, pottery_cast(size_t, count));
        for (i = 0; i < count; ++i)
//(fprintf(stderr,"%i %i\n",i,block[i])),
            pottery_test_assert(i == block[i]);
    }

    int_ring_destroy(&ring);
}

POTTERY_TEST(pottery_ring_wrap_forward) {
    int step_size = 5;
    int steps = 200;

    int_ring_t ring;
    int_ring_init(&ring);
    int value = 0;

    // load some values into the ring
    int i;
    for (i = 0; i < step_size; ++i)
        int_ring_insert_last(&ring, value + i);

    // repeatedly test loading more values, then extractping the original values,
    // leaving always at least step_size values in the ring. this tests
    // wrapping behaviour of the ring.
    int step;
    for (step = 0; step < steps; ++step) {
        for (i = 0; i < step_size; ++i)
            int_ring_insert_last(&ring, value + step_size + i);
        for (i = 0; i < step_size; ++i)
            pottery_test_assert((value + i) == int_ring_extract_first(&ring));
        value += step_size;
    }

    // extract the final values from the ring
    for (i = 0; i < step_size; ++i)
        pottery_test_assert((value + i) == int_ring_extract_first(&ring));

    int_ring_destroy(&ring);
}

POTTERY_TEST(pottery_ring_wrap_lastward) {
    int step_size = 5;
    int steps = 200;

    int_ring_t ring;
    int_ring_init(&ring);
    int value = 0;

    // load some values into the ring
    int i;
    for (i = 0; i < step_size; ++i)
        int_ring_insert_first(&ring, value + i);

    // repeatedly test loading more values, then extractping the original values,
    // leaving always at least step_size values in the ring. this tests
    // wrapping behaviour of the ring.
    int step;
    for (step = 0; step < steps; ++step) {
        for (i = 0; i < step_size; ++i)
            int_ring_insert_first(&ring, value + step_size + i);
        for (i = 0; i < step_size; ++i)
            pottery_test_assert((value + i) == int_ring_extract_last(&ring));
        value += step_size;
    }

    // extract the final values from the ring
    for (i = 0; i < step_size; ++i)
        pottery_test_assert((value + i) == int_ring_extract_last(&ring));

    int_ring_destroy(&ring);
}
