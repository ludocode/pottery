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
 * Helpers to wrap configuration
 */

static inline
size_t pottery_oht_key_hash(
        POTTERY_OPEN_HASH_TABLE_ARGS
        pottery_oht_key_t key)
{
    #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_TABLE_KEY_HASH(context, key);
    #else
        return POTTERY_OPEN_HASH_TABLE_KEY_HASH(key);
    #endif
}

static inline
size_t pottery_oht_key_equal(
        POTTERY_OPEN_HASH_TABLE_ARGS
        pottery_oht_key_t left,
        pottery_oht_key_t right)
{
    #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_TABLE_KEY_EQUAL(context, left, right);
    #else
        return POTTERY_OPEN_HASH_TABLE_KEY_EQUAL(left, right);
    #endif
}

/**
 * Returns the probing sequence interval of the given key for double hashing.
 */
#if POTTERY_OPEN_HASH_TABLE_DOUBLE_HASHING
static inline
size_t pottery_oht_key_double_hash_interval(
        POTTERY_OPEN_HASH_TABLE_ARGS
        pottery_oht_key_t key)
{
    // If the user hasn't given us a double hashing function, we just use the
    // same hash function, but we're using it differently since we're not
    // passing it through a Knuth multiplicative hash. We force the low bit on
    // to make it relatively prime with the size of the table.
    // (This means the hash function is unnecessarily re-computed. This can be
    // fixed later.)
    return 1 |
            #ifdef POTTERY_OPEN_HASH_TABLE_KEY_DOUBLE_HASH
                #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
                    POTTERY_OPEN_HASH_TABLE_KEY_DOUBLE_HASH(context, key);
                #else
                    POTTERY_OPEN_HASH_TABLE_KEY_DOUBLE_HASH(key);
                #endif
            #else
                pottery_oht_key_hash(POTTERY_OPEN_HASH_TABLE_VALS key);
            #endif
}
#endif

#if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
static inline
bool pottery_oht_entry_is_tombstone(
        POTTERY_OPEN_HASH_TABLE_ARGS
        pottery_oht_entry_t entry)
{
    #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_TABLE_IS_TOMBSTONE(context, entry);
    #else
        return POTTERY_OPEN_HASH_TABLE_IS_TOMBSTONE(entry);
    #endif
}

static inline
void pottery_oht_entry_set_tombstone(
        POTTERY_OPEN_HASH_TABLE_ARGS
        pottery_oht_entry_t entry)
{
    #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        POTTERY_OPEN_HASH_TABLE_SET_TOMBSTONE(context, entry);
    #else
        POTTERY_OPEN_HASH_TABLE_SET_TOMBSTONE(entry);
    #endif
}
#endif

static inline
bool pottery_oht_entry_is_empty(
        POTTERY_OPEN_HASH_TABLE_ARGS
        pottery_oht_entry_t entry)
{
    #ifdef POTTERY_OPEN_HASH_TABLE_IS_EMPTY
        #ifdef POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE
            return POTTERY_OPEN_HASH_TABLE_IS_EMPTY(context, entry);
        #else
            return POTTERY_OPEN_HASH_TABLE_IS_EMPTY(entry);
        #endif
    #elif POTTERY_OPEN_HASH_TABLE_EMPTY_IS_ZERO
        return *entry == 0;
    #else
        #error "Template configuration error!"
    #endif
}

static inline
bool pottery_oht_entry_is_element(
        POTTERY_OPEN_HASH_TABLE_ARGS
        pottery_oht_entry_t entry)
{
    #ifdef POTTERY_OPEN_HASH_TABLE_IS_VALUE
        #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
            return POTTERY_OPEN_HASH_TABLE_IS_VALUE(context, entry);
        #else
            return POTTERY_OPEN_HASH_TABLE_IS_VALUE(entry);
        #endif

    #else
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        if (pottery_oht_entry_is_tombstone(POTTERY_OPEN_HASH_TABLE_VALS entry))
            return false;
        #endif

        return !pottery_oht_entry_is_empty(POTTERY_OPEN_HASH_TABLE_VALS entry);
    #endif
}

