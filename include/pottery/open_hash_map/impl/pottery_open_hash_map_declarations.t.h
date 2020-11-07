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
    #ifdef POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE
    pottery_ohm_context_t context;
    #endif

    pottery_ohm_value_t* values;
    size_t log_2_size;

    size_t count;
    #if POTTERY_OPEN_HASH_MAP_TOMBSTONES
    size_t tombstones;
    #endif

    #if POTTERY_OPEN_HASH_MAP_HAS_METADATA
    // For simplicity we store metadata as one byte per bucket. We could
    // optimize this later to use the minimum number of bits. In the meantime
    // if you really care about performance you should use in-band values for
    // empty and tombstones.
    uint8_t* metadata;
    #endif
};

POTTERY_OPEN_HASH_MAP_EXTERN
pottery_error_t pottery_ohm_init(pottery_ohm_t* map
        #ifdef POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE
        , pottery_ohm_context_t context
        #endif
        );

POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_destroy(pottery_ohm_t* map);

/**
 *
 */
POTTERY_OPEN_HASH_MAP_EXTERN
pottery_error_t pottery_ohm_emplace(pottery_ohm_t* map, pottery_ohm_key_t key,
        pottery_ohm_ref_t* ref, bool* /*nullable*/ created);

static inline
size_t pottery_ohm_count(pottery_ohm_t* map) {
    return map->count;
}

static inline
size_t pottery_ohm_load(pottery_ohm_t* map) {
    return map->count
            #if POTTERY_OPEN_HASH_MAP_TOMBSTONES
            + map->tombstones
            #endif
            ;
}

/**
 * Returns the current capacity of the map, i.e. the bucket count times the
 * load factor.
 */
static inline
size_t pottery_ohm_capacity(pottery_ohm_t* map) {
    size_t size = pottery_cast(size_t, 1) << map->log_2_size;

    // Currently the max load factor is 4/7 (~57%). This should be
    // configurable, and we should benchmark this to choose a reasonable
    // default.
    size_t divisor = 7;
    return size - (divisor / 2) * (size / divisor);
}

static inline
bool pottery_ohm_is_empty(pottery_ohm_t* map) {
    return pottery_ohm_count(map) == 0;
}

static inline
pottery_ohm_ref_t pottery_ohm_find(pottery_ohm_t* map, pottery_ohm_key_t key) {
    return pottery_ohm_table_find(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size,
            key);
}

POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_displace(pottery_ohm_t* map, pottery_ohm_ref_t ref);

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_remove(pottery_ohm_t* map, pottery_ohm_ref_t ref);

/**
 * Returns true if an entry matching the given key was removed.
 */
POTTERY_OPEN_HASH_MAP_EXTERN
bool pottery_ohm_remove_key(pottery_ohm_t* map, pottery_ohm_key_t key);
#endif

/**
 * Returns true if the ref exists.
 */
static inline
bool pottery_ohm_ref_exists(pottery_ohm_t* map, pottery_ohm_ref_t ref) {
    return pottery_ohm_table_ref_exists(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size,
            ref);
}

/**
 * Returns true if there is a value in the map with this key.
 */
static inline
bool pottery_ohm_contains_key(pottery_ohm_t* map, pottery_ohm_key_t key) {
    return pottery_ohm_table_contains_key(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size,
            key);
}

static inline
pottery_ohm_key_t pottery_ohm_ref_key(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_ref_t ref)
{
    POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED;
    #ifndef POTTERY_OPEN_HASH_MAP_VALUE_KEY
        // with no defined key expression, the ref is the key
        return ref;
    #elif defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_MAP_VALUE_KEY(map->context, ref);
    #else
        return POTTERY_OPEN_HASH_MAP_VALUE_KEY(ref);
    #endif
}

static inline
bool pottery_ohm_ref_key_equal(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_key_t left, pottery_ohm_key_t right)
{
    POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED;
    #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_MAP_KEY_EQUAL(map->context, left, right);
    #else
        return POTTERY_OPEN_HASH_MAP_KEY_EQUAL(left, right);
    #endif
}

