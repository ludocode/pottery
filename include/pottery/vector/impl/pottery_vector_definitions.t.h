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

#ifndef POTTERY_VECTOR_IMPL
#error "This is an internal header. Do not include it."
#endif

// helper to allocate an array of elements
static inline pottery_vector_element_t* pottery_vector_impl_alloc(
        pottery_vector_t* vector, size_t* count)
{
    (void)vector;
    void* ptr = pottery_vector_alloc_malloc_array_at_least(
            POTTERY_VECTOR_CONTEXT_VAL(vector)
            pottery_alignof(pottery_vector_element_t),
            count, sizeof(pottery_vector_element_t));
    return pottery_cast(pottery_vector_element_t*, ptr);
}

// helper to free an array of elements
static inline void pottery_vector_impl_free(pottery_vector_t* vector,
        pottery_vector_element_t* elements)
{
    (void)vector;
    pottery_vector_alloc_free(
            POTTERY_VECTOR_CONTEXT_VAL(vector)
            pottery_alignof(pottery_vector_element_t), elements);
}

// minimum capacity for an allocation. internal space can be smaller than this.
static inline
size_t pottery_vector_minimum_capacity(void) {
    #ifdef POTTERY_VECTOR_MINIMUM_CAPACITY
    return (POTTERY_VECTOR_MINIMUM_CAPACITY);
    #else
    // We define a reasonable default minimum size as the greater of 4 elements
    // or however many fit in 32 bytes.
    return pottery_max_s(32 / sizeof(pottery_vector_element_t), 4);
    #endif
}

POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_reserve(pottery_vector_t* vector, size_t count) {
    // We create space at the end of the vector for the additional elements
    // then drop the count back down so we keep the space without resizing.
    size_t old_count = vector->count;
    if (count < old_count)
        return POTTERY_OK;
    pottery_vector_element_t* elements;
    pottery_error_t error = pottery_vector_impl_create_space(vector, old_count, count - old_count, &elements);
    if (error == POTTERY_OK)
        vector->count = old_count;
    return error;
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
// Destroys and destructs all elements in the vector.
// TODO get rid of this, just call destroy_bulk from lifecycle
static void pottery_vector_impl_destroy_destruct_all(pottery_vector_t* vector) {
    size_t i;
    for (i = 0; i < vector->count; ++i) {
        pottery_vector_element_t* element = pottery_vector_at(vector, i);
        pottery_vector_lifecycle_destroy(POTTERY_VECTOR_CONTEXT_VAL(vector) element);
        //pottery_vector_element_destruct(element);
    }
}
#endif

// Clears the vector after all its elements have been destroyed and destructed.
static void pottery_vector_impl_clear(pottery_vector_t* vector) {
    vector->count = 0;

    // We only free the vector if we're using more than double the minimum
    // space. This will prevent us from continually allocating and freeing if
    // the vector is alternating between empty and a small number of elements.
    // This is another parameter that should eventually be tunable.
    if (
            #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
            vector->values != vector->u.internal &&
            #endif
            vector->u.capacity > 2 * pottery_vector_minimum_capacity()
    ) {
        pottery_vector_impl_free(vector, vector->values);

        #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
        vector->values = vector->u.internal;
        #else
        vector->values = pottery_null;
        vector->u.capacity = 0;
        #endif
    }
}

POTTERY_VECTOR_EXTERN
void pottery_vector_displace_all(pottery_vector_t* vector) {
    #if 0
    size_t i;
    for (i = 0; i < vector->count; ++i) {
        pottery_vector_element_destruct(pottery_vector_at(vector, i));
    }
    #endif
    pottery_vector_impl_clear(vector);
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_VECTOR_EXTERN
void pottery_vector_remove_all(pottery_vector_t* vector) {
    pottery_vector_impl_destroy_destruct_all(vector);
    pottery_vector_impl_clear(vector);
}
#endif

POTTERY_VECTOR_EXTERN
void pottery_vector_destroy(pottery_vector_t* vector) {
    // make sure the container is not already destroyed or otherwise invalid
    pottery_assert(vector->values != (pottery_vector_element_t*)(-1));

    #if POTTERY_LIFECYCLE_CAN_DESTROY
    pottery_vector_impl_destroy_destruct_all(vector);
    #else
    // If we don't know how to destroy our values, it is an error to destroy a
    // non-empty container. The vector contents must be properly disposed of.
    pottery_assert(vector->count == 0);
    #endif

    if (vector->values != pottery_null
            #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
            && vector->values != vector->u.internal
            #endif
    ) {
        pottery_vector_impl_free(vector, vector->values);
    } else {
        #if POTTERY_DEBUG
        // If values is null or internal, assign it to garbage to detect
        // double-destroy. (If it was allocated, we expect you have some other
        // means of detecting double-free in debug builds, so we leave it
        // as-is.)
        vector->values = (pottery_vector_element_t*)(-1);
        #endif
    }
}

POTTERY_VECTOR_EXTERN
void pottery_vector_move(pottery_vector_t* to, pottery_vector_t* from) {
    #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
    if (from->values == from->u.internal) {

        // initialize the vector, which gives us sufficient internal space
        pottery_vector_init(to);
        pottery_assert(to->values == to->u.internal);
        pottery_assert(pottery_vector_capacity(to) >= from->count);

        // move the elements
        to->count = from->count;
        pottery_vector_lifecycle_move_bulk_restrict(POTTERY_VECTOR_CONTEXT_VAL(from) to->values, from->values, to->count);
        from->count = 0;

        // move the context
        #ifdef POTTERY_VECTOR_CONTEXT_TYPE
        to->context = from->context;
        #endif

        // destroy the source vector
        pottery_vector_destroy(from);
    } else
    #endif
    {
        // steal the values without init/destroy
        to->values = from->values;
        to->count = from->count;
        to->u.capacity = from->u.capacity;

        // also steal debug stuff
        #ifdef POTTERY_DEBUG
        to->self_check = from->self_check;
        to->leak_check = from->leak_check;
        #endif

        // steal the context
        #if defined(POTTERY_VECTOR_ALLOC_CONTEXT_TYPE) && !defined(POTTERY_VECTOR_ALLOC_CONTEXT)
        to->alloc_context = from->alloc_context;
        #endif
    }

    #if POTTERY_DEBUG
    from->self_check = (pottery_vector_t*)(-1);
    from->leak_check = (pottery_vector_t*)(-1);

    // As above, help detect double destroy.
    if (from->values == pottery_null
            #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
            || from->values == from->u.internal
            #endif
            )
        from->values = (pottery_vector_element_t*)(-1);
    #endif
}

POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_emplace_at(pottery_vector_t* vector, size_t index, pottery_vector_entry_t* entry) {
    pottery_error_t error = pottery_vector_impl_create_space(vector, index, 1, entry);
    #if 0
    if (error == POTTERY_OK) {
        error = pottery_vector_element_construct(entry);
        if (error != POTTERY_OK)
            pottery_vector_impl_remove_space(vector, index, 1);
    }
    #endif
    return error;
}

POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_emplace_at_bulk(pottery_vector_t* vector, size_t index, size_t count, pottery_vector_entry_t* entry) {
    pottery_error_t error = pottery_vector_impl_create_space(vector, index, count, entry);
    if (error != POTTERY_OK)
        return error;

    #if 0
    size_t i;
    for (i = index; i < count; ++i) {
        pottery_vector_element_t* element = pottery_bless(pottery_vector_element_t, *entry + i);
        error = pottery_vector_element_construct(&element);
        if (error != POTTERY_OK) {
            pottery_vector_element_destruct_bulk(*entry, i);
            pottery_vector_impl_remove_space(vector, index, count);
            return error;
        }
    }
    #endif

    return POTTERY_OK;
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_VECTOR_EXTERN
void pottery_vector_remove_at(pottery_vector_t* vector, size_t index) {
    pottery_vector_lifecycle_destroy(POTTERY_VECTOR_CONTEXT_VAL(vector) pottery_vector_at(vector, index));
    pottery_vector_displace_at(vector, index);
}
#endif

/*
 * Creates uninitialized, unconstructed space for count elements at index.
 */
POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_impl_create_space(pottery_vector_t* vector,
        size_t index, size_t count, pottery_vector_element_t** elements)
{
    #if POTTERY_DEBUG
    *elements = pottery_reinterpret_cast(pottery_vector_element_t*, -1);
    #endif

    // Make sure the arguments are valid.
    if (count == 0) {
        // we shouldn't need to set the pointer here because it shouldn't be
        // accessed but certain static analyzers (e.g. MSVC in LTO mode) think
        // it's accessed without being set.
        *elements = pottery_reinterpret_cast(pottery_vector_element_t*, -1);
        return POTTERY_OK; // no elements
    }
    if (vector->count + count < vector->count)
        return POTTERY_ERROR_OVERFLOW;
    size_t old_count = vector->count;
    size_t new_count = old_count + count;
    pottery_assert(index <= old_count);

    // see if we have enough space.
    if (new_count <= pottery_vector_capacity(vector)) {
        pottery_vector_lifecycle_move_bulk_up(POTTERY_VECTOR_CONTEXT_VAL(vector) vector->values + index + count,
                vector->values + index, vector->count - index);
        vector->count += count;
        *elements = vector->values + index;
        return POTTERY_OK;
    }

    // Calculate new capacity.
    // We grow by a factor of 1.5. This is apparently best practice now. It
    // would be nice to actually benchmark this.
    size_t old_capacity = pottery_vector_capacity(vector);
    size_t new_capacity = ((old_capacity / 2) + 1) * 3; // add 1 to avoid rounding down to zero
    if (new_capacity < old_capacity)
        return POTTERY_ERROR_OVERFLOW;
    if (new_capacity < new_count)
        new_capacity = new_count; // make sure we have enough space for all the elements to be added
    if (new_capacity < pottery_vector_minimum_capacity())
        new_capacity = pottery_vector_minimum_capacity();

    size_t alloc_size;
    if (pottery_mul_overflow_s(sizeof(pottery_vector_element_t), new_capacity, &alloc_size))
        return POTTERY_ERROR_OVERFLOW;

    // TODO if can move by value, use realloc_at_least
    pottery_vector_element_t* new_values = pottery_vector_impl_alloc(vector, &new_capacity);
    if (new_values == pottery_null)
        return POTTERY_ERROR_ALLOC;

    if (vector->values != pottery_null) {
        pottery_vector_lifecycle_move_bulk_restrict(POTTERY_VECTOR_CONTEXT_VAL(vector) new_values, vector->values, index);
        pottery_vector_lifecycle_move_bulk_restrict(POTTERY_VECTOR_CONTEXT_VAL(vector) new_values + index + count,
                vector->values + index, old_count - index);
        if (true
                #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
                && vector->values != vector->u.internal
                #endif
        ) {
            pottery_vector_impl_free(vector, vector->values);
        }
    }

    vector->values = new_values;
    vector->u.capacity = new_capacity;
    vector->count = new_count;
    *elements = pottery_bless(pottery_vector_element_t, vector->values + index);
    return POTTERY_OK;
}

#if POTTERY_LIFECYCLE_CAN_PASS
POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_insert_at_bulk(pottery_vector_t* vector, size_t index, const pottery_vector_element_t* values, size_t count) {
    if (count == 0)
        return POTTERY_OK;

    pottery_vector_element_t* inserted;
    pottery_error_t error = pottery_vector_impl_create_space(vector, index, count, &inserted);
    if (error != POTTERY_OK)
        return error;

    size_t i;
    for (i = 0; i < count; ++i) {
        pottery_vector_element_t* element = inserted + i;
        error = pottery_vector_lifecycle_init_copy(POTTERY_VECTOR_CONTEXT_VAL(vector) element, values++);
        if (error != POTTERY_OK) {
            pottery_vector_lifecycle_destroy_bulk(POTTERY_VECTOR_CONTEXT_VAL(vector) inserted, i);
            pottery_vector_impl_remove_space(vector, index, count);
            return error;
        }
    }

    return POTTERY_OK;
}
#endif

POTTERY_VECTOR_EXTERN
void pottery_vector_impl_remove_space(pottery_vector_t* vector, size_t index, size_t count) {
    if (count == 0)
        return;
    pottery_assert(index + count > index); // overflow check
    pottery_assert(vector->count >= index + count); // bounds check

    size_t old_count = vector->count;
    size_t new_count = old_count - count;
    vector->count = new_count;

    // See if we should shrink based on various factors. The below factors
    // certainly need some tweaking; right now they're mostly just guesses.

    // Right now we shrink by half because we allocate at least a given
    // buffer size and let the allocator expand it. We want to make sure
    // that the allocator won't expand our smaller buffer to give us a buffer
    // of the same original size which would cause us to attempt to shrink
    // repeatedly on every removal.

    bool should_shrink = true;
    size_t old_capacity = pottery_vector_capacity(vector);

    // Don't shrink if we're on internal space
    #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
    should_shrink &= vector->values != vector->u.internal;
    #endif

    // Don't shrink if we're still using more than a quarter of our capacity
    if (should_shrink)
        should_shrink &= new_count < old_capacity / 4;

    // Don't shrink if we're not empty and capacity is at most these thresholds
    const size_t threshold_bytes = 64;
    const size_t threshold_elements = 8;
    size_t old_bytes = 0;
    if (should_shrink && new_count != 0 && !pottery_mul_overflow_s(old_capacity,
                sizeof(pottery_vector_element_t), &old_bytes))
        should_shrink &= old_bytes > threshold_bytes && new_count > threshold_elements;

    // Don't shrink if new capacity wouldn't change
    size_t new_capacity = old_capacity;
    if (should_shrink) {
        // shrink down by half
        // (no overflow check needed here, current capacity is larger than
        // new_count * 2 because of checks above)
        new_capacity = pottery_max_s(new_count * 2, pottery_vector_minimum_capacity());
        should_shrink &= new_capacity < old_capacity;
    }

    if (should_shrink) {
        pottery_assert(new_capacity < old_capacity);

        pottery_vector_element_t* new_values;

        #if POTTERY_VECTOR_INTERNAL_CAPACITY == 0
        if (new_count == 0) {
            pottery_vector_impl_free(vector, vector->values);
            vector->values = pottery_null;
            vector->u.capacity = 0;
            return;
        }
        #endif

        // TODO if moves by value, use realloc_at_least

        #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
        if (new_capacity <= POTTERY_VECTOR_INTERNAL_CAPACITY) {
            // switch back to internal space
            new_values = vector->u.internal;
        } else
        #endif
        {
            // allocate new smaller space
            size_t expanded_capacity = new_capacity;
            new_values = pottery_vector_impl_alloc(vector, &expanded_capacity);

            if (expanded_capacity >= old_capacity) {
                // If expanding the capacity gave us a buffer that isn't
                // actually smaller, we assert against this because we should
                // prevent shrinking below a certain threshold to prevent this,
                // and we ignore the expanded capacity to pretend like we
                // actually are shrinking. This will incur some unnecessary
                // moves, but it will prevent us from trying to shrink over and
                // over every time we remove an element. If this happens to
                // you, it is safe to ignore this but please file a bug report
                // describing your platform!
                pottery_assert(false);
            } else {
                new_capacity = expanded_capacity;
            }
        }

        if (new_values != pottery_null) {
            // move values into new space
            pottery_vector_lifecycle_move_bulk_restrict(POTTERY_VECTOR_CONTEXT_VAL(vector) new_values, vector->values, index);
            pottery_vector_lifecycle_move_bulk_restrict(POTTERY_VECTOR_CONTEXT_VAL(vector) new_values + index,
                    vector->values + index + count, new_count - index);
            pottery_vector_impl_free(vector, vector->values);

            vector->values = new_values;

            #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
            if (new_values != vector->u.internal)
            #endif
                vector->u.capacity = new_capacity;

            return;
        }

        // if for whatever reason allocation of a smaller buffer failed, we
        // don't report error from remove/displace; we just keep the larger
        // buffer.
    }

    // no shrink. move elements down in-place.
    pottery_vector_lifecycle_move_bulk_down(POTTERY_VECTOR_CONTEXT_VAL(vector) vector->values + index,
            vector->values + index + count, new_count - index);
}

POTTERY_VECTOR_EXTERN
void pottery_vector_displace_at_bulk(pottery_vector_t* vector, size_t index, size_t count) {
    #if 0
    size_t i;
    for (i = index; i < index + count; ++i) {
        pottery_vector_element_destruct(pottery_vector_at(vector, i));
    }
    #endif
    pottery_vector_impl_remove_space(vector, index, count);
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_VECTOR_EXTERN
void pottery_vector_remove_at_bulk(pottery_vector_t* vector, size_t index, size_t count) {
    size_t i;
    for (i = index; i < index + count; ++i) {
        pottery_vector_lifecycle_destroy(POTTERY_VECTOR_CONTEXT_VAL(vector) pottery_vector_at(vector, i));
        //pottery_vector_element_destruct(pottery_vector_at(vector, i));
    }
    pottery_vector_impl_remove_space(vector, index, count);
}
#endif

POTTERY_VECTOR_EXTERN
void pottery_vector_swap(pottery_vector_t* left, pottery_vector_t* right) {
    #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
    if (left->values == left->u.internal || right->values == right->u.internal) {
        // If either vector is internal, we defer swap to move which will handle
        // it for us.
        POTTERY_DECLARE_UNCONSTRUCTED(pottery_vector_t, temp);
        pottery_vector_move(temp, left);
        pottery_vector_move(left, right);
        pottery_vector_move(right, temp);
        return;
    }
    #endif

    // Move by value. (This will call C++ copy constructor and copy assignment
    // for non-trivial contexts.)
    pottery_vector_t temp = *left;
    *left = *right;
    *right = temp;
}

#if POTTERY_LIFECYCLE_CAN_INIT_COPY
// Copies the contents of other into this where this is empty.
POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_impl_copy(pottery_vector_t* vector, const pottery_vector_t* other) {
    pottery_assert(pottery_vector_is_empty(vector));
    size_t count = pottery_vector_count(other);

    pottery_vector_element_t* src = other->values;
    pottery_vector_element_t* dest;
    pottery_error_t error = pottery_vector_impl_create_space(vector, 0, count, &dest);
    if (error != POTTERY_OK)
        return error;

    size_t i;
    for (i = 0; i < count; ++i) {
        pottery_vector_element_t* element = dest + i;
        error = pottery_vector_lifecycle_init_copy(POTTERY_VECTOR_CONTEXT_VAL(vector) element, src++);
        if (error != POTTERY_OK) {
            pottery_vector_lifecycle_destroy_bulk(POTTERY_VECTOR_CONTEXT_VAL(vector) dest, i);
            pottery_vector_impl_clear(vector);
            return error;
        }
    }

    #if defined(POTTERY_VECTOR_ALLOC_CONTEXT_TYPE) && !defined(POTTERY_VECTOR_ALLOC_CONTEXT)
    vector->alloc_context = other->alloc_context;
    #endif

    return POTTERY_OK;
}

POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_init_copy(pottery_vector_t* vector, const pottery_vector_t* other) {
    pottery_vector_init(vector);
    return pottery_vector_impl_copy(vector, other);
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_copy(pottery_vector_t* vector, const pottery_vector_t* other) {

    // If we have enough internal space and we copy by value (without the
    // possibility of an exception), we don't need the temporary because the
    // copy can't fail.
    #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0 && \
            POTTERY_LIFECYCLE_CAN_PASS && !POTTERY_CXX_EXCEPTIONS
    if (pottery_vector_count(other) <= POTTERY_VECTOR_INTERNAL_CAPACITY) {
        pottery_vector_remove_all(vector);

        // It should not be possible for this to fail.
        pottery_error_t error = pottery_vector_impl_copy(vector, other);
        (void)error;
        pottery_assert(error == POTTERY_OK);

        return POTTERY_OK;
    }
    #endif

    // Copy to a temporary so that we don't damage the vector if an error occurs
    pottery_vector_t temp;
    pottery_vector_init(&temp);
    pottery_error_t error = pottery_vector_impl_copy(&temp, other);
    if (error == POTTERY_OK)
        pottery_vector_swap(&temp, vector);
    pottery_vector_destroy(&temp);
    return error;
}
#endif // CAN_DESTROY
#endif // CAN_INIT_COPY

POTTERY_VECTOR_EXTERN
void pottery_vector_init_steal(pottery_vector_t* vector, pottery_vector_t* other) {
    pottery_vector_init(vector);
    pottery_vector_swap(vector, other);
}

POTTERY_VECTOR_EXTERN
void pottery_vector_steal(pottery_vector_t* vector, pottery_vector_t* other) {

    // If we can clear ourselves before swapping, we do so in order to destroy
    // the contents as early as possible. If we can't, the user will have to
    // destroy the contents, so we leave them in the other vector.
    #if POTTERY_LIFECYCLE_CAN_DESTROY
    pottery_vector_remove_all(vector);
    #endif

    pottery_vector_swap(vector, other);
}