/*
 * Functions
 */

static inline
size_t pottery_oht_bucket_for_hash(size_t log_2_size, size_t hash) {
    //printf("re-hashed %zi to %zi\n", hash, pottery_knuth_hash_s(hash, log_2_size));
    return pottery_knuth_hash_s(hash, log_2_size);
}

static inline
size_t pottery_oht_next_probe(size_t log_2_size, size_t index, size_t probe) {
    pottery_assert(probe > 0);

    // Note that we don't check the simple additions below for overflow. We
    // assume the table and, in the case of double hashing, the probing
    // interval cannot be larger than half of SIZE_MAX.

    #if POTTERY_OPEN_HASH_TABLE_LINEAR_PROBING
        (void)probe;
        size_t next = index + 1;

    #elif POTTERY_OPEN_HASH_TABLE_QUADRATIC_PROBING

        // Triangular numbers: 1, 3, 6, 10, ... (A000217)
        // These are guaranteed to cycle over all buckets in the table since our
        // table is power-of-two sized.

        // This is 1/2*probe + 1/2*probe^2, with proper fractions and overflow checks
        size_t probe_total;
        if (pottery_mul_overflow_s(probe, probe + 1, &probe_total))
            pottery_abort();
        probe_total /= 2;

        // More overflow checks
        size_t next = index + probe_total;
        if (next < index)
            pottery_abort();

    #elif POTTERY_OPEN_HASH_TABLE_DOUBLE_HASHING
        // Probe is the double hashing interval
        size_t next = index + probe;

    #else
        #error "No probing sequence defined"
    #endif

    size_t mask = (pottery_cast(size_t, 1) << log_2_size) - 1;
    return next & mask;
}

/**
 * Probes the hash table looking for either an element with the given key or an
 * empty or tombstone bucket in which it can be inserted.
 */
static
pottery_oht_entry_t pottery_oht_probe(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        pottery_oht_key_t key,
        bool* /*nullable*/ empty_or_tombstone)
{
    size_t hash = pottery_oht_key_hash(
            POTTERY_OPEN_HASH_TABLE_VALS key);
    size_t bucket = pottery_oht_bucket_for_hash(log_2_size, hash);
    size_t probe = 1;
    #if POTTERY_OPEN_HASH_TABLE_DOUBLE_HASHING
    probe = pottery_oht_key_double_hash_interval(
            POTTERY_OPEN_HASH_TABLE_VALS key);
    #endif

    size_t index = bucket;

    while (true) {
        //printf("probing at %zi\n", index);
        pottery_oht_entry_t entry = pottery_oht_array_access_select(
                POTTERY_OPEN_HASH_TABLE_VALS index);

        // see if we found an empty or tombstone bucket
        if (!pottery_oht_entry_is_element(POTTERY_OPEN_HASH_TABLE_VALS entry)) {
            //printf("found empty or tombstone\n");
            if (empty_or_tombstone != pottery_null)
                *empty_or_tombstone = true;
            return entry;
        }

        // see if we found a matching key
        //printf("comparing keys %s %s %i\n", key, pottery_oht_key(POTTERY_OPEN_HASH_TABLE_VALS entry), strcmp(key, pottery_oht_key(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL entry)));
        if (pottery_oht_key_equal(POTTERY_OPEN_HASH_TABLE_VALS key,
                    pottery_oht_key(POTTERY_OPEN_HASH_TABLE_VALS entry)))
        {
            //printf("found key at %zi\n", index);
            if (empty_or_tombstone != pottery_null)
                *empty_or_tombstone = false;
            return entry;
        }

        // found a collision with a non-matching entry. keep probing.
        //printf("found collision at %zi\n", index);
        index = pottery_oht_next_probe(log_2_size, index, probe);
        #if !POTTERY_OPEN_HASH_TABLE_DOUBLE_HASHING
        ++probe;
        #endif
        pottery_assert(index != bucket); // assert against full table
    }
}

POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_emplace_key(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* tombstones,
        #endif
        pottery_oht_key_t key,
        bool* /*nullable*/ created)
{
    bool empty_or_tombstone;
    pottery_oht_entry_t entry = pottery_oht_probe(
            POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size, key, &empty_or_tombstone);

    #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
    if (tombstones != pottery_null && empty_or_tombstone &&
            pottery_oht_entry_is_tombstone(POTTERY_OPEN_HASH_TABLE_VALS entry))
        (*tombstones)++;
    #endif

    if (created != pottery_null)
        *created = empty_or_tombstone;
    return entry;
}

POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_find(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        pottery_oht_key_t key)
{
    bool empty;
    pottery_oht_entry_t entry = pottery_oht_probe(
            POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size, key, &empty);
    //printf("empty %i\n",empty);
    return empty ? pottery_oht_end(POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size) : entry;
}

POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_displace(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* /*nullable*/ tombstones,
        #endif
        pottery_oht_entry_t entry)
{
    pottery_assert(!pottery_oht_entry_is_empty(POTTERY_OPEN_HASH_TABLE_VALS entry));

    #if POTTERY_OPEN_HASH_TABLE_LINEAR_PROBING
    size_t mask = (pottery_cast(size_t, 1) << log_2_size) - 1;
    size_t index = pottery_oht_array_access_index(POTTERY_OPEN_HASH_TABLE_VALS
            entry);



    #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES

    /*
     * Linear probing with tombstones
     */

    // In linear probing, if this element is after another element or
    // tombstone, we have to make it a tombstone to prevent breaking a
    // collision sequence. But if it's after an empty bucket, we know we
    // aren't part of a collision sequence, so we can clear it and any
    // subsequent tombstones.

    size_t previous = (index + mask) & mask;
    if (!pottery_oht_entry_is_empty(POTTERY_OPEN_HASH_TABLE_VALS
                pottery_oht_array_access_select(POTTERY_OPEN_HASH_TABLE_VALS previous)))
    {
        if (tombstones != pottery_null)
            (*tombstones)++;
        pottery_oht_entry_set_tombstone(POTTERY_OPEN_HASH_TABLE_VALS entry);
        return;
    }

    while (true) {
        // set this bucket as empty, and loop around if the next is a tombstone
        pottery_oht_entry_set_empty(POTTERY_OPEN_HASH_TABLE_VALS entry);
        index = (index + 1) & mask;
        entry = pottery_oht_array_access_select(
                POTTERY_OPEN_HASH_TABLE_ARGS index);
        if (!pottery_oht_entry_is_tombstone(POTTERY_OPEN_HASH_TABLE_VALS entry))
            break;
        if (tombstones != pottery_null)
            --tombstones;
    }



    #else

    /*
     * Linear probing without tombstones
     */

    // To remove from a linear probed hash table without tombstones, we can't
    // just clear the bucket; we need to look forward to see if there is a
    // colliding element that should be in this bucket and, if so, move it
    // back. Such a move creates a new empty bucket, so we need to continue the
    // search again until we run out of collisions to test.

    pottery_oht_entry_set_empty(POTTERY_OPEN_HASH_TABLE_VALS entry);

    while (true) {
        // look forward for a colliding element
        index = (index + 1) & mask;
        pottery_oht_entry_t test = pottery_oht_array_access_select(
                POTTERY_OPEN_HASH_TABLE_VALS index);

        // we can stop when we run out of colliding elements
        if (pottery_oht_entry_is_empty(POTTERY_OPEN_HASH_TABLE_VALS test))
            break;

        // see if this element goes in the now empty bucket
        bool empty;
        pottery_oht_entry_t target = pottery_oht_probe(
                POTTERY_OPEN_HASH_TABLE_VALS
                log_2_size,
                pottery_oht_key(POTTERY_OPEN_HASH_TABLE_VALS test),
                &empty);
        if (target == entry) {
            (void)empty;
            pottery_assert(empty);

            // this element must be moved back.
            pottery_oht_lifecycle_move_restrict(
                    POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL entry, test);
            pottery_oht_entry_set_empty(POTTERY_OPEN_HASH_TABLE_VALS test);

            // we continue the search with this new empty bucket.
            entry = test;
        }
    }
    #endif



    #elif POTTERY_OPEN_HASH_TABLE_TOMBSTONES

    /*
     * Non-linear probing with tombstones
     */

    // With non-linear probing modes there's no way to remove without adding a
    // tombstone because we don't know in what collision sequences we may be
    // taking part.

    (void)log_2_size;

    pottery_oht_entry_set_tombstone(POTTERY_OPEN_HASH_TABLE_VALS entry);
    if (tombstones != pottery_null)
        (*tombstones)++;

    #else
    #error "Either tombstones or linear probing are required."
    #endif
}

POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_next(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        pottery_oht_entry_t entry)
{
    pottery_oht_entry_t end = pottery_oht_end(POTTERY_OPEN_HASH_TABLE_VALS log_2_size);

    do {
        // It's undefined behaviour to call next() on the end of the table, so
        // we don't check it before calling next() on the array.
        // array_access_next() should at least assert that this is not the end
        // of the array.
        entry = pottery_oht_array_access_next(POTTERY_OPEN_HASH_TABLE_VALS entry);
    } while (!pottery_oht_array_access_equal(POTTERY_OPEN_HASH_TABLE_VALS entry, end) &&
            !pottery_oht_entry_is_element(POTTERY_OPEN_HASH_TABLE_VALS entry));

    return entry;
}

POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_previous(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        pottery_oht_entry_t entry)
{
    (void)log_2_size;

    do {
        // It's undefined behaviour to call previous() on the first element of
        // the array. If this happens, we'll (eventually) end up calling
        // previous on the base entry (the beginning of the array.)
        // array_access_previous() should at least assert that this is not the
        // base (begin) of the array.
        entry = pottery_oht_array_access_previous(POTTERY_OPEN_HASH_TABLE_VALS entry);
    } while (!pottery_oht_entry_is_element(POTTERY_OPEN_HASH_TABLE_VALS entry));

    return entry;
}

POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_begin(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size)
{
    #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
    pottery_oht_entry_t base = pottery_oht_array_access_begin(
            POTTERY_OPEN_HASH_TABLE_SOLE_VALS);
    #endif
    pottery_oht_entry_t end = pottery_oht_end(POTTERY_OPEN_HASH_TABLE_VALS log_2_size);
    pottery_oht_entry_t begin = base;
    while (!pottery_oht_array_access_equal(POTTERY_OPEN_HASH_TABLE_VALS begin, end) &&
            !pottery_oht_entry_is_element(POTTERY_OPEN_HASH_TABLE_VALS begin))
        begin = pottery_oht_array_access_next(POTTERY_OPEN_HASH_TABLE_VALS begin);
    return begin;
}

POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_entry_t pottery_oht_last(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size)
{
    #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
    pottery_oht_entry_t base = pottery_oht_array_access_begin(
            POTTERY_OPEN_HASH_TABLE_SOLE_VALS);
    #endif
    (void)base;

    pottery_oht_entry_t last = pottery_oht_end(POTTERY_OPEN_HASH_TABLE_VALS log_2_size);
    do {
        // There must be at least one element so we shouldn't need to bounds
        // check. We do in debug mode.
        pottery_assert(last != base);
        last = pottery_oht_array_access_previous(POTTERY_OPEN_HASH_TABLE_VALS last);
    } while (!pottery_oht_entry_is_element(POTTERY_OPEN_HASH_TABLE_VALS last));
    return last;
}

POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_displace_all(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size)
{
    #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
    pottery_oht_entry_t base = pottery_oht_array_access_begin(
            POTTERY_OPEN_HASH_TABLE_SOLE_VALS);
    #endif

    // It might be possible to optimize this to a simple call to memset() for
    // zero sentinels, but there's a long list of conditions. Here are some:
    //
    // - EMPTY_IS_ZERO needs to be 1, obviously;
    // - SET_ZERO needs to be undefined, since otherwise only a small part of
    //   the struct might need to be zero, and memset() would be slower;
    // - __cplusplus needs to be undefined, since we might have to run a custom
    //   assignment operator with side effects when assigning to 0;
    // - All ARRAY_ACCESS options need to be undefined (the array_access
    //   template needs default behaviour), otherwise the array might not be
    //   contiguous in memory;
    // - REF_TYPE needs to be undefined, since otherwise it might not even be a
    //   pointer
    //
    // This is not worth doing right now.

    size_t size = pottery_cast(size_t, 1) << log_2_size;
    size_t i;
    pottery_oht_entry_t entry = base;
    for (i = 0; i < size; ++i) {
        pottery_oht_entry_set_empty(POTTERY_OPEN_HASH_TABLE_VALS entry);
        entry = pottery_oht_array_access_next(POTTERY_OPEN_HASH_TABLE_VALS entry);
    }
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_destroy_all(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size)
{
    #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
    pottery_oht_entry_t base = pottery_oht_array_access_begin(
            POTTERY_OPEN_HASH_TABLE_SOLE_VALS);
    #endif

    size_t size = pottery_cast(size_t, 1) << log_2_size;
    size_t i;
    pottery_oht_entry_t entry = base;
    for (i = 0; i < size; ++i) {
        if (pottery_oht_entry_is_element(POTTERY_OPEN_HASH_TABLE_VALS entry))
            pottery_oht_lifecycle_destroy(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL entry);
        entry = pottery_oht_array_access_next(POTTERY_OPEN_HASH_TABLE_VALS entry);
    }
}

POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_remove_all(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size)
{
    #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
    pottery_oht_entry_t base = pottery_oht_array_access_begin(
            POTTERY_OPEN_HASH_TABLE_SOLE_VALS);
    #endif

    size_t size = pottery_cast(size_t, 1) << log_2_size;
    size_t i;
    pottery_oht_entry_t entry = base;
    for (i = 0; i < size; ++i) {
        if (pottery_oht_entry_is_element(POTTERY_OPEN_HASH_TABLE_VALS entry))
            pottery_oht_lifecycle_destroy(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL entry);
        // Clear it even if it wasn't an element since it might be a tombstone
        pottery_oht_entry_set_empty(POTTERY_OPEN_HASH_TABLE_VALS entry);
        entry = pottery_oht_array_access_next(POTTERY_OPEN_HASH_TABLE_VALS entry);
    }
}

POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_remove(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* /*nullable*/ tombstones,
        #endif
        pottery_oht_entry_t entry)
{
    pottery_oht_lifecycle_destroy(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL entry);
    pottery_oht_displace(
            POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size,
            #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
            tombstones,
            #endif
            entry);
}

POTTERY_OPEN_HASH_TABLE_EXTERN
bool pottery_oht_remove_key(
        POTTERY_OPEN_HASH_TABLE_ARGS
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* /*nullable*/ tombstones,
        #endif
        pottery_oht_key_t key)
{
    pottery_oht_entry_t entry = pottery_oht_find(POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size, key);

    if (!pottery_oht_entry_exists(POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size, entry))
    {
        //printf("not found\n");
        return false;
    }

    pottery_oht_remove(
            POTTERY_OPEN_HASH_TABLE_VALS
            log_2_size,
            #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
            tombstones,
            #endif
            entry);

    return true;
}
#endif
