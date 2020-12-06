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
            pottery_alignof(pottery_vector_element_t),
            elements);
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

    // NOTE: We create the space at the end even if the vector is double-ended
    // under the assumption that you're about to append this many elements.
    // This means that a double-ended vector can be very unbalanced after a
    // reserve. This is probably a good thing, because otherwise if you reserve
    // a large number of elements and then append that same number, it would
    // have to shift them down part way through, and the whole point of reserve
    // is to avoid having to do that.

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
static void pottery_vector_impl_destroy_all(pottery_vector_t* vector) {
    pottery_vector_lifecycle_destroy_bulk(POTTERY_VECTOR_CONTEXT_VAL(vector)
            pottery_vector_begin(vector), pottery_vector_count(vector));
}
#endif

POTTERY_VECTOR_EXTERN
void pottery_vector_impl_reset(pottery_vector_t* vector) {
    vector->count = 0;

    #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
        vector->storage = vector->u.internal;
    #else
        vector->storage = pottery_null;
        vector->u.capacity = 0;
    #endif

    #if POTTERY_VECTOR_DOUBLE_ENDED
    // We set the start offset to the start of the allocation. If you append
    // right away, we expect you to append more, so it's probably better to be
    // at the start. If you're prepending, the second prepend will recenter
    // so it'll only have to move one value.
    vector->begin = vector->storage;
    #endif
}

// Clears the vector after all its elements have been destroyed.
static void pottery_vector_impl_clear(pottery_vector_t* vector) {
    #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
    if (vector->storage == vector->u.internal) {
        pottery_vector_impl_reset(vector);
        return;
    }
    #endif

    // We only free the vector if we're using more than double the minimum
    // space. This will prevent us from continually allocating and freeing if
    // the vector is alternating between empty and a small number of elements.
    // This is another parameter that should eventually be tunable.

    if (vector->u.capacity > 2 * pottery_vector_minimum_capacity()) {
        pottery_vector_impl_free(vector, vector->storage);
        pottery_vector_impl_reset(vector);
    } else {
        vector->count = 0;

        #if POTTERY_VECTOR_DOUBLE_ENDED
        // Since we're keeping a non-trivial amount of space, we set the start
        // offset a quarter of the way through. If you only append, we won't
        // waste too much space before growing. If you only prepend, we'll
        // recenter without having to grow and before filling too much of the
        // vector.
        vector->begin = vector->storage + vector->u.capacity / 4;
        #endif
    }
}

