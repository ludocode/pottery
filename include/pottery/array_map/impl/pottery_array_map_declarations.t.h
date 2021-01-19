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

#ifndef POTTERY_ARRAY_MAP_IMPL
#error "This is an internal header. Do not include it."
#endif

#ifdef POTTERY_ARRAY_MAP_CONTEXT_TYPE
typedef POTTERY_ARRAY_MAP_CONTEXT_TYPE pottery_context_t;
#endif

typedef struct pottery_array_map_t {
    pottery_array_map_vector_t vector;

    #ifdef POTTERY_ARRAY_MAP_CONTEXT_TYPE
    pottery_context_t context;
    #endif
} pottery_array_map_t;

POTTERY_ARRAY_MAP_EXTERN
void pottery_array_map_init(pottery_array_map_t* map
    #ifdef POTTERY_ARRAY_MAP_CONTEXT_TYPE
    , pottery_context_t context
    #endif
    );

#ifdef POTTERY_ARRAY_MAP_ENABLE_EXTERN
POTTERY_ARRAY_MAP_EXTERN
void pottery_array_map_init_external(pottery_array_map_t* map,
    #ifdef POTTERY_ARRAY_MAP_CONTEXT_TYPE
    pottery_context_t context,
    #endif
    pottery_array_map_value_t* external, size_t external_capacity);
#endif

static inline
void pottery_array_map_destroy(pottery_array_map_t* map) {
    pottery_array_map_vector_destroy(&map->vector);
}

static inline
size_t pottery_array_map_count(pottery_array_map_t* map) {
    return pottery_array_map_vector_count(&map->vector);
}

static inline
size_t pottery_array_map_capacity(pottery_array_map_t* map) {
    return pottery_array_map_vector_capacity(&map->vector);
}

static inline
bool pottery_array_map_is_empty(pottery_array_map_t* map) {
    return pottery_array_map_vector_is_empty(&map->vector);
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
static inline
void pottery_array_map_remove_all(pottery_array_map_t* map) {
    pottery_array_map_vector_remove_all(&map->vector);
}
#endif

static inline
void pottery_array_map_displace_all(pottery_array_map_t* map) {
    pottery_array_map_vector_displace_all(&map->vector);
}

// fundamental accessors

/**
 * Finds a value by key.
 *
 * Use pottery_array_map_entry_exists() to determine whether a value was found.
 */
POTTERY_ARRAY_MAP_EXTERN
pottery_array_map_value_t* pottery_array_map_find(pottery_array_map_t* map,
        pottery_array_map_key_t key);

/**
 * Creates space for a value with the given key.
 */
POTTERY_ARRAY_MAP_EXTERN
pottery_error_t pottery_array_map_emplace_key(pottery_array_map_t* map,
        pottery_array_map_key_t key, pottery_array_map_entry_t* entry,
        bool* /*nullable*/ created);

// TODO this is temporarily here for backwards compatibility with old HomeFort
// code. Don't use place(); use emplace_key() instead.
static inline
pottery_array_map_value_t* pottery_array_map_place(pottery_array_map_t* map,
        pottery_array_map_key_t key, bool* /*non-null*/ created)
{
    pottery_array_map_entry_t entry;
    if (pottery_unlikely(POTTERY_OK != pottery_array_map_emplace_key(map, key, &entry, created)))
        return pottery_null;
    return entry;
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_ARRAY_MAP_EXTERN
void pottery_array_map_remove(pottery_array_map_t* map,
        pottery_array_map_entry_t entry);

/**
 * Returns true if an entry matching the given key was removed.
 */
POTTERY_ARRAY_MAP_EXTERN
bool pottery_array_map_remove_key(pottery_array_map_t* map, pottery_array_map_key_t key);
#endif

POTTERY_ARRAY_MAP_EXTERN
void pottery_array_map_displace(pottery_array_map_t* map,
        pottery_array_map_value_t* value);

POTTERY_ARRAY_MAP_EXTERN
bool pottery_array_map_displace_key(pottery_array_map_t* map,
        pottery_array_map_key_t key);

// lifecycle accessors

// value functions

// TODO get rid of this, replaced by ref_key
static inline
pottery_array_map_key_t pottery_array_map_value_key(pottery_array_map_value_t* value) {
    return pottery_array_map_ref_key(value);
}


// iterators

static inline
pottery_array_map_value_t* pottery_array_map_begin(pottery_array_map_t* map) {
    return pottery_array_map_vector_begin(&map->vector);
}

static inline
pottery_array_map_value_t* pottery_array_map_end(pottery_array_map_t* map) {
    return pottery_array_map_vector_end(&map->vector);
}

static inline
pottery_array_map_value_t* pottery_array_map_first(pottery_array_map_t* map) {
    return pottery_array_map_vector_first(&map->vector);
}

static inline
pottery_array_map_value_t* pottery_array_map_last(pottery_array_map_t* map) {
    return pottery_array_map_vector_last(&map->vector);
}

static inline
pottery_array_map_value_t* pottery_array_map_any(pottery_array_map_t* map) {
    // Return the last value so that nothing shifts when it's popped.
    return pottery_array_map_vector_last(&map->vector);
}

static inline
pottery_array_map_value_t* pottery_array_map_next(pottery_array_map_t* map,
        pottery_array_map_value_t* value)
{
    return pottery_array_map_vector_next(&map->vector, value);
}

static inline
pottery_array_map_value_t* pottery_array_map_entry_value(pottery_array_map_t* map,
        pottery_array_map_entry_t entry)
{
    (void)map;
    return entry;
}

static inline
bool pottery_array_map_entry_exists(pottery_array_map_t* map, pottery_array_map_entry_t entry) {
    return entry != pottery_array_map_end(map);
}

static inline
pottery_error_t pottery_array_map_reserve(pottery_array_map_t* map, size_t count) {
    return pottery_array_map_vector_reserve(&map->vector, count);
}

static inline
pottery_error_t pottery_array_map_shrink(pottery_array_map_t* map) {
    return pottery_array_map_vector_shrink(&map->vector);
}


/*
 * insert()
 */

#if POTTERY_LIFECYCLE_CAN_PASS && POTTERY_LIFECYCLE_CAN_DESTROY
/**
 * Inserts a value.
 *
 * If a value with the same key already exists in the map, the old value is
 * destroyed and replaced with the given value.
 */
POTTERY_ARRAY_MAP_EXTERN
pottery_error_t pottery_array_map_insert(pottery_array_map_t* map,
        pottery_array_map_value_t value);
#endif


/*
 * extract()
 */

#if POTTERY_LIFECYCLE_CAN_PASS

static inline
pottery_array_map_value_t pottery_array_map_extract(pottery_array_map_t* map,
        pottery_array_map_entry_t entry)
{
    pottery_array_map_ref_t ref = pottery_array_map_entry_value(map, entry);
    pottery_array_map_value_t ret = pottery_move_if_cxx(*ref);
    #ifdef __cplusplus
    // We have to run the destructor. See note in pottery_vector_extract()
    ref->~pottery_array_map_value_t();
    #endif
    pottery_array_map_displace(map, entry);
    return ret;
}

/**
 * Extracts a value by key. The value must exist.
 */
static inline
pottery_array_map_value_t pottery_array_map_extract_key(pottery_array_map_t* map,
        pottery_array_map_key_t key)
{
    pottery_array_map_entry_t entry = pottery_array_map_find(map, key);
    pottery_assert(pottery_array_map_entry_exists(map, entry));
    return pottery_array_map_extract(map, entry);
}

#endif // extract criteria
