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

#include <stddef.h>

// workarounds for MSVC C++/CLR not allowing different structs in different
// translation units with the same name
#ifdef __CLR_VER
#define foo_t dll_foo_t
#define foo_list_t dll_foo_list_t
#endif

typedef struct foo_t {
    struct foo_t* next;
    struct foo_t* previous;
    size_t value;
} foo_t;

#define POTTERY_DOUBLY_LINKED_LIST_PREFIX foo_list
#define POTTERY_DOUBLY_LINKED_LIST_VALUE_TYPE foo_t
#include "pottery/doubly_linked_list/pottery_doubly_linked_list_static.t.h"

#include "pottery/unit/test_pottery_framework.h"

POTTERY_TEST(pottery_doubly_linked_list_link_last) {
    foo_t foos[5];
    foo_list_t list;
    foo_list_init(&list);

    size_t i;
    for (i = 0; i < 5; ++i) {
        foos[i].value = i;
        pottery_test_assert(foo_list_count(&list) == i);
        foo_list_link_last(&list, &foos[i]);
    }

    pottery_test_assert(foo_list_count(&list) == 5);

    i = 0;
    foo_t* foo;
    for (foo = foo_list_first(&list); foo != pottery_null; foo = foo_list_next(&list, foo)) {
        pottery_test_assert(i++ == foo->value);
    }

    foo_list_unlink_all(&list);
    foo_list_destroy(&list);
}

POTTERY_TEST(pottery_doubly_linked_list_link_first) {
    foo_t foos[5];
    foo_list_t list;
    foo_list_init(&list);

    size_t i;
    for (i = 0; i < 5; ++i) {
        foos[i].value = i;
        foo_list_link_first(&list, &foos[i]);
    }

    i = 4;
    foo_t* foo;
    for (foo = foo_list_first(&list); foo != pottery_null; foo = foo_list_next(&list, foo)) {
        pottery_test_assert(i-- == foo->value);
    }

    foo_list_unlink_all(&list);
    foo_list_destroy(&list);
}
