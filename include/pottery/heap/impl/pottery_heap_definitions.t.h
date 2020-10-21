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

#ifndef POTTERY_HEAP_IMPL
#error "This is an internal header. Do not include it."
#endif

/*
 * Helpers to wrap configuration
 */

static pottery_always_inline
pottery_value_t* pottery_access(pottery_state_t state, size_t index) {
    #ifndef POTTERY_HEAP_ACCESS
        return &state.accessor[index];
    #elif defined(POTTERY_HEAP_CONTEXT_TYPE)
        return (POTTERY_HEAP_ACCESS(state.accessor, index, state.context));
    #else
        return (POTTERY_HEAP_ACCESS(state.accessor, index));
    #endif
}

static pottery_always_inline
bool pottery_heap_before(pottery_state_t state, pottery_value_t* left, pottery_value_t* right) {
    // max-heap means greater elements come before lesser ones.
    #if POTTERY_HEAP_CONTEXT_IS_COMPARE_CONTEXT
        return pottery_heap_compare_greater(state.accessor, left, right);
    #elif defined(POTTERY_HEAP_COMPARE_CONTEXT_TYPE)
        return pottery_heap_compare_greater(state.compare_context, left, right);
    #else
        (void)state;
        return pottery_heap_compare_greater(left, right);
    #endif
}

static pottery_always_inline
void pottery_heap_set_index(pottery_state_t state, pottery_value_t* value, size_t index) {
    (void)state;

    #ifdef POTTERY_HEAP_SET_INDEX
        #ifdef POTTERY_HEAP_CONTEXT_TYPE
            (POTTERY_HEAP_SET_INDEX((value), (index), state.context));
        #else
            (POTTERY_HEAP_SET_INDEX((value), (index)));
        #endif
    #else
    (void)value;
    (void)index;
    #endif
}

#if POTTERY_HEAP_USE_MOVE
// Moves from to to.
static pottery_always_inline
void pottery_heap_move(pottery_state_t state,
        pottery_heap_ref_t to, pottery_heap_ref_t from)
{
    #if POTTERY_HEAP_CONTEXT_IS_LIFECYCLE_CONTEXT
        pottery_heap_lifecycle_move(state.accessor, to, from);
    #elif defined(POTTERY_HEAP_LIFECYCLE_CONTEXT_TYPE)
        pottery_heap_lifecycle_move(state.lifecycle_context, to, from);
    #else
        (void)state;
        pottery_heap_lifecycle_move(to, from);
    #endif
}
#endif

#if !POTTERY_HEAP_USE_MOVE
// Swaps left and right.
static pottery_always_inline
void pottery_heap_swap(pottery_state_t state,
        pottery_heap_ref_t left, pottery_heap_ref_t right)
{
    #if POTTERY_HEAP_CONTEXT_IS_LIFECYCLE_CONTEXT
        pottery_heap_lifecycle_swap(state.accessor, left, right);
    #elif defined(POTTERY_HEAP_LIFECYCLE_CONTEXT_TYPE)
        pottery_heap_lifecycle_swap(state.lifecycle_context, left, right);
    #else
        (void)state;
        pottery_heap_lifecycle_swap(left, right);
    #endif
}
#endif

/*
 * Internal functions
 */

static
size_t pottery_heap_parent(size_t index) {
    pottery_assert(index != 0);
    return (index - 1) / 2;
}

static
size_t pottery_heap_child_left(size_t index) {
    // TODO check overflow. we don't check that this is in bounds before calling.
    return 2 * index + 1;
}

static
size_t pottery_heap_child_right(size_t index) {
    // TODO check overflow. we don't check that this is in bounds before calling.
    return 2 * index + 2;
}