POTTERY_VECTOR_EXTERN
void pottery_vector_displace_all(pottery_vector_t* vector) {
    pottery_vector_impl_clear(vector);
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_VECTOR_EXTERN
void pottery_vector_remove_all(pottery_vector_t* vector) {
    pottery_vector_impl_destroy_all(vector);
    pottery_vector_impl_clear(vector);
}
#endif

POTTERY_VECTOR_EXTERN
void pottery_vector_destroy(pottery_vector_t* vector) {
    // make sure the container is not already destroyed or otherwise invalid
    pottery_assert(vector->storage != pottery_reinterpret_cast(pottery_vector_element_t*, -1));

    // Clean up the checks
    #if POTTERY_DEBUG
        #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
            pottery_assert(vector->self_check == vector);
        #endif
        #if POTTERY_LEAK_CHECK
            pottery_assert(vector->leak_check != pottery_null);
            free(vector->leak_check);
        #endif
    #endif

    #if POTTERY_LIFECYCLE_CAN_DESTROY
    pottery_vector_impl_destroy_all(vector);
    #else
    // If we don't know how to destroy our values, it is an error to destroy a
    // non-empty container. The vector contents must be properly disposed of.
    pottery_assert(vector->count == 0);
    #endif

    if (vector->storage != pottery_null
            #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
            && vector->storage != vector->u.internal
            #endif
    ) {
        pottery_vector_impl_free(vector, vector->storage);
    } else {
        #if POTTERY_DEBUG
        // If values is null or internal, assign it to garbage to detect
        // double-destroy. (If it was allocated, we expect you have some other
        // means of detecting double-free in debug builds so we leave it
        // as-is.)
        vector->storage = pottery_reinterpret_cast(pottery_vector_element_t*, -1);
        #endif
    }
}

POTTERY_VECTOR_EXTERN
void pottery_vector_move(pottery_vector_t* to, pottery_vector_t* from) {
    #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
    if (from->storage == from->u.internal) {

        // initialize the vector, which gives us sufficient internal space
        pottery_vector_init(to);
        pottery_assert(to->storage == to->u.internal);
        pottery_assert(pottery_vector_capacity(to) >= from->count);

        // take the opportunity to recenter
        to->count = from->count;
        #if POTTERY_VECTOR_DOUBLE_ENDED
        to->begin = to->storage + (pottery_vector_capacity(to) - to->count) / 2;
        #endif

        // move the elements
        pottery_vector_lifecycle_move_bulk_restrict(POTTERY_VECTOR_CONTEXT_VAL(from)
                pottery_vector_begin(to), pottery_vector_begin(from), to->count);
        from->count = 0;

        // move the context
        #ifdef POTTERY_VECTOR_CONTEXT_TYPE
        to->context = pottery_move_if_cxx(from->context);
        #endif

        // destroy the source vector
        pottery_vector_destroy(from);
    } else
    #endif
    {
        // steal the storage without init/destroy
        to->storage = from->storage;
        #if POTTERY_VECTOR_DOUBLE_ENDED
        to->begin = from->begin;
        #endif
        to->count = from->count;
        to->u.capacity = from->u.capacity;

        // also steal debug stuff
        #if POTTERY_DEBUG
            #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
                to->self_check = to;
                from->self_check = pottery_reinterpret_cast(pottery_vector_t*, -1);
            #endif
            #if POTTERY_LEAK_CHECK
                to->leak_check = from->leak_check;
                from->leak_check = pottery_reinterpret_cast(void*, -1);
            #endif
        #endif

        // steal the context
        #if defined(POTTERY_VECTOR_ALLOC_CONTEXT_TYPE) && !defined(POTTERY_VECTOR_ALLOC_CONTEXT)
        to->alloc_context = pottery_move_if_cxx(from->alloc_context);
        #endif

        // As in destroy(), help detect double-destroy.
        #if POTTERY_DEBUG
        if (from->storage == pottery_null
                #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
                || from->storage == from->u.internal
                #endif
                )
            from->storage = pottery_reinterpret_cast(pottery_vector_element_t*, -1);
        #endif
    }
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_VECTOR_EXTERN
void pottery_vector_remove_at(pottery_vector_t* vector, size_t index) {
    pottery_vector_lifecycle_destroy(POTTERY_VECTOR_CONTEXT_VAL(vector)
            pottery_vector_at(vector, index));
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
    // Make sure the arguments are valid.
    if (count == 0) {
        *elements = pottery_reinterpret_cast(pottery_vector_element_t*, -1);
        return POTTERY_OK; // no elements
    }
    if (vector->count + count < vector->count) {
        *elements = pottery_reinterpret_cast(pottery_vector_element_t*, -1);
        return POTTERY_ERROR_OVERFLOW;
    }

    size_t old_count = vector->count;
    pottery_assert(index <= old_count);
    size_t new_count = old_count + count;
    size_t old_capacity = pottery_vector_capacity(vector);

    // When moving a double-ended vector, we only move one side of the elements
    // in-place if we would have to move less than half of them. If we have to
    // move more than half, we either recenter or grow.

    // See if we have enough space to move the after elements up
    bool can_move_up = new_count <= old_capacity
            #if POTTERY_VECTOR_DOUBLE_ENDED
            - pottery_cast(size_t, (vector->begin - vector->storage))
            && index > vector->count / 2
            #endif
            ;
#if 0&&POTTERY_VECTOR_DOUBLE_ENDED
printf("old_count %zi capacity %zi begin %zi\n",old_count,
    old_capacity,pottery_cast(size_t, (vector->begin - vector->storage)));
#endif

    // For a double-ended vector, see if we have enough space to move the
    // before elements down, and do it if it will move less elements than up
    #if POTTERY_VECTOR_DOUBLE_ENDED
    if (count <= pottery_cast(size_t, (vector->begin - vector->storage)) &&
            index < vector->count / 2 &&
            (!can_move_up || index < vector->count - index))
    {
        pottery_vector_element_t* new_begin = vector->begin - count;
        //printf("moving %zi elements down\n", index);
        pottery_vector_lifecycle_move_bulk_down(POTTERY_VECTOR_CONTEXT_VAL(vector)
                new_begin, vector->begin, index);
        vector->begin = new_begin;
        vector->count += count;
        *elements = vector->begin + index;
        return POTTERY_OK;
    }
    #endif

    // Move after elements up
    if (can_move_up) {
        //printf("moving %zi elements up\n", vector->count - index);
        pottery_vector_lifecycle_move_bulk_up(POTTERY_VECTOR_CONTEXT_VAL(vector)
                pottery_vector_begin(vector) + index + count,
                pottery_vector_begin(vector) + index,
                vector->count - index);
        vector->count += count;
        *elements = pottery_vector_begin(vector) + index;
        return POTTERY_OK;
    }

    #if POTTERY_VECTOR_DOUBLE_ENDED
    // Recenter. We only recenter if the vector is at most half full.
    if (new_count <= old_capacity && old_count < old_capacity / 2) {
        pottery_vector_element_t* new_begin = vector->storage + (old_capacity - new_count) / 2;

        if (new_begin == vector->begin) {
            // Only move after elements up
            pottery_vector_lifecycle_move_bulk_up(POTTERY_VECTOR_CONTEXT_VAL(vector)
                    new_begin + index + count, vector->begin + index, vector->count - index);

        } else if (new_begin < vector->begin) {
            // Move start down, move end up or down
            pottery_vector_lifecycle_move_bulk_down(POTTERY_VECTOR_CONTEXT_VAL(vector)
                    new_begin, vector->begin, index);
            pottery_vector_lifecycle_move_bulk(POTTERY_VECTOR_CONTEXT_VAL(vector)
                    new_begin + index + count, vector->begin + index, vector->count - index);

        } else {
            // Move start and end up
            pottery_vector_lifecycle_move_bulk_up(POTTERY_VECTOR_CONTEXT_VAL(vector)
                    new_begin + index + count, vector->begin + index, vector->count - index);
            pottery_vector_lifecycle_move_bulk_up(POTTERY_VECTOR_CONTEXT_VAL(vector)
                    new_begin, vector->begin, index);
        }

        vector->begin = new_begin;
        vector->count += count;
        *elements = vector->begin + index;
        return POTTERY_OK;
    }
    #endif

    // All attempts at moving in place failed, so we grow.

    // Calculate new capacity.
    // We grow by a factor of 1.5. This is apparently best practice now. It
    // would be nice to actually benchmark this (especially in the case of a
    // double-ended vector.)
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

    // TODO if moves by value and not double-ended, use realloc_at_least

    pottery_vector_element_t* new_storage = pottery_vector_impl_alloc(vector, &new_capacity);
    if (new_storage == pottery_null)
        return POTTERY_ERROR_ALLOC;

    // Center in the new storage if double-ended
    pottery_vector_element_t* new_begin = new_storage;
    #if POTTERY_VECTOR_DOUBLE_ENDED
    new_begin += (new_capacity - new_count) / 2;
    #endif

    if (vector->storage != pottery_null) {
        pottery_vector_lifecycle_move_bulk_restrict(POTTERY_VECTOR_CONTEXT_VAL(vector)
                new_begin,
                pottery_vector_begin(vector),
                index);
        pottery_vector_lifecycle_move_bulk_restrict(POTTERY_VECTOR_CONTEXT_VAL(vector)
                new_begin + index + count,
                pottery_vector_begin(vector) + index,
                old_count - index);

        #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
        if (vector->storage != vector->u.internal)
        #endif
        {
            pottery_vector_impl_free(vector, vector->storage);
        }
    }

    vector->storage = new_storage;
    #if POTTERY_VECTOR_DOUBLE_ENDED
    vector->begin = new_begin;
    #endif
    vector->u.capacity = new_capacity;
    vector->count = new_count;
    *elements = pottery_bless(pottery_vector_element_t, new_begin + index);
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

    pottery_vector_element_t* end = inserted + count;
    while (inserted != end) {
        // Use move construction (rather than the lifecycle move() operation)
        // to move values out of the array. We need to leave them constructed
        // in C++. See notes in pottery_vector_extract() for more details.
        // TODO add lifecycle function move_construct_bulk?
        *(inserted++) = pottery_move_if_cxx(*(values++));
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

    #if POTTERY_VECTOR_AUTO_SHRINK
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
    should_shrink &= vector->storage != vector->u.internal;
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

        // If we have no elements, shrink to nothing.
        if (new_count == 0) {
            pottery_vector_impl_free(vector, vector->storage);
            pottery_vector_impl_reset(vector);
            return;
        }

        pottery_vector_element_t* new_storage;
        pottery_vector_element_t* new_begin;

        // TODO if moves by value and not double-ended, use realloc_at_least

        #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
        if (new_capacity <= POTTERY_VECTOR_INTERNAL_CAPACITY) {
            // switch back to internal space
            new_storage = vector->u.internal;

            // For a double-ended vector, as with reset, we start at the start
            // of the internal space. (For single-ended it's always at the
            // start)
            new_begin = new_storage;
        } else
        #endif
        {
            // allocate new smaller space
            size_t expanded_capacity = new_capacity;
            new_storage = pottery_vector_impl_alloc(vector, &expanded_capacity);

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

            new_begin = new_storage;
            #if POTTERY_VECTOR_DOUBLE_ENDED
            // Center our data in the new storage
            new_begin += (new_capacity - new_count) / 2;
            #endif
        }

        if (new_storage != pottery_null) {
            // move values into new space
            pottery_vector_lifecycle_move_bulk_restrict(POTTERY_VECTOR_CONTEXT_VAL(vector)
                    new_begin,
                    pottery_vector_begin(vector),
                    index);
            pottery_vector_lifecycle_move_bulk_restrict(POTTERY_VECTOR_CONTEXT_VAL(vector)
                    new_begin + index,
                    pottery_vector_begin(vector) + index + count,
                    new_count - index);

            pottery_vector_impl_free(vector, vector->storage);
            vector->storage = new_storage;

            #if POTTERY_VECTOR_DOUBLE_ENDED
            vector->begin = new_begin;
            #endif

            #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
            if (new_storage != vector->u.internal)
            #endif
            {
                vector->u.capacity = new_capacity;
            }

            return;
        }

        // If for whatever reason allocation of a smaller buffer failed, we
        // don't report error from remove/displace; we just keep the larger
        // buffer.
    }
    #endif

    // No shrink. Move in-place.

    // If we have less elements before the removed elements than after them,
    // move elements up in-place
    #if POTTERY_VECTOR_DOUBLE_ENDED
    if (index < new_count / 2) {
        pottery_vector_element_t* new_begin = vector->begin + count;
        pottery_vector_lifecycle_move_bulk_up(POTTERY_VECTOR_CONTEXT_VAL(vector)
                new_begin, vector->begin, index);
        vector->begin = new_begin;
        return;
    }
    #endif

    // Move elements down in-place
    pottery_vector_lifecycle_move_bulk_down(POTTERY_VECTOR_CONTEXT_VAL(vector)
            pottery_vector_begin(vector) + index,
            pottery_vector_begin(vector) + index + count,
            new_count - index);
}

POTTERY_VECTOR_EXTERN
pottery_error_t pottery_vector_shrink(pottery_vector_t* vector) {
    #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
    if (vector->storage == vector->u.internal) {
        // We're already on internal capacity. Nothing to do.
        return POTTERY_OK;
    }
    #endif

    size_t count = pottery_vector_count(vector);
    if (count == 0) {
        // The vector is empty. Free any allocated buffer and reset (to either
        // null or internal capacity.)
        if (vector->storage != pottery_null
                #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
                && vector->storage != vector->u.internal
                #endif
        ) {
            pottery_vector_impl_free(vector, vector->storage);
        }
        pottery_vector_impl_reset(vector);
        return POTTERY_OK;
    }

    size_t old_capacity = pottery_vector_capacity(vector);
    size_t new_capacity = count;
    pottery_vector_element_t* new_storage;

    #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
    if (new_capacity <= POTTERY_VECTOR_INTERNAL_CAPACITY) {
        // Switch to internal capacity
        new_storage = vector->u.internal;
        new_capacity = POTTERY_VECTOR_INTERNAL_CAPACITY;
    } else
    #endif
    {
        // Round new capacity up to a multiple of 16 to avoid trying to shrink
        // something the allocator would just round up. (We expect our alloc
        // helpers to do something similar anyway via MALLOC_GOOD_SIZE. Maybe
        // the alloc helpers should expose this function so we can use it
        // here.)
        size_t mask = 15;
        new_capacity = (new_capacity + mask) & ~mask;

        // We don't bother shrinking if we're already taking up some threshold of
        // the capacity. This factor could be tweaked.
        if (new_capacity >= old_capacity - old_capacity / 8) {
            // We're already at capacity, or close enough to it. Nothing to do.
            return POTTERY_OK;
        }

        // Allocate a buffer at least this big
        new_storage = pottery_vector_impl_alloc(vector, &new_capacity);
        if (new_storage == pottery_null)
            return POTTERY_ERROR_ALLOC;
        if (new_capacity >= old_capacity) {
            // The new allocation wasn't any smaller than our current allocation.
            pottery_vector_impl_free(vector, new_storage);
            return POTTERY_OK;
        }
        vector->u.capacity = new_capacity;
    }

    pottery_vector_element_t* new_begin = new_storage;
    #if POTTERY_VECTOR_DOUBLE_ENDED
    // Center our data in the new storage
    new_begin += (new_capacity - count) / 2;
    #endif

    pottery_vector_lifecycle_move_bulk_restrict(POTTERY_VECTOR_CONTEXT_VAL(vector)
            new_begin, pottery_vector_begin(vector), count);
    pottery_vector_impl_free(vector, vector->storage);
    vector->storage = new_storage;
    #if POTTERY_VECTOR_DOUBLE_ENDED
    vector->begin = new_begin;
    #endif
    return POTTERY_OK;
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_VECTOR_EXTERN
void pottery_vector_remove_at_bulk(pottery_vector_t* vector, size_t index, size_t count) {
    size_t i;
    for (i = index; i < index + count; ++i) {
        pottery_vector_lifecycle_destroy(POTTERY_VECTOR_CONTEXT_VAL(vector)
                pottery_vector_at(vector, i));
    }
    pottery_vector_impl_remove_space(vector, index, count);
}
#endif

POTTERY_VECTOR_EXTERN
void pottery_vector_swap(pottery_vector_t* left, pottery_vector_t* right) {
    #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
    if (left->storage == left->u.internal || right->storage == right->u.internal) {
        // If either vector is internal, we defer swap to move which will handle
        // it for us.
        POTTERY_DECLARE_UNCONSTRUCTED(pottery_vector_t, temp);
        pottery_vector_move(temp, left);
        pottery_vector_move(left, right);
        pottery_vector_move(right, temp);
        return;
    }
    #endif

    // Move bitwise. (For C++ we use std::move in case the context is non-trivial.)
    pottery_vector_t temp = pottery_move_if_cxx(*left);
    *left = pottery_move_if_cxx(*right);
    *right = pottery_move_if_cxx(temp);
}

#if POTTERY_LIFECYCLE_CAN_INIT_COPY
// Copies the contents of other into this where this is empty.
static
pottery_error_t pottery_vector_impl_copy(pottery_vector_t* vector, const pottery_vector_t* other) {
    pottery_assert(pottery_vector_is_empty(vector));
    size_t count = pottery_vector_count(other);

    pottery_vector_element_t* src = pottery_vector_begin(other);
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
    vector->alloc_context = pottery_move_if_cxx(other->alloc_context);
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
    // copy can't fail. This is only in C or in C++ with exceptions disabled.
    //
    // (If exceptions are enabled, individual by-value copies could fail, so we
    // can't do this. We could in theory detect whether the copy constructor is
    // noexcept but it's not worth doing.)
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

    // Copy to a temporary so that we don't damage the vector if an error
    // occurs.
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
    // TODO this could be implemented somewhat more efficiently, especially in
    // the case of internal storage since we don't need to move our values
    // back. This can be done later.
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
