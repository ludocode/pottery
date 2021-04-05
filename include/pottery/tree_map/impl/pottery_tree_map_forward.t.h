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

#ifndef POTTERY_TREE_MAP_IMPL
#error "This is an internal header. Do not include it."
#endif

typedef struct pottery_tree_map_node_t {

    // value is the first field in node so that we can safely cast between node
    // and value under strict aliasing rules. find all instances of
    // pottery_reinterpret_cast() in tree_map if you want to change this.
    pottery_tree_map_value_t value;

    // right_child and is_red are collapsed together; is_red is the low bit.
    struct pottery_tree_map_node_t* parent;
    struct pottery_tree_map_node_t* left_child;
    uintptr_t right_child_and_is_red;

} pottery_tree_map_node_t;

typedef struct pottery_tree_map_t pottery_tree_map_t;

typedef pottery_tree_map_ref_t pottery_tree_map_entry_t;

static inline
pottery_tree_map_node_t* pottery_tree_map_impl_alloc(pottery_tree_map_t* map);

// Convert a node to a ref, correctly handling null
static inline
pottery_tree_map_ref_t pottery_tree_map_node_ref(pottery_tree_map_node_t* /*nullable*/ node) {
    return pottery_reinterpret_cast(pottery_tree_map_ref_t, node);
}

// Convert a ref to a node, correctly handling null
static inline
pottery_tree_map_node_t* pottery_tree_map_ref_node(pottery_tree_map_ref_t /*nullable*/ ref) {
    return pottery_reinterpret_cast(pottery_tree_map_node_t*, ref);
}

static inline
pottery_tree_map_key_t pottery_tree_map_node_key(pottery_tree_map_t* map, pottery_tree_map_node_t* node) {
    (void)map;
    #if defined(POTTERY_TREE_MAP_CONTEXT_TYPE)
        return pottery_tree_map_ref_key(map->context, &node->value);
    #else
        return pottery_tree_map_ref_key(&node->value);
    #endif
}

#ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
static inline
pottery_tree_map_context_t* pottery_tree_map_context(pottery_tree_map_t* map);
#endif

#if 0

#if POTTERY_TREE_MAP_CAN_DESTROY
static inline
void pottery_tree_map_entry_destroy(pottery_tree_map_t* map, pottery_tree_map_entry_t entry);
#endif

static inline
pottery_tree_map_key_t pottery_tree_map_entry_key(pottery_tree_map_t* map, pottery_tree_map_entry_t entry) {
    (void)map;
    #if defined(POTTERY_TREE_MAP_CONTEXT_TYPE)
        return pottery_tree_map_ref_key(map->context, entry);
    #else
        return pottery_tree_map_ref_key(entry);
    #endif
}

static inline
bool pottery_tree_map_entry_key_equal(pottery_tree_map_t* map, pottery_tree_map_key_t left, pottery_tree_map_key_t right);

static inline
size_t pottery_tree_map_entry_key_hash(pottery_tree_map_t* map, pottery_tree_map_key_t key);

static inline
bool pottery_tree_map_entry_is_empty(pottery_tree_map_t* map, pottery_tree_map_entry_t entry);

static inline
void pottery_tree_map_entry_set_empty(pottery_tree_map_t* map, pottery_tree_map_entry_t entry);

#if POTTERY_TREE_MAP_TOMBSTONES
static inline
bool pottery_tree_map_entry_is_tombstone(pottery_tree_map_t* map, pottery_tree_map_entry_t entry);

static inline
void pottery_tree_map_entry_set_tombstone(pottery_tree_map_t* map, pottery_tree_map_entry_t entry);
#endif

static inline
bool pottery_tree_map_entry_is_value(pottery_tree_map_t* map, pottery_tree_map_entry_t entry);

static inline
pottery_tree_map_entry_t pottery_tree_map_impl_values(pottery_tree_map_t* map);

/**
 * Returns the number of buckets currently in the map.
 */
static inline
size_t pottery_tree_map_bucket_count(pottery_tree_map_t* map);

#endif