static inline
size_t pottery_ohm_ref_key_hash(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_key_t key)
{
    POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED;
    #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_MAP_KEY_HASH(map->context, key);
    #else
        return POTTERY_OPEN_HASH_MAP_KEY_HASH(key);
    #endif
}

#ifdef POTTERY_OPEN_HASH_TABLE_KEY_DOUBLE_HASH
static inline
size_t pottery_ohm_ref_key_double_hash(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_key_t key)
{
    POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED;
    #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_MAP_KEY_DOUBLE_HASH(map->context, key);
    #else
        return POTTERY_OPEN_HASH_MAP_KEY_DOUBLE_HASH(key);
    #endif
}
#endif

#if POTTERY_OPEN_HASH_MAP_HAS_METADATA
// Gets a pointer to the metadata byte for a ref in the map.
static inline
uint8_t* pottery_ohm_ref_metadata(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_ref_t ref)
{
    POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED;
    return map->metadata + pottery_cast(size_t, ref - map->values);
}

static inline
void pottery_ohm_ref_set_other(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_ref_t ref)
{
    *pottery_ohm_ref_metadata(map, ref) =
            pottery_cast(uint8_t, pottery_ohm_bucket_state_other);
}
#endif

static inline
bool pottery_ohm_ref_is_empty(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_ref_t ref)
{
    POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED;
    #if POTTERY_OPEN_HASH_MAP_INTERNAL_EMPTY
        return *pottery_ohm_ref_metadata(map, ref) ==
                pottery_cast(uint8_t, pottery_ohm_bucket_state_empty);
    #elif defined(POTTERY_OPEN_HASH_MAP_IS_EMPTY)
        #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
            return POTTERY_OPEN_HASH_MAP_IS_EMPTY(map->context, ref);
        #else
            return POTTERY_OPEN_HASH_MAP_IS_EMPTY(ref);
        #endif
    #elif POTTERY_OPEN_HASH_MAP_EMPTY_IS_ZERO
        // This requires that the value is comparable to 0 with ==. If it's
        // e.g. a struct, you should specify both EMPTY_IS_ZERO to allow the
        // map to allocated it zeroed and IS_EMPTY to test some internal struct
        // bit to see if it's zero.
        return *ref == 0;
    #else
        #error "No way to test whether a bucket is empty!"
    #endif
}

static inline
void pottery_ohm_ref_set_empty(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_ref_t ref)
{
    POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED;
    #if POTTERY_OPEN_HASH_MAP_INTERNAL_EMPTY
        *pottery_ohm_ref_metadata(map, ref) =
                pottery_cast(uint8_t, pottery_ohm_bucket_state_empty);
    #elif defined(POTTERY_OPEN_HASH_MAP_SET_EMPTY)
        #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
            POTTERY_OPEN_HASH_MAP_SET_EMPTY(map->context, ref);
        #else
            POTTERY_OPEN_HASH_MAP_SET_EMPTY(ref);
        #endif
    #elif POTTERY_OPEN_HASH_MAP_EMPTY_IS_ZERO
        // This requires that the value is assignable to 0 with =. If it's e.g.
        // a struct, you should specify both EMPTY_IS_ZERO to allow the map to
        // allocated it zeroed and IS_EMPTY to test some internal struct bit to
        // see if it's zero.
        *ref = 0;
    #else
        #error "No way to set an empty bucket!"
    #endif
}

#if POTTERY_OPEN_HASH_MAP_TOMBSTONES
static inline
bool pottery_ohm_ref_is_tombstone(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_ref_t ref)
{
    POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED;
    #if POTTERY_OPEN_HASH_MAP_INTERNAL_TOMBSTONE
        return *pottery_ohm_ref_metadata(map, ref) ==
                pottery_cast(uint8_t, pottery_ohm_bucket_state_tombstone);
    #else
        #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
            return POTTERY_OPEN_HASH_MAP_IS_TOMBSTONE(map->context, ref);
        #else
            return POTTERY_OPEN_HASH_MAP_IS_TOMBSTONE(ref);
        #endif
    #endif
}

