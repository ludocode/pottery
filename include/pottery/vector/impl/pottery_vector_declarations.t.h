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

#ifndef POTTERY_VECTOR_IMPL
#error "This is an internal header. Do not include it."
#endif

#if !POTTERY_LIFECYCLE_CAN_MOVE
    #error "Pottery vector requires a move expression. Define LIFECYCLE_MOVE, LIFECYCLE_MOVE_BY_VALUE or LIFECYCLE_BY_VALUE. See the documentation for details."
#endif



/*
 * Forward Declarations
 */

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_VECTOR_EXTERN
void pottery_vector_impl_reset(pottery_vector_t* vector);

// The bulk of the implementation is in emplace_at_bulk() and
// displace_at_bulk(). These create space and remove space for values in
// the vector, moving elements around as needed. All other functions
// (emplace/insert/remove/extract/displace) wrap these.

POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_emplace_at_bulk(pottery_vector_t* vector, size_t index,
        size_t count, pottery_vector_value_t** out);

POTTERY_VECTOR_EXTERN
void pottery_vector_displace_at_bulk(pottery_vector_t* vector, size_t index, size_t count);

static inline
pottery_vector_entry_t pottery_vector_end(pottery_vector_t* vector);

static inline
pottery_vector_entry_t pottery_vector_begin(const pottery_vector_t* vector);
#endif



/*
 * Lifecycle Operations
 */

static inline
pottery_error_t pottery_vector_init(pottery_vector_t* vector
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
        , pottery_vector_context_t context
        #endif
) {
    pottery_vector_impl_reset(vector);

    #if POTTERY_DEBUG
        #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
            vector->self_check = vector; // internal capacity makes this not bitwise movable
        #endif
        #if POTTERY_LEAK_CHECK
            // Note that we don't use the configured allocator
            vector->leak_check = malloc(1);
        #endif
    #endif

    #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
    vector->context = context;
    #endif

    return POTTERY_OK;
}

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_VECTOR_EXTERN
void pottery_vector_destroy(pottery_vector_t* vector);

POTTERY_VECTOR_EXTERN
void pottery_vector_move(pottery_vector_t* to, pottery_vector_t* from);
#endif

#if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
static inline pottery_vector_context_t pottery_vector_context(pottery_vector_t* vector) {
    return vector->context;
}

static inline void pottery_vector_set_context(pottery_vector_t* vector,
        pottery_vector_context_t context)
{
    vector->context = pottery_move_if_cxx(context);
}
#endif

static inline
size_t pottery_vector_count(const pottery_vector_t* vector) {
    return vector->count;
}

static inline
bool pottery_vector_is_empty(const pottery_vector_t* vector) {
    return vector->count == 0;
}

static inline
size_t pottery_vector_capacity(const pottery_vector_t* vector) {
    #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
    if (vector->storage == vector->u.internal)
        return POTTERY_VECTOR_INTERNAL_CAPACITY;
    #endif
    return vector->u.capacity;
}

static inline
pottery_vector_entry_t pottery_vector_at(const pottery_vector_t* vector, size_t index) {
    pottery_assert(index < pottery_vector_count(vector));
    return pottery_vector_begin(vector) + index;
}

static inline
pottery_vector_entry_t pottery_vector_select(const pottery_vector_t* vector, size_t index) {
    pottery_assert(index <= pottery_vector_count(vector));
    return pottery_vector_begin(vector) + index;
}

static inline
pottery_vector_value_t* pottery_vector_data(pottery_vector_t* vector) {
    return pottery_vector_begin(vector);
}

static inline
const pottery_vector_value_t* pottery_vector_const_data(const pottery_vector_t* vector) {
    #if POTTERY_VECTOR_DOUBLE_ENDED
    return vector->begin;
    #else
    return vector->storage;
    #endif
}

/**
 * Returns the first element in the vector.
 *
 * The vector must have at least one element!
 */
