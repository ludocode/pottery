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

// Default to 16 buckets
#define POTTERY_OPEN_HASH_MAP_MINIMUM_LOG_2_SIZE 4

// Allocate an array of buckets
// If an empty bucket is zero, this allocates a zeroed array so that it doesn't
// need to be cleared.
static inline
pottery_ohm_value_t* pottery_ohm_impl_alloc(pottery_ohm_t* map, size_t size) {
    (void)map;
    void* ptr =
            #if POTTERY_OPEN_HASH_MAP_EMPTY_IS_ZERO
            pottery_ohm_alloc_malloc_array_zero
            #else
            pottery_ohm_alloc_malloc_array
            #endif
                (POTTERY_OPEN_HASH_MAP_CONTEXT_VAL
                pottery_alignof(pottery_ohm_value_t),
                size,
                sizeof(pottery_ohm_value_t));
    return pottery_cast(pottery_ohm_value_t*, ptr);
}

// Free an array of buckets
static inline
void pottery_ohm_impl_free(pottery_ohm_t* map, pottery_ohm_value_t* values) {
    (void)map;
    pottery_ohm_alloc_free(
            POTTERY_OPEN_HASH_MAP_CONTEXT_VAL
            pottery_alignof(pottery_ohm_value_t),
            values);
}

#if POTTERY_OPEN_HASH_MAP_HAS_METADATA
// Allocate an array of metadata
static inline
uint8_t* pottery_ohm_impl_alloc_metadata(pottery_ohm_t* map, size_t size) {
    (void)map;
    void* ptr =
            #if POTTERY_OPEN_HASH_MAP_INTERNAL_EMPTY
            pottery_ohm_alloc_malloc_array_zero
            #else
            pottery_ohm_alloc_malloc_array
            #endif
                (POTTERY_OPEN_HASH_MAP_CONTEXT_VAL
                pottery_alignof(uint8_t),
                size,
                sizeof(uint8_t));
    return pottery_cast(uint8_t*, ptr);
}

// Free an array of metadata
static inline
void pottery_ohm_impl_free_metadata(pottery_ohm_t* map, uint8_t* metadata) {
    (void)map;
    pottery_ohm_alloc_free(
            POTTERY_OPEN_HASH_MAP_CONTEXT_VAL
            pottery_alignof(uint8_t),
            metadata);
}
#endif

static
pottery_error_t pottery_ohm_init_impl(pottery_ohm_t* map,
        #ifdef POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE
        pottery_ohm_context_t context,
        #endif
        size_t log_2_size)
{
    size_t size = pottery_cast(size_t, 1) << log_2_size;

    // Allocate values
    map->values = pottery_ohm_impl_alloc(map, size);
    if (map->values == pottery_null)
        return POTTERY_ERROR_ALLOC;

    // Allocate metadata
    #if POTTERY_OPEN_HASH_MAP_HAS_METADATA
    map->metadata = pottery_ohm_impl_alloc_metadata(map, size);
    if (map->metadata == pottery_null) {
        pottery_ohm_impl_free(map, map->values);
        return POTTERY_ERROR_ALLOC;
    }
    #endif

    // Configure the table
    #ifdef POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE
    map->context = context;
    #endif
    map->log_2_size = log_2_size;
    map->count = 0;
    #if POTTERY_OPEN_HASH_MAP_TOMBSTONES
    map->tombstones = 0;
    #endif

    #if !POTTERY_OPEN_HASH_MAP_EMPTY_IS_ZERO && !POTTERY_OPEN_HASH_MAP_INTERNAL_EMPTY
    // Clear the new values
    size_t i;
    for (i = 0; i < size; ++i)
        pottery_ohm_table_ref_set_empty(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
                map->values + i);
    #endif

    return POTTERY_OK;
}

POTTERY_OPEN_HASH_MAP_EXTERN
pottery_error_t pottery_ohm_init(pottery_ohm_t* map
        #ifdef POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE
        , pottery_ohm_context_t context
        #endif
) {
    return pottery_ohm_init_impl(map,
            POTTERY_OPEN_HASH_MAP_CONTEXT_VAL
            POTTERY_OPEN_HASH_MAP_MINIMUM_LOG_2_SIZE);
}

static
void pottery_ohm_impl_free_allocs(pottery_ohm_t* map) {
    pottery_ohm_impl_free(map, map->values);
    #if POTTERY_OPEN_HASH_MAP_HAS_METADATA
    pottery_ohm_alloc_free(POTTERY_OPEN_HASH_MAP_CONTEXT_VAL 1, map->metadata);
    #endif
}

POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_destroy(pottery_ohm_t* map) {

    #if POTTERY_LIFECYCLE_CAN_DESTROY
    pottery_ohm_table_destroy_all(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size);
    #else
    // If we don't have a destroy expression, you must manually empty the hash
    // map before destroying it.
    pottery_assert(pottery_ohm_count(map) == 0);
    #endif

    pottery_ohm_impl_free_allocs(map);
}