static
void pottery_heap_sift_down(pottery_state_t state, size_t count,
        size_t index)
{
    //mlogV("starting to sift down");

    while (true) {

        // Grab the first child.
        size_t child_index = pottery_heap_child_left(index);
        if (child_index >= count) {
            // Node has no children; nothing left to do.
            break;
        }
        pottery_value_t* current = pottery_access(state, index);
        pottery_value_t* child = pottery_access(state, child_index);

        // See if we have a second child. If it comes before the first, we'll
        // use that one instead.
        size_t right_index = pottery_heap_child_right(index);
        if (right_index < count) {
            pottery_value_t* right = pottery_access(state, right_index);
            if (pottery_heap_before(state, right, child)) {
                child = right;
                child_index = right_index;
            }
        }

        // If we come before both children, we're done. (We negate here to
        // optimize for equal elements.)
        if (!pottery_heap_before(state, child, current))
            break;

        // Otherwise we swap ourselves with the child and loop around to keep
        // sifting.
        pottery_heap_swap(state, current, child);
        index = child_index;
    }
}

static
void pottery_heap_sift_up(pottery_state_t state, size_t index)
{
    //mlogV("starting to sift up");

    while (index != 0) {
        size_t parent_index = pottery_heap_parent(index);
        pottery_value_t* current = pottery_access(state, index);
        pottery_value_t* parent = pottery_access(state, parent_index);

        // If we come after our parent, we're done.
        if (!pottery_heap_before(state, current, parent))
            break;

        pottery_heap_swap(state, current, parent);
        index = parent_index;
    }
}

POTTERY_HEAP_EXTERN
void pottery_heap_build_impl(pottery_state_t state, size_t count) {
    if (count <= 1)
        return;
    for (size_t i = pottery_heap_parent(count - 1) + 1; i > 0;) {
        --i;
        pottery_heap_sift_down(state, count, i);
    }
}

POTTERY_HEAP_EXTERN
void pottery_heap_push_impl(pottery_state_t state, size_t current_count, size_t push_count) {

    // If we're inserting a sizeable chunk of elements, it's faster to append
    // in bulk and rebuild. This fraction could be a tunable parameter.
    if (push_count > current_count) {
        pottery_heap_build_impl(state, current_count + push_count);
        return;
    }

    // Otherwise we insert one by one.
    for (; push_count > 0; --push_count) {
        size_t new_index = current_count++;
        pottery_value_t* new_value = pottery_access(state, new_index);
        pottery_heap_set_index(state, new_value, new_index);
        pottery_heap_sift_up(state, new_index);
    }
}

POTTERY_HEAP_EXTERN
void pottery_heap_pop_impl(pottery_state_t state, size_t current_count, size_t pop_count) {
    for (; pop_count > 0; --pop_count) {
        size_t new_count = --current_count;
        pottery_value_t* first = pottery_access(state, 0);
        pottery_value_t* last = pottery_access(state, new_count);
        pottery_heap_swap(state, first, last);
        pottery_heap_sift_down(state, new_count, 0);
    }
}

POTTERY_HEAP_EXTERN
void pottery_heap_remove_impl(pottery_state_t state, size_t current_count, size_t index_to_remove)
{
    if (current_count == 1) {
        pottery_assert(index_to_remove == 0);
        return;
    }

    // Replace the value with the last value in the heap
    size_t new_count = current_count - 1;
    pottery_value_t* item_to_remove = pottery_access(state, index_to_remove);
    pottery_value_t* last = pottery_access(state, new_count);
    pottery_heap_swap(state, item_to_remove, last);
    pottery_value_t* removed = pottery_access(state, new_count);
    pottery_value_t* replaced = pottery_access(state, index_to_remove);

    // Sift it up or down depending on what we're replacing it with
    if (pottery_heap_before(state, removed, replaced))
        pottery_heap_sift_down(state, new_count, index_to_remove);
    else
        pottery_heap_sift_up(state, index_to_remove);
}

POTTERY_HEAP_EXTERN
size_t pottery_heap_valid_count_impl(pottery_state_t state, size_t count) {
    if (count <= 1)
        return count;
    for (size_t index = 1; index < count; ++index) {
        size_t parent_index = pottery_heap_parent(index);
        pottery_value_t* current = pottery_access(state, index);
        pottery_value_t* parent = pottery_access(state, parent_index);
        if (!pottery_heap_before(state, parent, current)) {
            return index;
        }
    }
    return count;
}

POTTERY_HEAP_EXTERN
bool pottery_heap_valid_impl(pottery_state_t state, size_t count) {
    return count == pottery_heap_valid_count_impl(state, count);
}