static inline
pottery_vector_entry_t pottery_vector_first(pottery_vector_t* vector) {
    pottery_assert(vector->count > 0);
    return pottery_vector_begin(vector);
}

/**
 * Returns the last element in the vector.
 *
 * The vector must have at least one element!
 */
static inline
pottery_vector_entry_t pottery_vector_last(pottery_vector_t* vector) {
    pottery_assert(vector->count > 0);
    return pottery_vector_begin(vector) + pottery_vector_count(vector) - 1;
}



/*
 * Miscellaneous
 */

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_reserve(pottery_vector_t* vector, size_t count);

POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_shrink(pottery_vector_t* vector);
#endif



/*
 * Entry Operations
 */

/**
 * Returns the offset of indices between the two given entries.
 */
static inline
ptrdiff_t pottery_vector_offset(pottery_vector_t* vector, pottery_vector_entry_t first,
        pottery_vector_entry_t second) {
    pottery_assert(first >= pottery_vector_begin(vector) && first <= pottery_vector_end(vector));
    pottery_assert(second >= pottery_vector_begin(vector) && second <= pottery_vector_end(vector));
    return pottery_cast(ptrdiff_t, second - first);
}

/**
 * Shifts an entry by the given offset.
 */
static inline
pottery_vector_entry_t pottery_vector_shift(pottery_vector_t* vector, pottery_vector_entry_t entry, ptrdiff_t offset) {
    pottery_assert(entry >= pottery_vector_begin(vector) && entry <= pottery_vector_end(vector));
    entry += offset;
    pottery_assert(entry >= pottery_vector_begin(vector) && entry <= pottery_vector_end(vector));
    return entry;
}

/**
 * Returns the index of the entry in the vector.
 */
static inline
size_t pottery_vector_index(pottery_vector_t* vector, pottery_vector_entry_t entry) {
    pottery_assert(entry >= pottery_vector_begin(vector) && entry <= pottery_vector_end(vector));
    return pottery_cast(size_t, entry - pottery_vector_begin(vector));
}

/**
 * Returns true if the entry exists, false otherwise.
 */
static inline
bool pottery_vector_entry_exists(pottery_vector_t* vector, pottery_vector_entry_t entry) {
    pottery_assert(entry >= pottery_vector_begin(vector) && entry <= pottery_vector_end(vector));
    return entry != pottery_vector_end(vector);
}

/**
 * Returns true if the entries are equal, false otherwise.
 */
static inline
bool pottery_vector_entry_equal(pottery_vector_t* vector, pottery_vector_entry_t left,
        pottery_vector_entry_t right)
{
    (void)vector;
    pottery_assert(left >= pottery_vector_begin(vector) && left <= pottery_vector_end(vector));
    pottery_assert(right >= pottery_vector_begin(vector) && right <= pottery_vector_end(vector));
    return left == right;
}

/**
 * Returns a pointer (a entry) to the value contained in this entry.
 */
static inline
pottery_vector_value_t* pottery_vector_entry_value(pottery_vector_t* vector,
        pottery_vector_entry_t entry)
{
    (void)vector;
    pottery_assert(entry >= pottery_vector_begin(vector) && entry <= pottery_vector_end(vector));
    pottery_assert(pottery_vector_entry_exists(vector, entry));
    return entry;
}

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_VECTOR_EXTERN
void pottery_vector_displace_all(pottery_vector_t* vector);

POTTERY_VECTOR_EXTERN
void pottery_vector_swap(pottery_vector_t* left, pottery_vector_t* right);

POTTERY_VECTOR_EXTERN
void pottery_vector_init_steal(pottery_vector_t* vector, pottery_vector_t* other);

POTTERY_VECTOR_EXTERN
void pottery_vector_steal(pottery_vector_t* vector, pottery_vector_t* other);

#if POTTERY_LIFECYCLE_CAN_INIT_COPY
POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_init_copy(pottery_vector_t* vector, const pottery_vector_t* other);
#endif

