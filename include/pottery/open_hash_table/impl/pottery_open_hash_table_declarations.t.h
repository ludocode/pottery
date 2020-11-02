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

#ifndef POTTERY_OPEN_HASH_TABLE_IMPL
#error "This is an internal header. Do not include it."
#endif

/*
 * types
 */

// The value is the concrete type for buckets and for elements in the hash
// table.
#ifdef POTTERY_OPEN_HASH_TABLE_VALUE_TYPE
typedef POTTERY_OPEN_HASH_TABLE_VALUE_TYPE pottery_oht_value_t;
#endif

// The ref is an abstract handle to an element and bucket in the hash table.
#ifdef POTTERY_OPEN_HASH_TABLE_REF_TYPE
typedef POTTERY_OPEN_HASH_TABLE_REF_TYPE pottery_oht_ref_t;
#else
typedef pottery_oht_value_t* pottery_oht_ref_t;
#endif

// The key type is an abstract handle to a key used for lookups in the hash
// table. Each element in the hash table has a unique key.
#ifdef POTTERY_OPEN_HASH_TABLE_KEY_TYPE
typedef POTTERY_OPEN_HASH_TABLE_KEY_TYPE pottery_oht_key_t;
#else
typedef pottery_oht_ref_t pottery_oht_key_t;
#endif

#ifdef POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE
typedef POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE pottery_oht_context_t;
#endif

typedef struct pottery_oht_t {
    #ifdef POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE
    pottery_oht_context_t context;
    #endif

    pottery_oht_ref_t first;
    size_t log_2_size; // power-of-two size (i.e. log_2 of the size)

    size_t count;

    #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
    size_t tombstones;
    #endif
} pottery_oht_t;

/*
 * Helpers to wrap configuration
 */

static inline
void pottery_oht_ref_set_empty(pottery_oht_t* oht, pottery_oht_ref_t ref) {
    (void)oht;
    #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        POTTERY_OPEN_HASH_TABLE_SET_EMPTY(oht->context, ref);
    #else
        POTTERY_OPEN_HASH_TABLE_SET_EMPTY(ref);
    #endif
}

// get the key for a ref
static pottery_always_inline
pottery_oht_key_t pottery_oht_key(pottery_oht_t* oht, pottery_oht_ref_t ref) {
    (void)oht;
    #ifndef POTTERY_OPEN_HASH_TABLE_KEY_FOR_VALUE
        // with no defined key expression, the ref is the key
        return ref;
    #elif defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_TABLE_KEY_FOR_VALUE(oht->context, ref);
    #else
        return POTTERY_OPEN_HASH_TABLE_KEY_FOR_VALUE(ref);
    #endif
}

static pottery_always_inline
pottery_oht_ref_t pottery_oht_access_at(pottery_oht_t* oht, size_t index) {
    // TODO rename to ACCESS_AT or ACCESS_SHIFT maybe
    #ifndef POTTERY_OPEN_HASH_TABLE_ACCESS
        // With no defined access expression, it's a simple array.
        return oht->first + index;
    #elif defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_TABLE_ACCESS(oht->context, oht->first, index);
    #else
        return POTTERY_OPEN_HASH_TABLE_ACCESS(oht->first, index);
    #endif
}

/*
 * Public functions
 */

/**
 * Configures a hash table around an existing array.
 *
 * The array must already be a valid hash table. (To set up a hash table array
 * for the first time, make sure all elements are empty.)
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_configure(pottery_oht_t* oht,
    POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG(context)
    pottery_oht_ref_t first,
    size_t log_2_size,
    size_t count
    #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
    , size_t tombstones
    #endif
) {
    #ifdef POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE
    oht->context = context;
    #endif
    oht->first = first;
    oht->log_2_size = log_2_size;
    oht->count = count;
    #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
    oht->tombstones = tombstones;
    #endif
}

static inline
size_t pottery_oht_count(pottery_oht_t* oht) {
    return oht->count;
}

#if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
static inline
size_t pottery_oht_tombstones(pottery_oht_t* oht) {
    return oht->tombstones;
}
#endif

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
 * This never returns a non-existent ref. The behaviour is undefined if the
 * table is full. It is your responsibility to avoid exceeding your preferred
 * load factor for the table.
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_ref_t pottery_oht_emplace(pottery_oht_t* oht,
        pottery_oht_key_t key, bool* /*nullable*/ created);

#if POTTERY_LIFECYCLE_CAN_PASS && POTTERY_LIFECYCLE_CAN_DESTROY
/**
 * Inserts a value.
 */
pottery_oht_ref_t pottery_oht_insert(pottery_oht_t* oht, pottery_oht_value_t value) {
    bool created;
    pottery_oht_ref_t ref = pottery_oht_emplace(oht, pottery_oht_key(oht, &value), &created);
    if (!created)
        pottery_oht_lifecycle_destroy(ref);
    *ref = value;
    return ref;
}
#endif

/**
 * Finds an entry with the given key.
 *
 * Returns NULL if there is no entry for this key.
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_ref_t pottery_oht_find(pottery_oht_t* oht, pottery_oht_key_t key);

/**
 * Displaces (removes) the given uninitialized entry from the hash table.
 *
 * You must destroy the value (including the key) if necessary before
 * displacing it from the table.
 *
 * If you want the table to destroy the value automatically, call remove()
 * instead.
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_displace(pottery_oht_t* oht, pottery_oht_ref_t ref);

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_remove(pottery_oht_t* oht, pottery_oht_ref_t ref);

/**
 * Returns true if an entry matching the given key was removed.
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
bool pottery_oht_remove_key(pottery_oht_t* oht, pottery_oht_key_t key);
#endif

/**
 * Returns a non-existent ref representing the end of the hash table.
 */
static inline
pottery_oht_ref_t pottery_oht_end(pottery_oht_t* oht) {
    return pottery_oht_access_at(oht, 1 << oht->log_2_size);
}

static inline
bool pottery_oht_ref_exists(pottery_oht_t* oht, pottery_oht_ref_t ref) {
    return ref != pottery_oht_end(oht);
}

/**
 * Returns a ref representing the start of the hash table (the first element
 * if it has any elements, or the end of the hash table otherwise.)
 *
 * If the hast table is empty, the returned ref does not exist, and is equal
 * to pottery_oht_end().
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_ref_t pottery_oht_begin(pottery_oht_t* oht);

/**
 * Returns the first element in the oht.
 *
 * The oht must have at least one element!
 */
static inline
pottery_oht_ref_t pottery_oht_first(pottery_oht_t* oht) {
    pottery_assert(oht->count > 0);
    return pottery_oht_begin(oht);
}

/**
 * Returns the last element in the oht.
 *
 * The oht must have at least one element!
 */
POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_ref_t pottery_oht_last(pottery_oht_t* oht);

POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_next(pottery_oht_t* oht, pottery_oht_ref_t* ref);

POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_previous(pottery_oht_t* oht, pottery_oht_ref_t* ref);
