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

#ifndef POTTERY_OPEN_HASH_TABLE_IMPL
#error "This is an internal header. Do not include it."
#endif

/*
 * types
 */

// The value is the concrete type for buckets and for elements in the table.
#ifdef POTTERY_OPEN_HASH_TABLE_VALUE_TYPE
typedef POTTERY_OPEN_HASH_TABLE_VALUE_TYPE pottery_oht_value_t;
#endif

// The ref is a pointer to a value or an abstract reference in the table.
#ifdef POTTERY_OPEN_HASH_TABLE_REF_TYPE
typedef POTTERY_OPEN_HASH_TABLE_REF_TYPE pottery_oht_ref_t;
#else
typedef pottery_oht_value_t* pottery_oht_ref_t;
#endif

// The entry is a handle to an entry in the generalized array of buckets.
#ifdef POTTERY_OPEN_HASH_TABLE_ENTRY_TYPE
typedef POTTERY_OPEN_HASH_TABLE_ENTRY_TYPE pottery_oht_entry_t;
#else
typedef pottery_oht_ref_t pottery_oht_entry_t;
#endif

// The key type is an abstract handle to a key used for lookups in the hash
// table. Each element in the hash table has a unique key. If a key type is not
// given, it's the same as the ref type (making this a set.)
#ifdef POTTERY_OPEN_HASH_TABLE_KEY_TYPE
typedef POTTERY_OPEN_HASH_TABLE_KEY_TYPE pottery_oht_key_t;
#else
typedef pottery_oht_ref_t pottery_oht_key_t;
#endif

#ifdef POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE
typedef POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE pottery_oht_context_t;
#endif

/*
 * Helpers to wrap configuration
 */

static inline
void pottery_oht_entry_set_empty(
        POTTERY_OPEN_HASH_TABLE_ARGS
        pottery_oht_entry_t entry)
{
    POTTERY_OPEN_HASH_TABLE_ARGS_UNUSED;

    #ifdef POTTERY_OPEN_HASH_TABLE_SET_EMPTY
        #ifdef POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE
            POTTERY_OPEN_HASH_TABLE_SET_EMPTY(context, entry);
        #else
            POTTERY_OPEN_HASH_TABLE_SET_EMPTY(entry);
        #endif
    #elif POTTERY_OPEN_HASH_TABLE_EMPTY_IS_ZERO
        *entry = 0;
    #else
        #error "Template configuration error!"
    #endif
}

// get the key for a entry
static pottery_always_inline
pottery_oht_key_t pottery_oht_key(
        POTTERY_OPEN_HASH_TABLE_ARGS
        pottery_oht_entry_t entry)
{
    POTTERY_OPEN_HASH_TABLE_ARGS_UNUSED;

    #ifndef POTTERY_OPEN_HASH_TABLE_REF_KEY
        // With no defined key expression, the ref is the key
        return pottery_oht_array_access_ref(POTTERY_OPEN_HASH_TABLE_VALS entry);
    #elif defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_TABLE_REF_KEY(context, entry);
    #else
        return POTTERY_OPEN_HASH_TABLE_REF_KEY(entry);
    #endif
}

/*
 * Public functions
 */

/**
 * Ensures there is an entry for the given key, adding it if necessary and
 * returning it.
 *
 * If an entry with the given key already existed, it will already be
 * initialized, and created will be set to false. If you want to replace the
 * entry, you must destroy it first.
 *
 * If an entry is created, it will be uninitialized, and created will be set to
 * true. You must initialize the value (including the key) on the entry before
 * calling any other open hash table functions.
 *
 * This never returns a non-existent entry. The behaviour is undefined if the
 * table is full. It is your responsibility to avoid exceeding your preferred
 * load factor for the table.
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_emplace(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* /*nullable*/ tombstones,
        #endif
        pottery_oht_key_t key,
        bool* /*nullable*/ created);

#if POTTERY_LIFECYCLE_CAN_PASS && POTTERY_LIFECYCLE_CAN_DESTROY
/**
 * Inserts a value.
 */
