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

struct pottery_tree_map_t {
    pottery_tree_map_tree_t tree;
    #ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
    pottery_tree_map_context_t context;
    #endif
};

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_init(pottery_tree_map_t* map
        #ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
        , pottery_tree_map_context_t context
        #endif
        );

POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_destroy(pottery_tree_map_t* map);

/**
 *
 */
POTTERY_TREE_MAP_EXTERN
pottery_error_t pottery_tree_map_emplace_key(pottery_tree_map_t* map, pottery_tree_map_key_t key,
        pottery_tree_map_entry_t* entry, bool* /*nullable*/ out_created);
#endif

static inline
size_t pottery_tree_map_count(pottery_tree_map_t* map) {
    return pottery_tree_map_tree_count(&map->tree);
}

static inline
bool pottery_tree_map_is_empty(pottery_tree_map_t* map) {
    return pottery_tree_map_count(map) == 0;
}

static inline
bool pottery_tree_map_entry_exists(pottery_tree_map_t* map, pottery_tree_map_entry_t entry) {
    (void)map;
    return entry != pottery_null;
}

static inline
pottery_tree_map_entry_t pottery_tree_map_find(pottery_tree_map_t* map, pottery_tree_map_key_t key) {
    return pottery_tree_map_node_ref(pottery_tree_map_tree_find(&map->tree, key));
}

POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_displace(pottery_tree_map_t* map, pottery_tree_map_entry_t entry);

POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_displace_all(pottery_tree_map_t* map);

/**
 * Returns true if an entry matching the given key was displaced.
 */
POTTERY_TREE_MAP_EXTERN
bool pottery_tree_map_displace_key(pottery_tree_map_t* map, pottery_tree_map_key_t key);

#if POTTERY_TREE_MAP_CAN_DESTROY
POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_remove(pottery_tree_map_t* map, pottery_tree_map_entry_t entry);

POTTERY_TREE_MAP_EXTERN
void pottery_tree_map_remove_all(pottery_tree_map_t* map);

/**
 * Returns true if an entry matching the given key was removed.
 */
POTTERY_TREE_MAP_EXTERN
bool pottery_tree_map_remove_key(pottery_tree_map_t* map, pottery_tree_map_key_t key);
#endif

static inline
pottery_tree_map_value_t* pottery_tree_map_entry_ref(pottery_tree_map_t* map, pottery_tree_map_entry_t entry) {
    (void)map;
    return entry;
}

/**
 * Returns true if there is a value in the map with this key.
 */
static inline
bool pottery_tree_map_contains_key(pottery_tree_map_t* map, pottery_tree_map_key_t key) {
    return pottery_null != pottery_tree_map_find(map, key);
}

static inline
pottery_tree_map_entry_t pottery_tree_map_any(pottery_tree_map_t* map) {
    return pottery_tree_map_node_ref(pottery_tree_map_tree_any(&map->tree));
}

static inline
pottery_tree_map_entry_t pottery_tree_map_begin(pottery_tree_map_t* map) {
    return pottery_tree_map_node_ref(pottery_tree_map_tree_begin(&map->tree));
}

static inline
pottery_tree_map_entry_t pottery_tree_map_end(pottery_tree_map_t* map) {
    return pottery_tree_map_node_ref(pottery_tree_map_tree_end(&map->tree));
}

static inline
pottery_tree_map_entry_t pottery_tree_map_first(pottery_tree_map_t* map) {
    return pottery_tree_map_node_ref(pottery_tree_map_tree_first(&map->tree));
}

static inline
pottery_tree_map_entry_t pottery_tree_map_last(pottery_tree_map_t* map) {
    return pottery_tree_map_node_ref(pottery_tree_map_tree_last(&map->tree));
}

static inline
pottery_tree_map_entry_t pottery_tree_map_next(pottery_tree_map_t* map, pottery_tree_map_entry_t entry) {
    return pottery_tree_map_node_ref(pottery_tree_map_tree_next(&map->tree, pottery_tree_map_ref_node(entry)));
}

static inline
pottery_tree_map_entry_t pottery_tree_map_previous(pottery_tree_map_t* map, pottery_tree_map_entry_t entry) {
    return pottery_tree_map_node_ref(pottery_tree_map_tree_previous(&map->tree, pottery_tree_map_ref_node(entry)));
}

#ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
static inline
pottery_tree_map_context_t* pottery_tree_map_context(pottery_tree_map_t* map) {
    return &map->context;
}
#endif


/*
 * insert()
 */

#if POTTERY_TREE_MAP_CAN_PASS && POTTERY_TREE_MAP_CAN_DESTROY
/**
 * Inserts a value.
 *
 * If a value with the same key already exists in the map, the old value is
 * destroyed and replaced with the given value.
 */
POTTERY_TREE_MAP_EXTERN
pottery_error_t pottery_tree_map_insert(pottery_tree_map_t* map,
        pottery_tree_map_value_t value);
#endif


/*
 * extract()
 */

#if POTTERY_TREE_MAP_CAN_PASS

static inline
pottery_tree_map_value_t pottery_tree_map_extract(pottery_tree_map_t* map,
        pottery_tree_map_entry_t entry)
{
    pottery_tree_map_ref_t ref = pottery_tree_map_entry_ref(map, entry);
    pottery_tree_map_value_t ret = pottery_move_if_cxx(*ref);
    #ifdef __cplusplus
    // We have to run the destructor. See note in pottery_vector_extract()
    ref->~pottery_tree_map_value_t();
    #endif
    pottery_tree_map_displace(map, entry);
    return ret;
}

/**
 * Extracts a value by key. The value must exist.
 */
static inline
pottery_tree_map_value_t pottery_tree_map_extract_key(pottery_tree_map_t* map,
        pottery_tree_map_key_t key)
{
    pottery_tree_map_entry_t entry = pottery_tree_map_find(map, key);
    pottery_assert(pottery_tree_map_entry_exists(map, entry));
    return pottery_tree_map_extract(map, entry);
}

#endif
