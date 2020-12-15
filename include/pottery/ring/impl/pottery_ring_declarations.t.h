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

#ifndef POTTERY_RING_IMPL
#error "This is an internal header. Do not include it."
#endif

#if !POTTERY_LIFECYCLE_CAN_MOVE
#error "A move expression is required so that the ring can grow."
#endif

typedef POTTERY_RING_VALUE_TYPE pottery_ring_value_t;

typedef pottery_ring_value_t* pottery_ring_entry_t;

/**
 * A double-ended queue of values implemented as a circular array.
 *
 * This has nearly the same interface as the paged deque, but it has different
 * performance characteristics and invalidation rules.
 *
 * When the ring changes for any reason (insert, extract, commit, drop, etc.), all
 * pointers to any elements in the ring are invalidated and any ongoing
 * emplace or access is invalidated at both ends. This is because the ring
 * may shrink or grow which may move all elements. This is different from the
 * paged deque; see the documentation on deque for more details.
 *
 * The ring is (by default) growable. When the ring needs to grow, all
 * values are copied.
 */
typedef struct pottery_ring_t {
    pottery_ring_value_t* values;
    size_t capacity;
    size_t start;
    size_t count;

    //#if POTTERY_RING_INTERNAL_CAPACITY > 0
    // TODO this doesn't work yet. it should probably be in a union with capacity like vector
    //pottery_ring_value_t internal[POTTERY_RING_INTERNAL_CAPACITY];
    //#endif
} pottery_ring_t;

static inline void pottery_ring_sanity_check(pottery_ring_t* ring) {
    pottery_assert((ring->capacity == 0) == (ring->values == pottery_null));
}

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_RING_EXTERN
void pottery_ring_init(pottery_ring_t* ring);
POTTERY_RING_EXTERN
void pottery_ring_destroy(pottery_ring_t* ring);
#endif

static inline
size_t pottery_ring_count(pottery_ring_t* ring) {
    return ring->count;
}

static inline
bool pottery_ring_is_empty(pottery_ring_t* ring) {
    return ring->count == 0;
}

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_RING_EXTERN
void pottery_ring_displace_all(pottery_ring_t* ring);

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_RING_EXTERN
void pottery_ring_remove_all(pottery_ring_t* ring);
#endif

POTTERY_RING_EXTERN
pottery_error_t pottery_ring_emplace_first(pottery_ring_t* ring, pottery_ring_entry_t* entry);
POTTERY_RING_EXTERN
pottery_error_t pottery_ring_emplace_last(pottery_ring_t* ring, pottery_ring_entry_t* entry);
#endif

#if POTTERY_LIFECYCLE_CAN_PASS

// TODO add C++ const ref / r-value ref helpers like vector

// C by value
// (This might still be used in C++ if POTTERY_VECTOR_CXX is disabled so we
// still need to properly run constructors.)

static inline
pottery_error_t pottery_ring_insert_first(pottery_ring_t* ring, pottery_ring_value_t value) {
    pottery_ring_entry_t entry;
    pottery_error_t error = pottery_ring_emplace_first(ring, &entry);
    if (error == POTTERY_OK)
        pottery_move_construct(pottery_ring_value_t, *entry, value);
    return error;
}

static inline
pottery_error_t pottery_ring_insert_last(pottery_ring_t* ring, pottery_ring_value_t value) {
    pottery_ring_entry_t entry;
    pottery_error_t error = pottery_ring_emplace_last(ring, &entry);
    if (error == POTTERY_OK)
        pottery_move_construct(pottery_ring_value_t, *entry, value);
    return error;
}
#endif

static inline
pottery_ring_value_t* pottery_ring_at(pottery_ring_t* ring, size_t index) {
    pottery_ring_sanity_check(ring);
    pottery_assert(index < pottery_ring_count(ring));

    size_t offset = (ring->start + index) % ring->capacity;
    //fprintf(stderr, "at index %i offset %i\n", (int)index, (int)offset);
    return ring->values + offset;
}

static inline
pottery_ring_value_t* pottery_ring_select(pottery_ring_t* ring, size_t index) {
    if (index == pottery_ring_count(ring))
        return pottery_null;
    return pottery_ring_at(ring, index);
}

static inline
size_t pottery_ring_index(pottery_ring_t* ring, pottery_ring_value_t* value) {
    size_t offset = pottery_cast(size_t, value - ring->values);
    return (offset + ring->capacity - ring->start) % ring->capacity;
}

static inline
pottery_ring_value_t* pottery_ring_first(pottery_ring_t* ring) {
    pottery_assert(!pottery_ring_is_empty(ring));
    return ring->values + ring->start;
}

static inline
pottery_ring_value_t* pottery_ring_last(pottery_ring_t* ring) {
    pottery_assert(!pottery_ring_is_empty(ring));
    return pottery_ring_at(ring, ring->count - 1);
}

