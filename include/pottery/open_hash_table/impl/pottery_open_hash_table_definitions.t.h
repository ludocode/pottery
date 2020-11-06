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
 * Helpers to wrap configuration
 */

// Gets the index of a ref
static pottery_always_inline
size_t pottery_oht_access_index(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t first,
        pottery_oht_ref_t ref)
{
    #ifndef POTTERY_OPEN_HASH_TABLE_ACCESS_OFFSET
        POTTERY_OPEN_HASH_TABLE_CONTEXT_UNUSED;
        return pottery_cast(size_t, ref - first);
    #elif defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_TABLE_ACCESS_OFFSET(context, first, index);
    #else
        return POTTERY_OPEN_HASH_TABLE_ACCESS_OFFSET(first, index);
    #endif
}

static pottery_always_inline
pottery_oht_ref_t pottery_oht_access_next(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t ref)
{
    POTTERY_OPEN_HASH_TABLE_CONTEXT_UNUSED;
    // TODO configurable access
    return ref + 1;
}

static pottery_always_inline
pottery_oht_ref_t pottery_oht_access_previous(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t ref)
{
    POTTERY_OPEN_HASH_TABLE_CONTEXT_UNUSED;
    // TODO configurable access
    return ref - 1;
}

static inline
size_t pottery_oht_key_hash(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
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
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
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
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
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
                pottery_oht_key_hash(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL key);
            #endif
}
#endif

#if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
static inline
bool pottery_oht_ref_is_tombstone(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t ref)
{
    #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_TABLE_IS_TOMBSTONE(context, ref);
    #else
        return POTTERY_OPEN_HASH_TABLE_IS_TOMBSTONE(ref);
    #endif
}

static inline
void pottery_oht_ref_set_tombstone(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t ref)
{
    #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        POTTERY_OPEN_HASH_TABLE_SET_TOMBSTONE(context, ref);
    #else
        POTTERY_OPEN_HASH_TABLE_SET_TOMBSTONE(ref);
    #endif
}
#endif

static inline
bool pottery_oht_ref_is_empty(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t ref)
{
    #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
        return POTTERY_OPEN_HASH_TABLE_IS_EMPTY(context, ref);
    #else
        return POTTERY_OPEN_HASH_TABLE_IS_EMPTY(ref);
    #endif
}