#if POTTERY_LIFECYCLE_CAN_INIT_COPY && POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_copy(pottery_vector_t* vector, const pottery_vector_t* other);
#endif

#endif // POTTERY_FORWARD_DECLARATIONS



/*
 * Construct
 */

#if defined(__cplusplus) && POTTERY_VECTOR_CXX
template<class... Args> static inline
pottery_error_t pottery_vector_construct_at(pottery_vector_t* vector,
        size_t index, pottery_vector_entry_t* entry, Args&&... args)
{
    // Use a temporary to avoid confusing clang analyzer
    pottery_vector_entry_t temp;
    pottery_error_t error = pottery_vector_emplace_at(vector, index, &temp);
    *entry = temp;
    if (error != POTTERY_OK)
        return error;

    error = pottery::construct(temp, std::forward<Args>(args)...);
    if (error != POTTERY_OK)
        pottery_vector_displace_at_bulk(vector, index, 1);
    return error;
}

template<class... Args> static inline
pottery_error_t pottery_vector_construct_first(pottery_vector_t* vector,
        pottery_vector_entry_t* entry, Args&&... args)
{
    return pottery_vector_construct_at(vector, 0, entry, std::forward<Args>(args)...);
}

template<class... Args> static inline
pottery_error_t pottery_vector_construct_last(pottery_vector_t* vector,
        pottery_vector_entry_t* entry, Args&&... args)
{
    return pottery_vector_construct_at(vector, vector->count, entry, std::forward<Args>(args)...);
}
#endif



/*
 * Insert
 */

#if POTTERY_LIFECYCLE_CAN_PASS

#if defined(__cplusplus) && POTTERY_VECTOR_CXX

#if POTTERY_LIFECYCLE_CAN_INIT_COPY

// C++ const entryerence

static inline
pottery_error_t pottery_vector_insert_at(pottery_vector_t* vector,
        size_t index, const pottery_vector_value_t& value)
{
    pottery_vector_entry_t entry;
    pottery_error_t error = pottery_vector_emplace_at_bulk(vector, index, 1, &entry);
    if (error == POTTERY_OK) {
        error = pottery_vector_lifecycle_init_copy(POTTERY_VECTOR_CONTEXT_VAL(vector) entry, &value);
        if (error != POTTERY_OK)
            pottery_vector_displace_at_bulk(vector, index, 1);
    }
    return error;
}

static inline
pottery_error_t pottery_vector_insert_first(pottery_vector_t* vector, pottery_vector_value_t& value) {
    return pottery_vector_insert_at(vector, 0, value);
}

static inline
pottery_error_t pottery_vector_insert_last(pottery_vector_t* vector, pottery_vector_value_t& value) {
    return pottery_vector_insert_at(vector, vector->count, value);
}

static inline
pottery_error_t pottery_vector_insert_before(pottery_vector_t* vector,
        pottery_vector_entry_t* entry, pottery_vector_value_t& value)
{
    size_t index = pottery_vector_index(vector, entry);
    return pottery_vector_insert_at(vector, index, value);
}

static inline
pottery_error_t pottery_vector_insert_after(pottery_vector_t* vector,
        pottery_vector_entry_t* entry, pottery_vector_value_t& value)
{
    pottery_assert(pottery_vector_entry_exists(vector, entry));
    size_t index = pottery_vector_index(vector, entry);
    return pottery_vector_insert_at(vector, index + 1, value);
}

#endif // POTTERY_LIFECYCLE_CAN_INIT_COPY

#if POTTERY_LIFECYCLE_CAN_INIT_STEAL

// C++ rvalue entryerence

static inline
pottery_error_t pottery_vector_insert_at(pottery_vector_t* vector,
        size_t index, pottery_vector_value_t&& value)
{
    pottery_vector_entry_t entry;
    pottery_error_t error = pottery_vector_emplace_at_bulk(vector, index, 1, &entry);
    if (error == POTTERY_OK) {
        error = pottery_vector_lifecycle_init_steal(POTTERY_VECTOR_CONTEXT_VAL(vector) entry, &value);
        if (error != POTTERY_OK)
            pottery_vector_displace_at_bulk(vector, index, 1);
    }
    return error;
}

