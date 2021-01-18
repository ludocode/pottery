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

// this is a minimal example of using the pottery priority_queue. this should become an
// introductory example to pottery.

/**
 * The below code statically defines a priority_queue of literal strings,
 * smallest first.
 *
 * We've chosen the prefix `cstr_pq`, so its type is `cstr_pq_t` and
 * all its functions start with `cstr_pq_`.
 */

#define POTTERY_PRIORITY_QUEUE_PREFIX cstr_pq
#define POTTERY_PRIORITY_QUEUE_VALUE_TYPE const char*
#define POTTERY_PRIORITY_QUEUE_LIFECYCLE_BY_VALUE 1
// We negate strcmp so that smaller values come first
#define POTTERY_PRIORITY_QUEUE_COMPARE_THREE_WAY(a,b) -strcmp(*a,*b)
#include "pottery/priority_queue/pottery_priority_queue_static.t.h"

#include "pottery/unit/test_pottery_framework.h"

POTTERY_TEST(pottery_cstr_pq_example) {
    cstr_pq_t priority_queue;
    cstr_pq_init(&priority_queue);

    cstr_pq_insert(&priority_queue, "Bob");
    cstr_pq_insert(&priority_queue, "Eve");
    cstr_pq_insert(&priority_queue, "Alice");
    cstr_pq_insert(&priority_queue, "Donna");
    cstr_pq_insert(&priority_queue, "Carl");

    /*
    puts(cstr_pq_extract_first(&priority_queue)); // prints Alice
    puts(cstr_pq_extract_first(&priority_queue)); // prints Bob
    puts(cstr_pq_extract_first(&priority_queue)); // prints Carl
    puts(cstr_pq_extract_first(&priority_queue)); // prints Donna
    puts(cstr_pq_extract_first(&priority_queue)); // prints Eve
    */

    pottery_test_assert(0 == strcmp(cstr_pq_extract_first(&priority_queue), "Alice"));
    pottery_test_assert(0 == strcmp(cstr_pq_extract_first(&priority_queue), "Bob"));
    pottery_test_assert(0 == strcmp(cstr_pq_extract_first(&priority_queue), "Carl"));
    pottery_test_assert(0 == strcmp(cstr_pq_extract_first(&priority_queue), "Donna"));
    pottery_test_assert(0 == strcmp(cstr_pq_extract_first(&priority_queue), "Eve"));

    cstr_pq_destroy(&priority_queue);
}
