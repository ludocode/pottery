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

#ifndef POTTERY_OPEN_HASH_MAP_IMPL
#error "This is an internal header. Do not include it."
#endif

struct pottery_ohm_t {
    pottery_ohm_table_t table;
    #if POTTERY_OPEN_HASH_MAP_HAS_METADATA
    uint8_t* metadata;
    #endif
};

POTTERY_OPEN_HASH_MAP_EXTERN
pottery_error_t pottery_ohm_init(pottery_ohm_t* ohm);

POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_destroy(pottery_ohm_t* ohm);

/**
 *
 * Returns a non-existent ref in case of error.
 */
POTTERY_OPEN_HASH_MAP_EXTERN
pottery_ohm_ref_t pottery_ohm_emplace(pottery_ohm_t* ohm,
        pottery_ohm_key_t key, bool* /*nullable*/ created);

static inline
pottery_ohm_ref_t pottery_ohm_find(pottery_ohm_t* ohm, pottery_ohm_key_t key) {
    return pottery_ohm_table_find(&ohm->table, key);
}

POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_displace(pottery_ohm_t* ohm, pottery_ohm_ref_t ref);

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_remove(pottery_ohm_t* ohm, pottery_ohm_ref_t ref);

/**
 * Returns true if an entry matching the given key was removed.
 */
POTTERY_OPEN_HASH_MAP_EXTERN
bool pottery_ohm_remove_key(pottery_ohm_t* ohm, pottery_ohm_key_t key);
#endif

static inline
bool pottery_ohm_ref_exists(pottery_ohm_t* ohm, pottery_ohm_ref_t ref) {
    return pottery_ohm_table_ref_exists(&ohm->table, ref);
}

static inline
pottery_ohm_key_t pottery_ohm_ref_key(pottery_ohm_t* map, pottery_ohm_ref_t ref) {
    (void)map;
    #ifndef POTTERY_OPEN_HASH_MAP_KEY_FOR_VALUE
        // with no defined key expression, the ref is the key
        return ref;
    #elif defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_MAP_KEY_FOR_VALUE(map->context, ref);
    #else
        return POTTERY_OPEN_HASH_MAP_KEY_FOR_VALUE(ref);
    #endif
}

static inline
bool pottery_ohm_ref_key_equal(pottery_ohm_t* map,
        pottery_ohm_key_t left, pottery_ohm_key_t right)
{
    (void)map;
    #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_MAP_KEY_EQUAL(map->context, left, right);
    #else
        return POTTERY_OPEN_HASH_MAP_KEY_EQUAL(left, right);
    #endif
}

static inline
size_t pottery_ohm_ref_key_hash(pottery_ohm_t* map, pottery_ohm_key_t key) {
    (void)map;
    #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_MAP_KEY_HASH(map->context, key);
    #else
        return POTTERY_OPEN_HASH_MAP_KEY_HASH(key);
    #endif
}

static inline
bool pottery_ohm_ref_is_empty(pottery_ohm_t* map, pottery_ohm_ref_t ref) {
    (void)map;
    #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_MAP_IS_EMPTY(map->context, ref);
    #else
        return POTTERY_OPEN_HASH_MAP_IS_EMPTY(ref);
    #endif
}

static inline
void pottery_ohm_ref_set_empty(pottery_ohm_t* map, pottery_ohm_ref_t ref) {
    (void)map;
    #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        POTTERY_OPEN_HASH_MAP_SET_EMPTY(map->context, ref);
    #else
        POTTERY_OPEN_HASH_MAP_SET_EMPTY(ref);
    #endif
}

#if POTTERY_OPEN_HASH_MAP_TOMBSTONES
static inline
bool pottery_ohm_ref_is_tombstone(pottery_ohm_t* map, pottery_ohm_ref_t ref) {
    (void)map;
    #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_MAP_IS_TOMBSTONE(map->context, ref);
    #else
        return POTTERY_OPEN_HASH_MAP_IS_TOMBSTONE(ref);
    #endif
}

static inline
void pottery_ohm_ref_set_tombstone(pottery_ohm_t* map, pottery_ohm_ref_t ref) {
    (void)map;
    #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        POTTERY_OPEN_HASH_MAP_SET_TOMBSTONE(map->context, ref);
    #else
        POTTERY_OPEN_HASH_MAP_SET_TOMBSTONE(ref);
    #endif
}
#endif

static inline
bool pottery_ohm_ref_is_value(pottery_ohm_t* map, pottery_ohm_ref_t ref) {
    (void)map;

    #ifdef POTTERY_OPEN_HASH_MAP_IS_ELEMENT // TODO rename to IS_VALUE
        #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
            return POTTERY_OPEN_HASH_MAP_IS_ELEMENT(map->context, ref);
        #else
            return POTTERY_OPEN_HASH_MAP_IS_ELEMENT(ref);
        #endif

    #else
        #if POTTERY_OPEN_HASH_MAP_TOMBSTONES
        if (pottery_ohm_ref_is_tombstone(map, ref))
            return false;
        #endif

        return !pottery_ohm_ref_is_empty(map, ref);
    #endif
}

static inline
void pottery_ohm_ref_move(pottery_ohm_t* map, pottery_ohm_ref_t to, pottery_ohm_ref_t from) {
    (void)map;
    pottery_ohm_lifecycle_move(/*TODO context arg*/ to, from);
    // TODO clear metadata
}

static inline
void pottery_ohm_ref_destroy(pottery_ohm_t* map, pottery_ohm_ref_t ref) {
    (void)map;
    pottery_ohm_lifecycle_destroy(/*TODO context arg*/ ref);
}