static inline
pottery_error_t pottery_vector_insert_first(pottery_vector_t* vector, pottery_vector_value_t&& value) {
    return pottery_vector_insert_at(vector, 0, std::move(value));
}

static inline
pottery_error_t pottery_vector_insert_last(pottery_vector_t* vector, pottery_vector_value_t&& value) {
    return pottery_vector_insert_at(vector, vector->count, std::move(value));
}

static inline
pottery_error_t pottery_vector_insert_before(pottery_vector_t* vector,
        pottery_vector_entry_t* entry, pottery_vector_value_t&& value)
{
    size_t index = pottery_vector_index(vector, entry);
    return pottery_vector_insert_at(vector, index, std::move(value));
}

static inline
pottery_error_t pottery_vector_insert_after(pottery_vector_t* vector,
        pottery_vector_entry_t* entry, pottery_vector_value_t&& value)
{
    pottery_assert(pottery_vector_entry_exists(vector, entry));
    size_t index = pottery_vector_index(vector, entry);
    return pottery_vector_insert_at(vector, index + 1, std::move(value));
}

#endif

#else // C/C++

// C by value
// (This might still be used in C++ if POTTERY_VECTOR_CXX is disabled so we
// still need to properly run constructors.)

static inline
pottery_error_t pottery_vector_insert_at(pottery_vector_t* vector, size_t index, pottery_vector_value_t value) {
    pottery_vector_entry_t entry;
    pottery_error_t error = pottery_vector_emplace_at_bulk(vector, index, 1, &entry);
    if (error == POTTERY_OK)
        pottery_move_construct(pottery_vector_value_t, *entry, value);
    return error;
}

static inline
pottery_error_t pottery_vector_insert_first(pottery_vector_t* vector, pottery_vector_value_t value) {
    return pottery_vector_insert_at(vector, 0, value);
}

static inline
pottery_error_t pottery_vector_insert_last(pottery_vector_t* vector, pottery_vector_value_t value) {
    return pottery_vector_insert_at(vector, vector->count, value);
}

static inline
pottery_error_t pottery_vector_insert_before(pottery_vector_t* vector,
        pottery_vector_entry_t entry, pottery_vector_value_t value)
{
    size_t index = pottery_vector_index(vector, entry);
    return pottery_vector_insert_at(vector, index, value);
}

static inline
pottery_error_t pottery_vector_insert_after(pottery_vector_t* vector,
        pottery_vector_entry_t entry, pottery_vector_value_t value)
{
    pottery_assert(pottery_vector_entry_exists(vector, entry));
    size_t index = pottery_vector_index(vector, entry);
    return pottery_vector_insert_at(vector, index + 1, value);
}

#endif // C/C++

// bulk methods

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_insert_at_bulk(pottery_vector_t* vector, size_t index,
        const pottery_vector_value_t* values, size_t count);
#endif

static inline
pottery_error_t pottery_vector_insert_first_bulk(pottery_vector_t* vector, const pottery_vector_value_t* values, size_t count) {
    return pottery_vector_insert_at_bulk(vector, 0, values, count);
}

static inline
pottery_error_t pottery_vector_insert_last_bulk(pottery_vector_t* vector, const pottery_vector_value_t* values, size_t count) {
    return pottery_vector_insert_at_bulk(vector, pottery_vector_count(vector), values, count);
}

static inline
pottery_error_t pottery_vector_insert_vector(pottery_vector_t* vector, size_t index, pottery_vector_t* other) {
    return pottery_vector_insert_at_bulk(vector, index, pottery_vector_begin(other), pottery_vector_count(other));
}

