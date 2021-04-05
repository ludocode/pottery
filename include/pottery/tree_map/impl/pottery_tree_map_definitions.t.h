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

// Allocate a node
static inline
pottery_tree_map_node_t* pottery_tree_map_impl_alloc(pottery_tree_map_t* map) {
    (void)map;
    void* ptr = pottery_tree_map_alloc_malloc(POTTERY_TREE_MAP_CONTEXT_VAL
                pottery_alignof(pottery_tree_map_node_t),
                sizeof(pottery_tree_map_node_t));
    return pottery_cast(pottery_tree_map_node_t*, ptr);
}

// Free a node
static inline
void pottery_tree_map_impl_free(pottery_tree_map_t* map, pottery_tree_map_node_t* node) {
    (void)map;
    pottery_tree_map_alloc_free(
            POTTERY_TREE_MAP_CONTEXT_VAL
            pottery_alignof(pottery_tree_map_node_t),
            node);
}

POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_init(pottery_tree_map_t* map
        #ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
        , pottery_tree_map_context_t context
        #endif
) {
    #ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
    map->context = context;
    #endif
    pottery_tree_map_tree_init(&map->tree, map);
    //return POTTERY_OK;
}

POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_destroy(pottery_tree_map_t* map) {

    #if POTTERY_TREE_MAP_CAN_DESTROY
    pottery_tree_map_remove_all(map);
    #else
    // If we don't have a destroy expression, you must manually empty the hash
    // map before destroying it.
    pottery_assert(pottery_tree_map_is_empty(map));
    #endif
}

POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_displace(pottery_tree_map_t* map, pottery_tree_map_entry_t entry) {
    pottery_tree_map_node_t* node = pottery_tree_map_ref_node(entry);
    pottery_tree_map_tree_unlink(&map->tree, node);
    pottery_tree_map_impl_free(map, node);
}

POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_displace_all(pottery_tree_map_t* map) {
    // TODO speed, shouldn't need to rebalance as we go, should be able to just
    // depth-first walk and free as we go
    while (true) {
        pottery_tree_map_entry_t entry = pottery_tree_map_any(map);
        if (entry == pottery_null)
            break;
        pottery_tree_map_displace(map, entry);
    }
}

POTTERY_TREE_MAP_EXTERN
bool pottery_tree_map_displace_key(pottery_tree_map_t* map, pottery_tree_map_key_t key) {
    pottery_tree_map_entry_t entry = pottery_tree_map_find(map, key);
    if (entry == pottery_null)
        return false;
    pottery_tree_map_displace(map, entry);
    return true;
}

#if POTTERY_TREE_MAP_CAN_DESTROY
POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_remove(pottery_tree_map_t* map, pottery_tree_map_entry_t entry) {
    pottery_tree_map_node_t* node = pottery_tree_map_ref_node(entry);
    pottery_tree_map_tree_unlink(&map->tree, node);
    pottery_tree_map_lifecycle_destroy(POTTERY_TREE_MAP_CONTEXT_VAL &node->value);
    pottery_tree_map_impl_free(map, node);
}

POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_remove_all(pottery_tree_map_t* map) {
    // TODO speed, shouldn't need to rebalance as we go, should be able to just
    // depth-first walk and destroy/free as we go
    while (true) {
        pottery_tree_map_entry_t entry = pottery_tree_map_any(map);
        if (entry == pottery_null)
            break;
        pottery_tree_map_remove(map, entry);
    }
}

POTTERY_TREE_MAP_EXTERN
bool pottery_tree_map_remove_key(pottery_tree_map_t* map, pottery_tree_map_key_t key) {
    pottery_tree_map_entry_t entry = pottery_tree_map_find(map, key);
    if (entry == pottery_null)
        return false;
    pottery_tree_map_remove(map, entry);
    return true;
}
#endif

#if POTTERY_TREE_MAP_CAN_PASS && POTTERY_TREE_MAP_CAN_DESTROY
POTTERY_TREE_MAP_EXTERN
pottery_error_t pottery_tree_map_insert(pottery_tree_map_t* map,
        pottery_tree_map_value_t value)
{
    bool created;
    pottery_tree_map_entry_t entry;
    pottery_error_t error = pottery_tree_map_emplace_key(map,
            pottery_tree_map_ref_key(&value), &entry, &created);
    if (error == POTTERY_OK) {
        if (!created)
            pottery_tree_map_lifecycle_destroy(entry);
        pottery_move_construct(pottery_tree_map_value_t, *entry, value);
    }
    return error;
}
#endif // insert criteria
