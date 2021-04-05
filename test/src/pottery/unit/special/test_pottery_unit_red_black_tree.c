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
#include <stdbool.h>

// workarounds for MSVC C++/CLR not allowing different structs in different
// translation units with the same name
#ifdef __CLR_VER
#define foo_t rbt_foo_t
#define foo_tree_t rbt_foo_tree_t
#endif

typedef struct foo_t {
    struct foo_t* left_child;
    struct foo_t* right_child;
    struct foo_t* parent;
    bool is_red;
    size_t value;
} foo_t;

#define POTTERY_RED_BLACK_TREE_PREFIX foo_tree
#define POTTERY_RED_BLACK_TREE_KEY_TYPE size_t
#define POTTERY_RED_BLACK_TREE_VALUE_TYPE foo_t
#define POTTERY_RED_BLACK_TREE_REF_KEY(foo) foo->value
#define POTTERY_RED_BLACK_TREE_COMPARE_LESS(x, y) x < y
#include "pottery/red_black_tree/pottery_red_black_tree_static.t.h"

#include "pottery/unit/test_pottery_framework.h"

POTTERY_TEST(pottery_red_black_tree_basic) {
    foo_tree_t tree;
    foo_tree_init(&tree);

    foo_t foo;
    foo.value = 5;
    foo_tree_link(&tree, &foo);

    foo_tree_unlink_all(&tree);
    foo_tree_destroy(&tree);
}

POTTERY_TEST(pottery_red_black_tree_insert_in_order) {
    foo_tree_t tree;
    foo_tree_init(&tree);

    foo_t foos[3];
    size_t count = pottery_array_count(foos);
    size_t i;

    for (i = 0; i < count; ++i) {
        foos[i].value = i;
        foo_tree_link(&tree, &foos[i]);
    }

    for (i = 0; i < count; ++i) {
        foo_t* foo = foo_tree_find(&tree, i);
        pottery_test_assert(foo == &foos[i]);
    }

    for (i = 0; i < count; ++i) {
        foo_tree_unlink(&tree, &foos[i]);
    }

    #undef COUNT

    foo_tree_unlink_all(&tree);
    foo_tree_destroy(&tree);
}