static inline
pottery_error_t pottery_vector_insert_first_vector(pottery_vector_t* vector, pottery_vector_t* other) {
    return pottery_vector_insert_vector(vector, 0, other);
}

static inline
pottery_error_t pottery_vector_insert_last_vector(pottery_vector_t* vector, pottery_vector_t* other) {
    return pottery_vector_insert_vector(vector, pottery_vector_count(vector), other);
}

#endif // CAN_PASS



/*
 * Iteration
 */

/**
 * Returns an entry representing the start of the vector (the first element if
 * it has any elements, or the end of the vector otherwise.)
 *
 * If the vector is empty, the returned entry does not exist, and is equal to
 * pottery_vector_end().
 */
static inline
pottery_vector_entry_t pottery_vector_begin(const pottery_vector_t* vector) {
    #if POTTERY_VECTOR_DOUBLE_ENDED
    return vector->begin;
    #else
    return vector->storage;
    #endif
}

/**
 * Returns a non-existent entry representing the end of the vector (one past
 * the last element in the vector.)
 *
 * @see pottery_vector_entry_exists()
 */
static inline
pottery_vector_entry_t pottery_vector_end(pottery_vector_t* vector) {
    // Vector uses one-past-the-end as its end entry. If the vector is empty,
    // it uses the start of the storage, or the start offset into the storage
    // for a double-ended vector. This may be null if there is no internal
    // capacity and if storage has not yet been allocated.
    return pottery_vector_begin(vector) + pottery_vector_count(vector);
}

/**
 * Shifts the entry over to the next element, or the end of the vector.
 *
 * It is an error to call this on the end of the vector.
 */
static inline
pottery_vector_entry_t pottery_vector_next(pottery_vector_t* vector, pottery_vector_entry_t entry) {
    (void)vector;
    pottery_assert(entry >= pottery_vector_begin(vector) && entry <= pottery_vector_end(vector));
    pottery_assert(entry != pottery_vector_end(vector));
    return entry + 1;
}

/**
 * Shifts the entry over to the previous element, which must exist.
 *
 * Calling this on the end of the vector returns a entry to the last element.
 *
 * It is an error to call this on the first element.
 */
static inline
pottery_vector_entry_t pottery_vector_previous(pottery_vector_t* vector, pottery_vector_entry_t entry) {
    (void)vector;
    pottery_assert(entry >= pottery_vector_begin(vector) && entry <= pottery_vector_end(vector));
    pottery_assert(entry != pottery_vector_begin(vector));
    return entry - 1;
}



/*
 * Emplace
 */

static inline
pottery_error_t pottery_vector_emplace_at(pottery_vector_t* vector,
        size_t index, pottery_vector_entry_t* entry)
{
    return pottery_vector_emplace_at_bulk(vector, index, 1, entry);
}

static inline
pottery_error_t pottery_vector_emplace_first(pottery_vector_t* vector, pottery_vector_entry_t* entry) {
    return pottery_vector_emplace_at(vector, 0, entry);
}

static inline
pottery_error_t pottery_vector_emplace_last(pottery_vector_t* vector, pottery_vector_entry_t* entry) {
    return pottery_vector_emplace_at(vector, pottery_vector_count(vector), entry);
}

static inline
pottery_error_t pottery_vector_emplace_first_bulk(pottery_vector_t* vector,
        size_t count, pottery_vector_entry_t* entry)
{
    return pottery_vector_emplace_at_bulk(vector, 0, count, entry);
}

static inline
pottery_error_t pottery_vector_emplace_last_bulk(pottery_vector_t* vector,
        size_t count, pottery_vector_entry_t* entry)
{
    return pottery_vector_emplace_at_bulk(vector, pottery_vector_count(vector), count, entry);
}



/*
 * Displace
 */

static inline
void pottery_vector_displace_at(pottery_vector_t* vector, size_t index) {
    pottery_vector_displace_at_bulk(vector, index, 1);
}

