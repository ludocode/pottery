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

#if POTTERY_LIFECYCLE_CAN_DESTROY
static void pottery_ring_impl_destroy_all(pottery_ring_t* ring) {
    pottery_ring_entry_t entry = pottery_ring_begin(ring);
    while (pottery_ring_entry_exists(ring, entry)) {
        pottery_ring_lifecycle_destroy(entry);
        entry = pottery_ring_next(ring, entry);
    }
}
#endif

POTTERY_RING_EXTERN
void pottery_ring_init(pottery_ring_t* ring) {
    ring->values = pottery_null;
    ring->capacity = 0;
    ring->start = 0;
    ring->count = 0;
}

POTTERY_RING_EXTERN
void pottery_ring_destroy(pottery_ring_t* ring) {

    #if POTTERY_LIFECYCLE_CAN_DESTROY
    pottery_ring_impl_destroy_all(ring);
    #else
    // Without a destroy expression it is an error to destroy a non-empty
    // container.
    pottery_assert(ring->count == 0);
    #endif

    if (ring->values != pottery_null)
        pottery_ring_alloc_free(pottery_alignof(pottery_ring_value_t), ring->values);
}

POTTERY_RING_EXTERN
void pottery_ring_displace_all(pottery_ring_t* ring) {
    // TODO: reset to internal/external storage, etc
    ring->count = 0;
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_RING_EXTERN
void pottery_ring_remove_all(pottery_ring_t* ring) {
    pottery_ring_impl_destroy_all(ring);
    pottery_ring_displace_all(ring);
}

POTTERY_RING_EXTERN
void pottery_ring_remove_first_bulk(pottery_ring_t* ring, size_t count) {
    // TODO performance
    size_t i;
    for (i = 0; i < count; ++i)
        pottery_ring_remove_first(ring);
}

POTTERY_RING_EXTERN
void pottery_ring_remove_last_bulk(pottery_ring_t* ring, size_t count) {
    // TODO performance
    size_t i;
    for (i = 0; i < count; ++i)
        pottery_ring_remove_last(ring);
}
#endif

POTTERY_RING_EXTERN
void pottery_ring_displace_first(pottery_ring_t* ring) {
    //fprintf(stderr, "displace first\n");
    pottery_ring_sanity_check(ring);
    pottery_assert(!pottery_ring_is_empty(ring));

    ring->start = (ring->start + 1) % ring->capacity;
    --ring->count;

    // TODO shrink
}

POTTERY_RING_EXTERN
void pottery_ring_displace_last(pottery_ring_t* ring) {
    //fprintf(stderr, "displace last\n");
    pottery_ring_sanity_check(ring);
    pottery_assert(!pottery_ring_is_empty(ring));

    --ring->count;

    // TODO shrink
}

POTTERY_RING_EXTERN
void pottery_ring_displace_first_bulk(pottery_ring_t* ring, size_t count) {
    // TODO performance
    size_t i;
    for (i = 0; i < count; ++i)
        pottery_ring_displace_first(ring);
}

POTTERY_RING_EXTERN
void pottery_ring_displace_last_bulk(pottery_ring_t* ring, size_t count) {
    // TODO performance
    size_t i;
    for (i = 0; i < count; ++i)
        pottery_ring_displace_last(ring);
}

static pottery_error_t pottery_ring_grow(pottery_ring_t* ring, size_t minimum_capacity) {
    pottery_ring_sanity_check(ring);

    // TODO should actually use minimum capacity?
    (void)minimum_capacity;

    size_t new_capacity = 4;
    while (new_capacity < (2 * ring->count)) {
        new_capacity *= 2;
    }

    // We limit the overall size so we don't need to do overflow checks.
    if (new_capacity > SIZE_MAX / 4)
        return POTTERY_ERROR_ALLOC;

    //fprintf(stderr, "grow newcap %i\n", (int)new_capacity);

    // Note that we aren't using at_least. We'll be changing ring to use
    // power-of-two sizes so it can mask instead of modulus.

    pottery_ring_value_t* new_values = pottery_cast(pottery_ring_value_t*,
            pottery_ring_alloc_malloc_array(
                pottery_alignof(pottery_ring_value_t),
                new_capacity,
                sizeof(pottery_ring_value_t)));
    if (new_values == pottery_null)
        return POTTERY_ERROR_ALLOC;

    if (ring->count > 0) {
        size_t part1 = ring->capacity - ring->start;
        if (part1 >= ring->count) {
            //fprintf(stderr, "start %i cap %i count %i part1 %i\n", (int)ring->start, (int)ring->capacity, (int)ring->count, (int)part1);
            pottery_ring_lifecycle_move_bulk_restrict(new_values, ring->values + ring->start, ring->count);
        } else {
            size_t part2 = ring->count - part1;
            pottery_ring_lifecycle_move_bulk_restrict(new_values, ring->values + ring->start, part1);
            pottery_ring_lifecycle_move_bulk_restrict(new_values + part1, ring->values, part2);
        }
    }

    if (ring->values != pottery_null)
        pottery_ring_alloc_free(pottery_alignof(pottery_ring_value_t), ring->values);
    ring->values = new_values;
    ring->capacity = new_capacity;
    ring->start = 0;

    pottery_ring_sanity_check(ring);

    return POTTERY_OK;
}

POTTERY_RING_EXTERN
pottery_error_t pottery_ring_emplace_last(pottery_ring_t* ring, pottery_ring_entry_t* entry) {
    pottery_ring_sanity_check(ring);
    //fprintf(stderr, "insert last\n");

    size_t new_count = ring->count + 1;
    if (ring->capacity < new_count) {
        pottery_error_t error = pottery_ring_grow(ring, new_count);
        if (error != POTTERY_OK)
            return error;
    }

    ring->count = new_count;
    *entry = pottery_ring_at(ring, new_count - 1);
    pottery_ring_sanity_check(ring);
    return POTTERY_OK;
}

POTTERY_RING_EXTERN
pottery_error_t pottery_ring_emplace_first(pottery_ring_t* ring, pottery_ring_entry_t* entry) {
    pottery_ring_sanity_check(ring);
    //fprintf(stderr, "insert first\n");

    size_t new_count = ring->count + 1;
    if (ring->capacity < new_count) {
        pottery_error_t error = pottery_ring_grow(ring, new_count);
        if (error != POTTERY_OK)
            return error;
    }

    ring->count = new_count;
    ring->start = (ring->start + ring->capacity - 1) % ring->capacity;
    *entry = pottery_ring_at(ring, 0);
    pottery_ring_sanity_check(ring);
    return POTTERY_OK;
}

#if POTTERY_LIFECYCLE_CAN_PASS
POTTERY_RING_EXTERN
void pottery_ring_insert_last_bulk(pottery_ring_t* ring, const pottery_ring_value_t* values, size_t count) {
    pottery_ring_sanity_check(ring);
    // TODO performance
    size_t i = 0;
    for (; i < count; ++i)
        pottery_ring_insert_last(ring, values[i]);
    pottery_ring_sanity_check(ring);
}

POTTERY_RING_EXTERN
void pottery_ring_insert_first_bulk(pottery_ring_t* ring, const pottery_ring_value_t* values, size_t count) {
    pottery_ring_sanity_check(ring);
    // TODO performance
    size_t i;
    for (i = 0; i < count; ++i)
        pottery_ring_insert_first(ring, values[count - i - 1]);
    pottery_ring_sanity_check(ring);
}

POTTERY_RING_EXTERN
void pottery_ring_extract_first_bulk(pottery_ring_t* ring, pottery_ring_value_t* values, size_t count) {
    pottery_ring_sanity_check(ring);
    size_t i;
    for (i = 0; i < count; ++i)
        values[i] = pottery_ring_extract_first(ring);
    pottery_ring_sanity_check(ring);
}

POTTERY_RING_EXTERN
void pottery_ring_extract_last_bulk(pottery_ring_t* ring, pottery_ring_value_t* values, size_t count) {
    //fprintf(stderr, "extract last bulk\n");
    pottery_ring_sanity_check(ring);
    size_t i;
    for (i = 0; i < count; ++i)
        values[count - i - 1] = pottery_ring_extract_last(ring);
    pottery_ring_sanity_check(ring);
}
#endif