static inline
pottery_ring_value_t* pottery_ring_begin(pottery_ring_t* ring) {
    if (ring->count == 0)
        return pottery_null;
    return ring->values + ring->start;
}

static inline
pottery_ring_value_t* pottery_ring_end(pottery_ring_t* ring) {
    // Ring uses null as its end entry.
    (void)ring;
    return pottery_null;
}

static inline
bool pottery_ring_entry_exists(pottery_ring_t* ring, pottery_ring_value_t* entry) {
    (void)ring;
    return entry != pottery_null;
}

static inline
pottery_ring_value_t* pottery_ring_entry_value(pottery_ring_t* ring, pottery_ring_value_t* entry) {
    (void)ring;
    return entry;
}

static inline
pottery_ring_value_t* pottery_ring_next(pottery_ring_t* ring, pottery_ring_value_t* entry) {
    if (entry == pottery_ring_last(ring))
        return pottery_null;
    ++entry;
    if (entry == ring->values + ring->capacity)
        entry = ring->values;
    return entry;
}

static inline
pottery_ring_value_t* pottery_ring_previous(pottery_ring_t* ring, pottery_ring_value_t* entry) {
    // It is an error to call this on the first element.
    pottery_assert(entry != pottery_ring_first(ring));
    if (entry == pottery_null)
        return pottery_ring_last(ring);

    if (entry == ring->values)
        entry = ring->values + ring->capacity - 1;
    else
        --entry;

    return entry;
}

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_RING_EXTERN
void pottery_ring_displace_first(pottery_ring_t* ring);
POTTERY_RING_EXTERN
void pottery_ring_displace_last(pottery_ring_t* ring);

POTTERY_RING_EXTERN
void pottery_ring_displace_first_bulk(pottery_ring_t* ring, size_t count);
POTTERY_RING_EXTERN
void pottery_ring_displace_last_bulk(pottery_ring_t* ring, size_t count);
#endif

#if POTTERY_LIFECYCLE_CAN_DESTROY
static inline
void pottery_ring_remove_first(pottery_ring_t* ring) {
    pottery_ring_lifecycle_destroy(pottery_ring_first(ring));
    pottery_ring_displace_first(ring);
}

static inline
void pottery_ring_remove_last(pottery_ring_t* ring) {
    pottery_ring_lifecycle_destroy(pottery_ring_last(ring));
    pottery_ring_displace_last(ring);
}

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_RING_EXTERN
void pottery_ring_remove_first_bulk(pottery_ring_t* ring, size_t count);

POTTERY_RING_EXTERN
void pottery_ring_remove_last_bulk(pottery_ring_t* ring, size_t count);
#endif
#endif

#if POTTERY_LIFECYCLE_CAN_PASS
static inline
pottery_ring_value_t pottery_ring_extract_first(pottery_ring_t* ring) {
    pottery_ring_value_t* entry = pottery_ring_first(ring);
    pottery_ring_value_t element = pottery_move_if_cxx(*entry);
    #ifdef __cplusplus
    // see pottery_vector_extract() for details
    entry->~pottery_ring_value_t();
    #endif
    pottery_ring_displace_first(ring);
    return element;
}

static inline
pottery_ring_value_t pottery_ring_extract_last(pottery_ring_t* ring) {
    pottery_ring_value_t* entry = pottery_ring_last(ring);
    pottery_ring_value_t element = pottery_move_if_cxx(*entry);
    #ifdef __cplusplus
    // see pottery_vector_extract() for details
    entry->~pottery_ring_value_t();
    #endif
    pottery_ring_displace_last(ring);
    return element;
}

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_RING_EXTERN
void pottery_ring_insert_last_bulk(pottery_ring_t* ring, const pottery_ring_value_t* values, size_t count);

/**
 * Pushes each element from the given array in reverse order into the front of
 * the ring. (The last element of the given array will end up at the front
 * of the ring.)
 *
 * This is the opposite operation of pottery_ring_extract_first_bulk();
 * inserting an array of bulk data and then extracting it again is functionally
 * idempotent.
 */
POTTERY_RING_EXTERN
void pottery_ring_insert_first_bulk(pottery_ring_t* ring, const pottery_ring_value_t* values, size_t count);

POTTERY_RING_EXTERN
void pottery_ring_extract_first_bulk(pottery_ring_t* ring, pottery_ring_value_t* values, size_t count);

/**
 * Pops elements from the end of the ring one-by-one in into the given array
 * in reverse order. (The element at the end of the ring will be placed at the
 * start of the array.)
 *
 * This is the opposite operation of pottery_ring_insert_last_bulk(); inserting
 * an array of bulk data and then extracting it again is functionally
 * idempotent.
 */
POTTERY_RING_EXTERN
void pottery_ring_extract_last_bulk(pottery_ring_t* ring, pottery_ring_value_t* values, size_t count);
#endif
#endif
