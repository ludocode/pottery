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

#ifndef POTTERY_DEQUE_IMPL
#error "This is an internal header. Do not include it."
#endif

/**
 * A paged double-ended queue.
 *
 * See docs/containers/deque.md
 */
struct pottery_deque_t {
    pottery_deque_page_ring_t pages;
    pottery_deque_page_t spare;
    size_t first_page_start;
    size_t last_page_end;
};

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_DEQUE_EXTERN
void pottery_deque_init(pottery_deque_t* deque);

POTTERY_DEQUE_EXTERN
void pottery_deque_destroy(pottery_deque_t* deque);

/**
 * Returns the number of elements in the deque.
 *
 * @note If you just want to check if the deque is empty,
 *       pottery_deque_is_empty() is significantly faster than comparing the
 *       count to 0.
 */
POTTERY_DEQUE_EXTERN
size_t pottery_deque_count(pottery_deque_t* deque);
#endif

static inline
bool pottery_deque_is_empty(pottery_deque_t* deque) {
    // The deque never contains empty pages (not counting spares), so the deque
    // is empty if the page ring is empty.
    return pottery_deque_page_ring_is_empty(&deque->pages);
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
#if POTTERY_FORWARD_DECLARATIONS
POTTERY_DEQUE_EXTERN
void pottery_deque_remove_all(pottery_deque_t* deque);

POTTERY_DEQUE_EXTERN
void pottery_deque_remove_first_bulk(pottery_deque_t* deque, size_t count);

POTTERY_DEQUE_EXTERN
void pottery_deque_remove_last_bulk(pottery_deque_t* deque, size_t count);
#endif

static inline
void pottery_deque_remove_first(pottery_deque_t* deque) {
    pottery_deque_remove_first_bulk(deque, 1);
}

static inline
void pottery_deque_remove_last(pottery_deque_t* deque) {
    pottery_deque_remove_last_bulk(deque, 1);
}
#endif

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_DEQUE_EXTERN
void pottery_deque_displace_all(pottery_deque_t* deque);

POTTERY_DEQUE_EXTERN
void pottery_deque_displace_first_bulk(pottery_deque_t* deque, size_t count);

POTTERY_DEQUE_EXTERN
void pottery_deque_displace_last_bulk(pottery_deque_t* deque, size_t count);
#endif

static inline
void pottery_deque_displace_first(pottery_deque_t* deque) {
    pottery_deque_displace_first_bulk(deque, 1);
}

static inline
void pottery_deque_displace_last(pottery_deque_t* deque) {
    pottery_deque_displace_last_bulk(deque, 1);
}

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_DEQUE_EXTERN
pottery_error_t pottery_deque_emplace_first_bulk(pottery_deque_t* deque, pottery_deque_entry_t* entry, size_t count);

POTTERY_DEQUE_EXTERN
pottery_error_t pottery_deque_emplace_last_bulk(pottery_deque_t* deque, pottery_deque_entry_t* entry, size_t count);
#endif

static inline
pottery_error_t pottery_deque_emplace_last(pottery_deque_t* deque, pottery_deque_entry_t* entry) {
    return pottery_deque_emplace_last_bulk(deque, entry, 1);
}

static inline
pottery_error_t pottery_deque_emplace_first(pottery_deque_t* deque, pottery_deque_entry_t* entry) {
    return pottery_deque_emplace_first_bulk(deque, entry, 1);
}

#if POTTERY_LIFECYCLE_CAN_PASS

// TODO add C++ const ref / r-value ref helpers like vector

// C by value
// (This might still be used in C++ if POTTERY_VECTOR_CXX is disabled so we
// still need to properly run constructors.)

static inline
pottery_error_t pottery_deque_insert_last(pottery_deque_t* deque, pottery_deque_value_t value) {
    pottery_deque_entry_t entry;
    pottery_error_t error = pottery_deque_emplace_last(deque, &entry);
    if (error != POTTERY_OK)
        return error;
    pottery_move_construct(pottery_deque_value_t, *pottery_deque_entry_value(deque, entry), value);
    return POTTERY_OK;
}

static inline
pottery_error_t pottery_deque_insert_first(pottery_deque_t* deque, pottery_deque_value_t value) {
    pottery_deque_entry_t entry;
    pottery_error_t error = pottery_deque_emplace_first(deque, &entry);
    if (error != POTTERY_OK)
        return error;
    pottery_move_construct(pottery_deque_value_t, *pottery_deque_entry_value(deque, entry), value);
    return POTTERY_OK;
}
#endif

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_DEQUE_EXTERN
pottery_deque_entry_t pottery_deque_select(pottery_deque_t* deque, size_t index);

POTTERY_DEQUE_EXTERN
size_t pottery_deque_index(pottery_deque_t* deque, pottery_deque_entry_t entry);
#endif

static inline
pottery_deque_entry_t pottery_deque_at(pottery_deque_t* deque, size_t index) {
    pottery_assert(index < pottery_deque_count(deque));
    return pottery_deque_select(deque, index);
}

static inline
pottery_deque_entry_t pottery_deque_first(pottery_deque_t* deque) {
    pottery_assert(!pottery_deque_is_empty(deque));
    pottery_deque_page_t* page = pottery_deque_page_ring_first(&deque->pages);
    return pottery_deque_entry_make(page, *page + deque->first_page_start);
}

static inline
pottery_deque_entry_t pottery_deque_last(pottery_deque_t* deque) {
    pottery_assert(!pottery_deque_is_empty(deque));
    pottery_deque_page_t* page = pottery_deque_page_ring_last(&deque->pages);
    return pottery_deque_entry_make(page, *page + deque->last_page_end - 1);
}

#if POTTERY_LIFECYCLE_CAN_PASS
static inline
pottery_deque_value_t pottery_deque_extract_first(pottery_deque_t* deque) {
    pottery_deque_value_t* p = pottery_deque_entry_value(deque, pottery_deque_first(deque));
    pottery_deque_value_t ret = pottery_move_if_cxx(*p);
    #ifdef __cplusplus
    // We have to run the destructor. See note in pottery_vector_extract()
    p->~pottery_deque_value_t();
    #endif
    pottery_deque_displace_first(deque);
    return ret;
}

static inline
pottery_deque_value_t pottery_deque_extract_last(pottery_deque_t* deque) {
    pottery_deque_value_t* p = pottery_deque_entry_value(deque, pottery_deque_last(deque));
    pottery_deque_value_t ret = pottery_move_if_cxx(*p);
    #ifdef __cplusplus
    // We have to run the destructor. See note in pottery_vector_extract()
    p->~pottery_deque_value_t();
    #endif
    pottery_deque_displace_last(deque);
    return ret;
}
#endif

#if POTTERY_FORWARD_DECLARATIONS
#if POTTERY_LIFECYCLE_CAN_PASS
/**
 * Pushes all values from the given array into the end of the deque. (The last
 * value of the given array will be the last value of the deque.)
 */
POTTERY_DEQUE_EXTERN
pottery_error_t pottery_deque_insert_last_bulk(pottery_deque_t* deque,
        const pottery_deque_value_t* values, size_t count);

/**
 * Pushes all values from the given array in reverse order into the front of
 * the deque. (The first value of the given array will be the first value of
 * the deque.)
 *
 * This is the opposite operation of pottery_deque_extract_first_bulk();
 * inserting an array of bulk data and then extracting it again is functionally
 * idempotent.
 */
POTTERY_DEQUE_EXTERN
pottery_error_t pottery_deque_insert_first_bulk(pottery_deque_t* deque, const pottery_deque_value_t* values, size_t count);

/**
 * Pops count values from the front of the deque into the given array. (The
 * first value of the deque will be the first value of the given array.)
 */
POTTERY_DEQUE_EXTERN
void pottery_deque_extract_first_bulk(pottery_deque_t* deque, pottery_deque_value_t* values, size_t count);

/**
 * Pops values from the back of the deque one-by-one in into the given array in
 * reverse order. (The value at the back of the deque will be placed at the end
 * of the given array.)
 *
 * This is the opposite operation of pottery_deque_insert_last_bulk();
 * inserting an array of bulk data and then extracting it again is functionally
 * idempotent.
 */
POTTERY_DEQUE_EXTERN
void pottery_deque_extract_last_bulk(pottery_deque_t* deque, pottery_deque_value_t* values, size_t count);
#endif

/**
 * Returns a pointer to up to count contiguous values starting at the given
 * entry, shifting the entry forwards past those values.
 *
 * The given count is adjusted to the number of contiguous values returned and
 * the number of values past which the entry has been shifted.
 *
 * The pointer returned is the same as that returned by
 * pottery_deque_entry_value() before the given entry is shifted.
 */
POTTERY_DEQUE_EXTERN
pottery_deque_value_t* pottery_deque_next_bulk(pottery_deque_t* deque,
        pottery_deque_entry_t* entry, size_t* count);

/**
 * Returns a pointer to up to count contiguous values starting at the given
 * entry, shifting the entry backwards past those values.
 *
 * The given count is adjusted to the number of contiguous values returned and
 * the number of values past which the entry has been shifted.
 *
 * The pointer returned is the same as that returned by
 * pottery_deque_entry_value() before the given entry is shifted.
 *
 * @warning The pointer returned is to the first value when iterating
 *          backwards, so it is the last value in memory! You must offset the
 *          pointer negatively to iterate through the returned values.
 */
POTTERY_DEQUE_EXTERN
pottery_deque_value_t* pottery_deque_previous_bulk(pottery_deque_t* deque,
        pottery_deque_entry_t* entry, size_t* count);
#endif

// Like most deque functions, the below could be optimized by making non-bulk
// implementations. For now we just wrap the bulk functions.

static inline
pottery_deque_entry_t pottery_deque_next(pottery_deque_t* deque, pottery_deque_entry_t entry) {
    size_t count = 1;
    pottery_deque_next_bulk(deque, &entry, &count);
    return entry;
}

static inline
pottery_deque_entry_t pottery_deque_previous(pottery_deque_t* deque, pottery_deque_entry_t entry) {
    size_t count = 1;
    pottery_deque_previous_bulk(deque, &entry, &count);
    return entry;
}

static inline
pottery_deque_entry_t pottery_deque_end(pottery_deque_t* deque) {
    // Deque uses one past the last element on the last page as its end entry.
    // (This means if the last page is full, the end entry is one past the end of
    // the page.) The only exception is when the deque is empty: since it has
    // no pages, the end entry for an empty deque is null/null.
    if (pottery_deque_is_empty(deque))
        return pottery_deque_entry_make(pottery_null, pottery_null);
    pottery_deque_page_t* page = pottery_deque_page_ring_last(&deque->pages);
    return pottery_deque_entry_make(page, *page + deque->last_page_end);
}

static inline
pottery_deque_entry_t pottery_deque_begin(pottery_deque_t* deque) {
    if (pottery_deque_is_empty(deque))
        return pottery_deque_entry_make(pottery_null, pottery_null);
    pottery_deque_page_t* page = pottery_deque_page_ring_first(&deque->pages);
    return pottery_deque_entry_make(page, *page + deque->first_page_start);
}

static inline
bool pottery_deque_entry_equal(pottery_deque_t* deque,
        pottery_deque_entry_t left, pottery_deque_entry_t right)
{
    (void)deque;
    // If the values match, the pages must as well.
    pottery_assert(left.value != right.value || left.page == right.page);
    return left.value == right.value;
}

static inline
bool pottery_deque_entry_exists(pottery_deque_t* deque, pottery_deque_entry_t entry) {
    pottery_assert(pottery_deque_index(deque, entry) <= pottery_deque_count(deque));
    return !pottery_deque_entry_equal(deque, entry, pottery_deque_end(deque));
}