static inline
bool pottery_oht_ref_is_element(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t ref)
{
    #ifdef POTTERY_OPEN_HASH_TABLE_IS_ELEMENT
        #if defined(POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE)
            return POTTERY_OPEN_HASH_TABLE_IS_ELEMENT(context, ref);
        #else
            return POTTERY_OPEN_HASH_TABLE_IS_ELEMENT(ref);
        #endif

    #else
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        if (pottery_oht_ref_is_tombstone(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref))
            return false;
        #endif

        return !pottery_oht_ref_is_empty(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref);
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
pottery_oht_ref_t pottery_oht_probe(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t first,
        size_t log_2_size,
        pottery_oht_key_t key,
        bool* /*nullable*/ empty_or_tombstone)
{
    size_t hash = pottery_oht_key_hash(
            POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL key);
    size_t bucket = pottery_oht_bucket_for_hash(log_2_size, hash);
    size_t probe = 1;
    #if POTTERY_OPEN_HASH_TABLE_DOUBLE_HASHING
    probe = pottery_oht_key_double_hash_interval(
            POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL key);
    #endif

    size_t index = bucket;

    while (true) {
        //printf("probing at %zi\n", index);
        pottery_oht_ref_t ref = pottery_oht_access_at(
                POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL first, index);

        // see if we found an empty or tombstone bucket
        if (!pottery_oht_ref_is_element(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref)) {
            //printf("found empty or tombstone\n");
            if (empty_or_tombstone != pottery_null)
                *empty_or_tombstone = true;
            return ref;
        }

        // see if we found a matching key
        //printf("comparing keys %s %s %i\n", key, pottery_oht_key(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref), strcmp(key, pottery_oht_key(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref)));
        if (pottery_oht_key_equal(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL key,
                    pottery_oht_key(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref)))
        {
            //printf("found key at %zi\n", index);
            if (empty_or_tombstone != pottery_null)
                *empty_or_tombstone = false;
            return ref;
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
pottery_oht_ref_t pottery_oht_emplace(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t first,
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* tombstones,
        #endif
        pottery_oht_key_t key,
        bool* /*nullable*/ created)
{
    bool empty_or_tombstone;
    pottery_oht_ref_t ref = pottery_oht_probe(
            POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL
            first, log_2_size, key, &empty_or_tombstone);

    #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
    if (tombstones != pottery_null && empty_or_tombstone &&
            pottery_oht_ref_is_tombstone(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref))
        (*tombstones)++;
    #endif

    if (created != pottery_null)
        *created = empty_or_tombstone;
    return ref;
}

POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_ref_t pottery_oht_find(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t first,
        size_t log_2_size,
        pottery_oht_key_t key)
{
    bool empty;
    pottery_oht_ref_t ref = pottery_oht_probe(
            POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL
            first, log_2_size, key, &empty);
    //printf("empty %i\n",empty);
    return empty ? pottery_oht_end(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL
            first, log_2_size) : ref;
}

POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_displace(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t first,
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* /*nullable*/ tombstones,
        #endif
        pottery_oht_ref_t ref)
{
    pottery_assert(!pottery_oht_ref_is_empty(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref));

    #if POTTERY_OPEN_HASH_TABLE_LINEAR_PROBING
    size_t mask = (pottery_cast(size_t, 1) << log_2_size) - 1;
    size_t index = pottery_oht_access_index(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL
            first, ref);



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
    if (!pottery_oht_ref_is_empty(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL
                pottery_oht_access_at(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL previous)))
    {
        if (tombstones != pottery_null)
            (*tombstones)++;
        pottery_oht_ref_set_tombstone(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref);
        return;
    }

    while (true) {
        // set this bucket as empty, and loop around if the next is a tombstone
        pottery_oht_ref_set_empty(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref);
        index = (index + 1) & mask;
        ref = pottery_oht_access_at(
                POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG first, index);
        if (!pottery_oht_ref_is_tombstone(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref))
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

    pottery_oht_ref_set_empty(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref);

    while (true) {
        // look forward for a colliding element
        index = (index + 1) & mask;
        pottery_oht_ref_t test = pottery_oht_access_at(
                POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL first, index);

        // we can stop when we run out of colliding elements
        if (pottery_oht_ref_is_empty(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL test))
            break;

        // see if this element goes in the now empty bucket
        bool empty;
        pottery_oht_ref_t target = pottery_oht_probe(
                POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL
                first,
                log_2_size,
                pottery_oht_key(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL test),
                &empty);
        if (target == ref) {
            (void)empty;
            pottery_assert(empty);

            // this element must be moved back.
            pottery_oht_lifecycle_move(
                    POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref, test);
            pottery_oht_ref_set_empty(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL test);

            // we continue the search with this new empty bucket.
            ref = test;
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

    (void)first;
    (void)log_2_size;

    pottery_oht_ref_set_tombstone(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref);
    if (tombstones != pottery_null)
        (*tombstones)++;

    #else
    #error "Either tombstones or linear probing are required."
    #endif
}

POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_next(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t first,
        size_t log_2_size,
        pottery_oht_ref_t* ref)
{
    pottery_oht_ref_t end = pottery_oht_end(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL first, log_2_size);
    do {
        *ref = pottery_oht_access_next(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL *ref);
    } while (*ref != end && !pottery_oht_ref_is_element(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL *ref));
}

POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_previous(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t first,
        size_t log_2_size,
        pottery_oht_ref_t* ref)
{
    do {
        if (*ref == first) {
            *ref = pottery_oht_end(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL first, log_2_size);
            return;
        }
        *ref = pottery_oht_access_previous(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL *ref);
    } while (!pottery_oht_ref_is_element(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL *ref));
}

POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_ref_t pottery_oht_begin(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t first,
        size_t log_2_size)
{
    pottery_oht_ref_t end = pottery_oht_end(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL first, log_2_size);
    pottery_oht_ref_t begin = first;
    while (end != begin && !pottery_oht_ref_is_element(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL begin))
        begin = pottery_oht_access_next(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL begin);
    return begin;
}

POTTERY_OPEN_HASH_TABLE_EXTERN
pottery_oht_ref_t pottery_oht_last(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t first,
        size_t log_2_size)
{
    pottery_oht_ref_t last = pottery_oht_end(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL first, log_2_size);
    do {
        // There must be at least one element so we shouldn't need to bounds
        // check. We do in debug mode.
        pottery_assert(last != first);
        last = pottery_oht_access_previous(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL last);
    } while (!pottery_oht_ref_is_element(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL last));
    return last;
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_OPEN_HASH_TABLE_EXTERN
void pottery_oht_remove(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t first,
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* /*nullable*/ tombstones,
        #endif
        pottery_oht_ref_t ref)
{
    pottery_oht_lifecycle_destroy(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL ref);
    pottery_oht_displace(
            POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL
            first,
            log_2_size,
            #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
            tombstones,
            #endif
            ref);
}

POTTERY_OPEN_HASH_TABLE_EXTERN
bool pottery_oht_remove_key(
        POTTERY_OPEN_HASH_TABLE_CONTEXT_ARG
        pottery_oht_ref_t first,
        size_t log_2_size,
        #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
        size_t* /*nullable*/ tombstones,
        #endif
        pottery_oht_key_t key)
{
    pottery_oht_ref_t ref = pottery_oht_find(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL
            first, log_2_size, key);

    if (!pottery_oht_ref_exists(POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL
            first, log_2_size, ref))
    {
        //printf("not found\n");
        return false;
    }

    pottery_oht_remove(
            POTTERY_OPEN_HASH_TABLE_CONTEXT_VAL
            first,
            log_2_size,
            #if POTTERY_OPEN_HASH_TABLE_TOMBSTONES
            tombstones,
            #endif
            ref);

    return true;
}
#endif