static inline
void pottery_ohm_ref_set_tombstone(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_ref_t ref)
{
    POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED;
    #if POTTERY_OPEN_HASH_MAP_INTERNAL_TOMBSTONE
        *pottery_ohm_ref_metadata(map, ref) =
                pottery_cast(uint8_t, pottery_ohm_bucket_state_tombstone);
    #else
        #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
            POTTERY_OPEN_HASH_MAP_SET_TOMBSTONE(map->context, ref);
        #else
            POTTERY_OPEN_HASH_MAP_SET_TOMBSTONE(ref);
        #endif
    #endif
}
#endif

static inline
bool pottery_ohm_ref_is_value(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_ref_t ref)
{
    POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED;

    // If we have metadata, we check it first. We can't rely on it exclusively
    // because the user might have configured empty or tombstones in-band.
    #if POTTERY_OPEN_HASH_MAP_HAS_METADATA
    if (*pottery_ohm_ref_metadata(map, ref) !=
            pottery_cast(uint8_t, pottery_ohm_bucket_state_other))
        return false;

    // We can't use IS_VALUE if we have metadata because that means something
    // is not in-band: the user can't possibly tell whether something is empty
    // or a tombstone if we're storing that ourselves.
    #elif defined(POTTERY_OPEN_HASH_MAP_IS_VALUE)
        #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
            return POTTERY_OPEN_HASH_MAP_IS_VALUE(map->context, ref);
        #else
            return POTTERY_OPEN_HASH_MAP_IS_VALUE(ref);
        #endif

    #endif

    // If the user has defined in-band empty or tombstones, we won't have those
    // stored in our metadata so check them here.
    #if POTTERY_OPEN_HASH_MAP_TOMBSTONES && !POTTERY_OPEN_HASH_MAP_INTERNAL_TOMBSTONE
        if (pottery_ohm_ref_is_tombstone(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map) ref))
            return false;
    #endif
    #if !POTTERY_OPEN_HASH_MAP_INTERNAL_EMPTY
    if (pottery_ohm_ref_is_empty(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map) ref))
        return false;
    #endif

    // Otherwise it's a value.
    return true;
}

static inline
void pottery_ohm_ref_move(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_ref_t to, pottery_ohm_ref_t from)
{
    pottery_ohm_lifecycle_move(
            #ifdef POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE
            map->context,
            #endif
            to,
            from);

    #if POTTERY_OPEN_HASH_MAP_HAS_METADATA
    *pottery_ohm_ref_metadata(map, to) =
            pottery_cast(uint8_t, pottery_ohm_bucket_state_other);
    #endif
}

static inline
void pottery_ohm_ref_destroy(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_ARG
        pottery_ohm_ref_t ref)
{
    POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_UNUSED;

    pottery_ohm_lifecycle_destroy(
            #ifdef POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE
            map->context,
            #endif
            ref);
}

static inline
pottery_ohm_ref_t pottery_ohm_begin(pottery_ohm_t* map) {
    return pottery_ohm_table_begin(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size);
}

static inline
pottery_ohm_ref_t pottery_ohm_end(pottery_ohm_t* map) {
    return pottery_ohm_table_end(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size);
}

static inline
pottery_ohm_ref_t pottery_ohm_first(pottery_ohm_t* map) {
    return pottery_ohm_table_first(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size);
}

static inline
pottery_ohm_ref_t pottery_ohm_last(pottery_ohm_t* map) {
    return pottery_ohm_table_last(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size);
}

static inline
void pottery_ohm_next(pottery_ohm_t* map, pottery_ohm_ref_t* ref) {
    pottery_ohm_table_next(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size,
            ref);
}

static inline
void pottery_ohm_previous(pottery_ohm_t* map, pottery_ohm_ref_t* ref) {
    pottery_ohm_table_previous(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size,
            ref);
}