static inline
void pottery_vector_displace(pottery_vector_t* vector, pottery_vector_entry_t entry) {
    pottery_vector_displace_at(vector, pottery_vector_index(vector, entry));
}

static inline
void pottery_vector_displace_first(pottery_vector_t* vector) {
    pottery_assert(!pottery_vector_is_empty(vector));
    pottery_vector_displace_at(vector, 0);
}

static inline
void pottery_vector_displace_last(pottery_vector_t* vector) {
    pottery_assert(!pottery_vector_is_empty(vector));
    pottery_vector_displace_at(vector, pottery_vector_count(vector) - 1);
}



/*
 * Remove
 */

#if POTTERY_LIFECYCLE_CAN_DESTROY
#if POTTERY_FORWARD_DECLARATIONS
POTTERY_VECTOR_EXTERN
void pottery_vector_remove_at(pottery_vector_t* vector, size_t index);

POTTERY_VECTOR_EXTERN
void pottery_vector_remove_all(pottery_vector_t* vector);

POTTERY_VECTOR_EXTERN
void pottery_vector_remove_at_bulk(pottery_vector_t* vector, size_t index, size_t count);
#endif // forward decls

static inline
void pottery_vector_remove_first_bulk(pottery_vector_t* vector, size_t count) {
    pottery_vector_remove_at_bulk(vector, 0, count);
}

static inline
void pottery_vector_remove_last_bulk(pottery_vector_t* vector, size_t count) {
    pottery_vector_remove_at_bulk(vector, pottery_vector_count(vector) - count, count);
}

static inline
void pottery_vector_remove(pottery_vector_t* vector, pottery_vector_entry_t entry) {
    pottery_vector_remove_at(vector, pottery_vector_index(vector, entry));
}

static inline
void pottery_vector_remove_first(pottery_vector_t* vector) {
    pottery_assert(!pottery_vector_is_empty(vector));
    pottery_vector_remove_at(vector, 0);
}

static inline
void pottery_vector_remove_last(pottery_vector_t* vector) {
    pottery_assert(!pottery_vector_is_empty(vector));
    pottery_vector_remove_at(vector, pottery_vector_count(vector) - 1);
}
#endif // CAN_DESTROY



/*
 * Extract
 */

#if POTTERY_LIFECYCLE_CAN_PASS
static inline
pottery_vector_value_t pottery_vector_extract(pottery_vector_t* vector,
        pottery_vector_entry_t entry)
{
    pottery_vector_value_t ret = pottery_move_if_cxx(*entry);

    #ifdef __cplusplus
    // The above uses move construction (rather than the move() operation) to
    // pull the value out. For C this makes no difference because CAN_PASS
    // requires MOVE_BY_VALUE. But we do it this way for C++ because we need a
    // real value we can return. If we moved into an unconstructed temporary,
    // no destructor would run on return, and if we moved into a normal value,
    // its constructor would be run twice.
    //
    // We don't want to remove() the existing value in the array (in case e.g.
    // DESTROY is free(); we don't own it anymore), but we still need to run
    // its destructor before we displace() it so we do that manually.
    entry->~pottery_vector_value_t();
    #endif

    pottery_vector_displace(vector, entry);

    return ret;
}

static inline
pottery_vector_value_t pottery_vector_extract_at(pottery_vector_t* vector, size_t index) {
    pottery_vector_entry_t entry = pottery_vector_at(vector, index);
    return pottery_vector_extract(vector, entry);
}

static inline
pottery_vector_value_t pottery_vector_extract_first(pottery_vector_t* vector) {
    pottery_assert(!pottery_vector_is_empty(vector));
    return pottery_vector_extract_at(vector, 0);
}

static inline
pottery_vector_value_t pottery_vector_extract_last(pottery_vector_t* vector) {
    pottery_assert(!pottery_vector_is_empty(vector));
    return pottery_vector_extract_at(vector, pottery_vector_count(vector) - 1);
}
#endif
