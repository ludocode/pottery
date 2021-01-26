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
#include <stdint.h>

// workarounds for MSVC C++/CLR not allowing different structs in different
// translation units with the same name
#ifdef __CLR_VER
#define coords_t dll_struct_ref_coords_t
#define node_t dll_struct_ref_node_t
#endif

// Test of a struct ref type which is not comparable with operator==. The ref
// is a pair of coordinates within a two-dimensional pool of nodes. The context
// is the pool.

typedef struct coords_t {
    uint8_t x, y;
} coords_t;

typedef struct node_t {
    coords_t next;
    coords_t previous;
    const char* name;
} node_t;

#define POTTERY_DOUBLY_LINKED_LIST_PREFIX node_list
#define POTTERY_DOUBLY_LINKED_LIST_REF_TYPE coords_t
#define POTTERY_DOUBLY_LINKED_LIST_CONTEXT_TYPE node_t**
// We need a custom REF_EQUAL since structs cannot be compared with ==
#define POTTERY_DOUBLY_LINKED_LIST_REF_EQUAL(pool, a, b) a.x == b.x && a.y == b.y
// We need custom NEXT and PREVIOUS pointers to dereference our refs
#define POTTERY_DOUBLY_LINKED_LIST_NEXT(pool, ref) pool[ref.x][ref.y].next
#define POTTERY_DOUBLY_LINKED_LIST_PREVIOUS(pool, ref) pool[ref.x][ref.y].previous
// We need a custom NULL to store an invalid ref
#define POTTERY_DOUBLY_LINKED_LIST_NULL(pool) pottery_compound_literal(coords_t, UINT8_MAX, UINT8_MAX)
#include "pottery/doubly_linked_list/pottery_doubly_linked_list_static.t.h"

#include "pottery/unit/test_pottery_framework.h"

POTTERY_TEST(pottery_doubly_linked_list_struct_ref) {
    static node_t nodes[25];
    static node_t* pool[] = {nodes, nodes + 5, nodes + 10, nodes + 15, nodes + 25};

    node_list_t list;
    node_list_init(&list, pool);

    {
        coords_t bob = {3, 4};
        pool[bob.x][bob.y].name = "bob";
        node_list_link_last(&list, bob);

        coords_t alice = {2, 1};
        pool[alice.x][alice.y].name = "alice";
        node_list_link_first(&list, alice);

        coords_t eve = {1, 0};
        pool[eve.x][eve.y].name = "eve";

        // Note: This fails in TinyCC 0.9.27, apparently due to a
        // miscompilation bug.
        //node_list_link_after(&list, node_list_first(&list), eve);
        node_list_link_after(&list, alice, eve);
    }

    coords_t ref = node_list_begin(&list);
    pottery_test_assert(0 == strcmp(pool[ref.x][ref.y].name, "alice"));
    ref = node_list_next(&list, ref);
    pottery_test_assert(0 == strcmp(pool[ref.x][ref.y].name, "eve"));
    ref = node_list_next(&list, ref);
    pottery_test_assert(0 == strcmp(pool[ref.x][ref.y].name, "bob"));
    ref = node_list_next(&list, ref);
    pottery_test_assert(!node_list_entry_exists(&list, ref));

    node_list_unlink_all(&list);
    node_list_destroy(&list);
}