static inline
pottery_oht_entry_t pottery_oht_insert(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* /*nullable*/ tombstones,
        #endif
        pottery_oht_value_t value)
{
    bool created;

    pottery_oht_entry_t entry = pottery_oht_emplace(
            POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size,
            #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
            tombstones,
            #endif
            pottery_oht_key(POTTERY_OPEN_HASH_TABLE_VALS &value),
            &created);

    if (!created) {
        // destroy the existing value
        pottery_oht_lifecycle_destroy(entry);
    }

    pottery_move_construct(pottery_oht_value_t, *entry, value);
    return entry;
}
#endif

/**
 * Finds an entry with the given key.
 *
 * Returns NULL if there is no entry for this key.
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_find(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        pottery_oht_key_t key);

/**
 * Displaces (removes without destroying) the given uninitialized entry from
 * the hash table.
 *
 * You must destroy the value (including the key) if necessary before
 * displacing it from the table.
 *
 * If you want the table to destroy the value automatically, call remove()
 * instead.
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_displace(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* /*nullable*/ tombstones,
        #endif
        pottery_oht_entry_t entry);

/**
 * Displaces all elements from the hash table.
 *
 * This also sweeps the hash table of tombstones (if applicable). The hash
 * table is effectively cleared; all buckets will be empty.
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_displace_all(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size);

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_remove(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* /*nullable*/ tombstones,
        #endif
        pottery_oht_entry_t entry);

/**
 * Returns true if an entry matching the given key was removed.
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
bool pottery_oht_remove_key(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* /*nullable*/ tombstones,
        #endif
        pottery_oht_key_t key);

/**
 * Removes (destroying) all elements in the hash table.
 *
 * This also sweeps the hash table of tombstones (if applicable). The hash
 * table is effectively cleared (after destroying); all buckets will be empty.
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_remove_all(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size);

/**
 * Destroys all elements in the hash table.
 *
 * The hash table is effectively corrupt after calling this since it still
 * contains entries, but the keys for those entries are uninitialized. Use this
 * as a precursor to discarding or re-initializing the table.
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_destroy_all(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size);
#endif

#if POTTERY_LIFECYCLE_CAN_PASS
// TODO extract
#endif

/**
 * Returns a non-existent entry representing the end of the hash table.
 */
static inline
pottery_oht_entry_t pottery_oht_end(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size)
{
    // Open Hash Table uses the end entry of the array as its end entry. (We
    // have to select() it manually if the array access template doesn't
    // support end().)
    #if POTTERY_ARRAY_ACCESS_INHERENT_COUNT
    (void)log_2_size;
    return pottery_oht_array_access_end(POTTERY_OPEN_HASH_TABLE_SOLE_VALS);
    #else
    return pottery_oht_array_access_select(
            POTTERY_OPEN_HASH_TABLE_VALS
            pottery_cast(size_t, 1) << log_2_size);
    #endif
}

static inline
bool pottery_oht_entry_exists(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        pottery_oht_entry_t entry)
{
    return entry != pottery_oht_end(POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size);
}

static inline
bool pottery_oht_contains_key(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        pottery_oht_key_t key)
{
    return pottery_oht_entry_exists(
            POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size,
            pottery_oht_find(
                    POTTERY_OPEN_HASH_TABLE_VALS
                    log_2_size,
                    key));
}

/**
 * Returns a entry representing the start of the hash table (the first element
 * if it has any elements, or the end of the hash table otherwise.)
 *
 * If the hash table is empty, the returned entry does not exist, and is equal
 * to pottery_oht_end().
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_begin(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size);

/**
 * Returns the first element in the table.
 *
 * The table must have at least one element!
 */
static inline
pottery_oht_entry_t pottery_oht_first(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size)
{
    pottery_oht_entry_t entry = pottery_oht_begin(POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size);
    pottery_assert(entry != pottery_oht_end(POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size));
    return entry;
}

/**
 * Returns the last element in the table.
 *
 * The table must have at least one element!
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_last(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size);

POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_next(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        pottery_oht_entry_t entry);

POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_previous(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        pottery_oht_entry_t entry);