// Re-hashes the table, possibly resizing in the process.
pottery_noinline static
pottery_error_t pottery_rehash(pottery_ohm_t* map, size_t new_log_2_size) {

    // Allocate a new map with the new size
    pottery_ohm_t new_map;
    pottery_error_t error = pottery_ohm_init_impl(&new_map,
            #ifdef POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE
            map->context,
            #endif
            new_log_2_size);
    if (error != POTTERY_OK)
        return error;

    // Replace our map, saving the old values
    pottery_ohm_t old_map = *map;
    *map = new_map;

    // Migrate all values from old table into new one
    pottery_ohm_ref_t source = pottery_ohm_begin(&old_map);
    while (pottery_ohm_ref_exists(&old_map, source)) {
        //printf("migrating bucket %zi\n", source - old_map.values);

        pottery_ohm_ref_t target = pottery_ohm_table_emplace(
                POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
                map->values,
                map->log_2_size,
                #if POTTERY_OPEN_HASH_MAP_TOMBSTONES
                pottery_null,
                #endif
                pottery_ohm_table_key(
                        POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(&old_map)
                        source),
                pottery_null);

        pottery_ohm_lifecycle_move(
                POTTERY_OPEN_HASH_MAP_CONTEXT_VAL
                target, source);

        #if POTTERY_OPEN_HASH_MAP_HAS_METADATA
        pottery_ohm_ref_set_other(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map) target);
        #endif
        ++map->count;

        pottery_ohm_next(&old_map, &source);
    }

    // Free the old map
    pottery_ohm_impl_free_allocs(&old_map);
    return POTTERY_OK;
}

static inline
pottery_error_t pottery_ohm_grow_if_needed(pottery_ohm_t* map) {
    if (pottery_ohm_load(map) <= pottery_ohm_capacity(map)) {
        // no need to grow
        return POTTERY_OK;
    }

    // If the table is full of tombstones, we don't grow; we just rehash to
    // the same size. Otherwise we double in size.
    size_t new_log_2_size = map->log_2_size + (
        #if POTTERY_OPEN_HASH_MAP_TOMBSTONES
        (map->tombstones > map->count) ? 0 :
        #endif
        1);

    return pottery_rehash(map, new_log_2_size);
}

static inline
void pottery_ohm_shrink_if_needed(pottery_ohm_t* map) {

    // We shrink by half if the count is less than a quarter of our capacity.
    if (map->log_2_size <= POTTERY_OPEN_HASH_MAP_MINIMUM_LOG_2_SIZE ||
            pottery_ohm_count(map) > pottery_ohm_capacity(map) / 4) {
        // no need to shrink
        return;
    }

    // Note: we ignore the return value here. If allocating the smaller table
    // fails, we just keep our larger table, this way displace can't fail.
    (void)pottery_rehash(map, map->log_2_size - 1);
}

POTTERY_OPEN_HASH_MAP_EXTERN
pottery_error_t pottery_ohm_emplace(pottery_ohm_t* map, pottery_ohm_key_t key,
        pottery_ohm_ref_t* ref, bool* /*nullable*/ created)
{
    // If we're full we grow regardless of whether the element already exists
    // in order to make sure there's enough room for it.
    pottery_error_t error = pottery_ohm_grow_if_needed(map);
    if (error != POTTERY_OK)
        return error;

    *ref = pottery_ohm_table_emplace(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size,
            #if POTTERY_OPEN_HASH_MAP_TOMBSTONES
            &map->tombstones,
            #endif
            key,
            created);

    #if POTTERY_OPEN_HASH_MAP_HAS_METADATA
    pottery_ohm_ref_set_other(POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map) *ref);
    #endif

    ++map->count;
    return POTTERY_OK;
}

POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_displace(pottery_ohm_t* map, pottery_ohm_ref_t ref) {
    pottery_ohm_table_displace(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size,
            #if POTTERY_OPEN_HASH_MAP_TOMBSTONES
            &map->tombstones,
            #endif
            ref);
    --map->count;
    pottery_ohm_shrink_if_needed(map);
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_remove(pottery_ohm_t* map, pottery_ohm_ref_t ref) {
    pottery_ohm_table_remove(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size,
            #if POTTERY_OPEN_HASH_MAP_TOMBSTONES
            &map->tombstones,
            #endif
            ref);
    --map->count;
    pottery_ohm_shrink_if_needed(map);
}

POTTERY_OPEN_HASH_MAP_EXTERN
bool pottery_ohm_remove_key(pottery_ohm_t* map, pottery_ohm_key_t key) {
    bool removed = pottery_ohm_table_remove_key(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size,
            #if POTTERY_OPEN_HASH_MAP_TOMBSTONES
            &map->tombstones,
            #endif
            key);
    if (removed) {
        --map->count;
        pottery_ohm_shrink_if_needed(map);
    }
    return removed;
}
#endif

POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_displace_all(pottery_ohm_t* map) {
    // If the size is larger than the minimum, we re-allocate to the minimum
    // size; otherwise we clear and keep the existing allocation.

    // (Eventually it might be nice if open_hash_map didn't need to allocate
    // memory if it has no elements, like vector does. If/when we implement
    // this later, this should just free and null the buckets instead.)

    if (map->log_2_size == POTTERY_OPEN_HASH_MAP_MINIMUM_LOG_2_SIZE) {
        pottery_ohm_table_displace_all(
                POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
                map->values,
                map->log_2_size);
        return;
    }

    // Save the old allocations in case re-allocating fails. In case of
    // failure, we just keep the old allocations.
    pottery_ohm_t old_map = *map;

    if (pottery_unlikely(POTTERY_OK != pottery_ohm_init_impl(map,
            POTTERY_OPEN_HASH_MAP_CONTEXT_VAL
            POTTERY_OPEN_HASH_MAP_MINIMUM_LOG_2_SIZE)))
    {
        *map = old_map;
        pottery_ohm_table_displace_all(
                POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
                map->values,
                map->log_2_size);
        return;
    }

    pottery_ohm_impl_free_allocs(&old_map);
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_remove_all(pottery_ohm_t* map) {
    // destroy_all() followed by displace_all() could iterate twice over the
    // whole map if it doesn't resize. This will only happen if allocation
    // fails or if it's the minimum size, and in both cases performance is
    // irrelevant.
    pottery_ohm_table_destroy_all(
            POTTERY_OPEN_HASH_MAP_TABLE_CONTEXT_VAL(map)
            map->values,
            map->log_2_size);
    pottery_ohm_displace_all(map);
}
#endif
