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

#ifndef POTTERY_OPEN_HASH_MAP_IMPL
#error "This is an internal header. Do not include it."
#endif

#if POTTERY_OPEN_HASH_MAP_HAS_METADATA
typedef enum pottery_ohm_bucket_state_t {

    // We use zero as the empty state so we can zero-allocate our metadata and
    // not have to clear it.
    #if POTTERY_OPEN_HASH_MAP_INTERNAL_EMPTY
    pottery_ohm_bucket_state_empty = 0,
    #endif

    // This is not necessarily a value since the user might have declared only
    // in-band tombstones and not in-band empty or vice versa.
    pottery_ohm_bucket_state_other = 1,

    #if POTTERY_OPEN_HASH_MAP_INTERNAL_TOMBSTONE
    pottery_ohm_bucket_state_tombstone = 2,
    #endif
} pottery_ohm_bucket_state_t;
#endif

typedef struct pottery_ohm_t pottery_ohm_t;

static inline
void pottery_ohm_entry_move(pottery_ohm_t* map, pottery_ohm_entry_t to, pottery_ohm_entry_t from);

#if POTTERY_OPEN_HASH_MAP_CAN_DESTROY
static inline
void pottery_ohm_entry_destroy(pottery_ohm_t* map, pottery_ohm_entry_t entry);
#endif

static inline
pottery_ohm_key_t pottery_ohm_entry_key(pottery_ohm_t* map, pottery_ohm_entry_t entry) {
    (void)map;
    #if defined(POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE)
        return pottery_ohm_ref_key(map->context, entry);
    #else
        return pottery_ohm_ref_key(entry);
    #endif
}

static inline
bool pottery_ohm_entry_key_equal(pottery_ohm_t* map, pottery_ohm_key_t left, pottery_ohm_key_t right);

static inline
size_t pottery_ohm_entry_key_hash(pottery_ohm_t* map, pottery_ohm_key_t key);

static inline
bool pottery_ohm_entry_is_empty(pottery_ohm_t* map, pottery_ohm_entry_t entry);

static inline
void pottery_ohm_entry_set_empty(pottery_ohm_t* map, pottery_ohm_entry_t entry);

#if POTTERY_OPEN_HASH_MAP_TOMBSTONES
static inline
bool pottery_ohm_entry_is_tombstone(pottery_ohm_t* map, pottery_ohm_entry_t entry);

static inline
void pottery_ohm_entry_set_tombstone(pottery_ohm_t* map, pottery_ohm_entry_t entry);
#endif

static inline
bool pottery_ohm_entry_is_value(pottery_ohm_t* map, pottery_ohm_entry_t entry);

static inline
pottery_ohm_entry_t pottery_ohm_impl_values(pottery_ohm_t* map);

/**
 * Returns the number of buckets currently in the map.
 */
static inline
size_t pottery_ohm_bucket_count(pottery_ohm_t* map);
