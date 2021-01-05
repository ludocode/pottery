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

#ifndef POTTERY_HEAP_IMPL
#error "This is an internal header. Do not include it."
#endif

/*
 * Helpers to wrap configuration
 */

static pottery_always_inline
pottery_heap_ref_t pottery_heap_access(pottery_heap_state_t state, size_t index) {
    #ifndef POTTERY_HEAP_ACCESS
        // With no defined access expression, it's a simple array.
        return state.first + index;
    #elif defined(POTTERY_HEAP_CONTEXT_TYPE)
        return POTTERY_HEAP_ACCESS(state.context, state.first, index);
    #else
        return POTTERY_HEAP_ACCESS(state.first, index);
    #endif
}

static pottery_always_inline
void pottery_heap_set_index(pottery_heap_state_t state, pottery_heap_ref_t value, size_t index) {
    (void)state;

    #ifdef POTTERY_HEAP_SET_INDEX
        #ifdef POTTERY_HEAP_CONTEXT_TYPE
            (POTTERY_HEAP_SET_INDEX((state.context), (value), (index)));
        #else
            (POTTERY_HEAP_SET_INDEX((value), (index)));
        #endif
    #else
    (void)value;
    (void)index;
    #endif
}

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
    return 2 * index + 1;
}

static
size_t pottery_heap_child_right(size_t index) {
    return 2 * index + 2;
}

static
void pottery_heap_sift_down(pottery_heap_state_t state, size_t count, size_t index) {
    //mlogV("starting to sift down");

    while (true) {

        // Grab the first child.
        size_t child_index = pottery_heap_child_left(index);
        if (child_index >= count) {
            // Node has no children; nothing left to do.
            break;
        }
        pottery_heap_ref_t current = pottery_heap_access(state, index);
        pottery_heap_ref_t child = pottery_heap_access(state, child_index);

        // See if we have a second child. If it comes before the first, we'll
        // use that one instead.
        size_t right_index = pottery_heap_child_right(index);
        if (right_index < count) {
            pottery_heap_ref_t right = pottery_heap_access(state, right_index);
            if (pottery_heap_compare_greater(POTTERY_HEAP_CONTEXT_VAL(state) right, child)) {
                child = right;
                child_index = right_index;
            }
        }

        // If we come before both children, we're done.
        if (pottery_heap_compare_greater_or_equal(POTTERY_HEAP_CONTEXT_VAL(state) current, child))
            break;

        // Otherwise we swap ourselves with the child and loop around to keep
        // sifting.
        pottery_heap_lifecycle_swap_restrict(POTTERY_HEAP_CONTEXT_VAL(state) current, child);
        index = child_index;
    }
}

static
void pottery_heap_sift_up(pottery_heap_state_t state, size_t index) {
    //mlogV("starting to sift up");

    while (index != 0) {
        size_t parent_index = pottery_heap_parent(index);
        pottery_heap_ref_t current = pottery_heap_access(state, index);
        pottery_heap_ref_t parent = pottery_heap_access(state, parent_index);

        // If we come after our parent, we're done.
        if (pottery_heap_compare_greater_or_equal(POTTERY_HEAP_CONTEXT_VAL(state) parent, current))
            break;

        pottery_heap_lifecycle_swap_restrict(POTTERY_HEAP_CONTEXT_VAL(state) current, parent);
        index = parent_index;
    }
}

POTTERY_HEAP_EXTERN
void pottery_heap_build_impl(pottery_heap_state_t state, size_t count) {
    if (count <= 1)
        return;

    // We limit the heap size to eliminate overflow checks and prevent any
    // out-of-bounds accesses. It's probably not practically possible to have a
    // heap this size.
    if (count > SIZE_MAX / 4)
        pottery_abort();

    size_t i;
    for (i = pottery_heap_parent(count - 1) + 1; i > 0;) {
        --i;
        pottery_heap_sift_down(state, count, i);
    }
}

POTTERY_HEAP_EXTERN
void pottery_heap_expand_bulk_impl(pottery_heap_state_t state, size_t current_count, size_t expand_count) {

    // bounds checks
    size_t new_total = current_count + expand_count;
    if (new_total < current_count)
        pottery_abort();
    if (new_total > SIZE_MAX / 4)
        pottery_abort();

    // If we're inserting a sizeable chunk of elements, it's faster to append
    // in bulk and rebuild. This fraction could be a tunable parameter.
    if (expand_count > current_count) {
        pottery_heap_build_impl(state, current_count + expand_count);
        return;
    }

    // Otherwise we insert one by one.
    for (; expand_count > 0; --expand_count) {
        size_t new_index = current_count++;
        pottery_heap_ref_t new_value = pottery_heap_access(state, new_index);
        pottery_heap_set_index(state, new_value, new_index);
        pottery_heap_sift_up(state, new_index);
    }
}

POTTERY_HEAP_EXTERN
void pottery_heap_contract_bulk_impl(pottery_heap_state_t state, size_t current_count, size_t pop_count) {
    pottery_assert(pop_count <= current_count);
    if (current_count > SIZE_MAX / 4)
        pottery_abort();

    for (; pop_count > 0; --pop_count) {
        size_t new_count = --current_count;
        if (new_count == 0)
            return;
        pottery_heap_ref_t first = pottery_heap_access(state, 0);
        pottery_heap_ref_t last = pottery_heap_access(state, new_count);
        pottery_heap_lifecycle_swap_restrict(POTTERY_HEAP_CONTEXT_VAL(state) first, last);
        pottery_heap_sift_down(state, new_count, 0);
    }
}

POTTERY_HEAP_EXTERN
void pottery_heap_contract_at_impl(pottery_heap_state_t state, size_t current_count, size_t index_to_contract) {
    if (current_count == 1) {
        pottery_assert(index_to_contract == 0);
        return;
    }

    if (current_count > SIZE_MAX / 4)
        pottery_abort();
    pottery_assert(index_to_contract < current_count);

    size_t new_count = current_count - 1;
    if (index_to_contract == new_count)
        return;

    // Replace the value with the last value in the heap
    pottery_heap_ref_t item_to_remove = pottery_heap_access(state, index_to_contract);
    pottery_heap_ref_t last = pottery_heap_access(state, new_count);
    pottery_heap_lifecycle_swap_restrict(POTTERY_HEAP_CONTEXT_VAL(state) item_to_remove, last);
    pottery_heap_ref_t removed = pottery_heap_access(state, new_count);
    pottery_heap_ref_t replaced = pottery_heap_access(state, index_to_contract);

    // Sift it up or down depending on what we're replacing it with
    if (pottery_heap_compare_greater(POTTERY_HEAP_CONTEXT_VAL(state) removed, replaced))
        pottery_heap_sift_down(state, new_count, index_to_contract);
    else
        pottery_heap_sift_up(state, index_to_contract);
}

POTTERY_HEAP_EXTERN
size_t pottery_heap_valid_count_impl(pottery_heap_state_t state, size_t count) {
    if (count <= 1)
        return count;
    if (count > SIZE_MAX / 4)
        pottery_abort();

    size_t index;
    for (index = 1; index < count; ++index) {
        size_t parent_index = pottery_heap_parent(index);
        pottery_heap_ref_t current = pottery_heap_access(state, index);
        pottery_heap_ref_t parent = pottery_heap_access(state, parent_index);
        if (pottery_heap_compare_less(POTTERY_HEAP_CONTEXT_VAL(state) parent, current)) {
            return index;
        }
    }
    return count;
}
