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

#ifndef POTTERY_INSERTION_SORT_IMPL
#error "This is an internal header. Do not include it."
#endif

/*
 * Helpers to wrap configuration
 */

typedef struct pottery_insertion_sort_state_t {
    pottery_insertion_sort_context_t context;
    #if POTTERY_INSERTION_SORT_SEPARATE_LIFECYCLE_CONTEXT
    pottery_insertion_sort_lifecycle_context_t lifecycle_context;
    #endif
    #if POTTERY_INSERTION_SORT_SEPARATE_COMPARE_CONTEXT
    pottery_insertion_sort_compare_context_t compare_context;
    #endif
} pottery_insertion_sort_state_t;

// Gets the ref for the element at index.
static pottery_always_inline
pottery_insertion_sort_ref_t pottery_insertion_sort_access(pottery_insertion_sort_state_t state, size_t index) {
    #ifndef POTTERY_INSERTION_SORT_ACCESS
        // With no defined access expression, the context is a simple array.
        return state.context + index;
    #else
        return POTTERY_INSERTION_SORT_ACCESS(state.context, index);
    #endif
}

// Compares left and right; returns true if left comes before right.
static pottery_always_inline
bool pottery_insertion_sort_before(pottery_insertion_sort_state_t state,
        pottery_insertion_sort_ref_t left, pottery_insertion_sort_ref_t right)
{
    #if POTTERY_INSERTION_SORT_CONTEXT_IS_COMPARE_CONTEXT
        return pottery_insertion_sort_compare_less(state.context, left, right);
    #elif defined(POTTERY_INSERTION_SORT_COMPARE_CONTEXT_TYPE)
        return pottery_insertion_sort_compare_less(state.compare_context, left, right);
    #else
        (void)state;
        return pottery_insertion_sort_compare_less(left, right);
    #endif
}

#if POTTERY_INSERTION_SORT_USE_MOVE
// Moves from to to.
static pottery_always_inline
void pottery_insertion_sort_move(pottery_insertion_sort_state_t state,
        pottery_insertion_sort_ref_t to, pottery_insertion_sort_ref_t from)
{
    #if POTTERY_INSERTION_SORT_CONTEXT_IS_LIFECYCLE_CONTEXT
        pottery_insertion_sort_lifecycle_move(state.context, to, from);
    #elif defined(POTTERY_INSERTION_SORT_LIFECYCLE_CONTEXT_TYPE)
        pottery_insertion_sort_lifecycle_move(state.lifecycle_context, to, from);
    #else
        (void)state;
        pottery_insertion_sort_lifecycle_move(to, from);
    #endif
}
#endif

#if !POTTERY_INSERTION_SORT_USE_MOVE
// Swaps left and right.
static pottery_always_inline
void pottery_insertion_sort_swap(pottery_insertion_sort_state_t state,
        pottery_insertion_sort_ref_t left, pottery_insertion_sort_ref_t right)
{
    #if POTTERY_INSERTION_SORT_CONTEXT_IS_LIFECYCLE_CONTEXT
        pottery_insertion_sort_lifecycle_swap(state.context, left, right);
    #elif defined(POTTERY_INSERTION_SORT_LIFECYCLE_CONTEXT_TYPE)
        pottery_insertion_sort_lifecycle_swap(state.lifecycle_context, left, right);
    #else
        (void)state;
        pottery_insertion_sort_lifecycle_swap(left, right);
    #endif
}
#endif



/*
 * Implementation
 */

// There are actually two implementations of insertion sort here, one that
// moves through a temporary and one that swaps.

#if POTTERY_INSERTION_SORT_USE_MOVE
static inline
void pottery_insertion_sort_by_move(pottery_insertion_sort_state_t state, size_t count) {
    POTTERY_DECLARE_UNCONSTRUCTED(pottery_insertion_sort_value_t, temp);

    size_t i;
    for (i = 1; i < count; ++i) {
        pottery_insertion_sort_ref_t current = pottery_insertion_sort_access(state, i);
        pottery_insertion_sort_ref_t previous = pottery_insertion_sort_access(state, i - 1);

        if (pottery_insertion_sort_before(state, current, previous)) {
            pottery_insertion_sort_move(state, temp, current);
            pottery_insertion_sort_move(state, current, previous);

            size_t j = i - 1;
            for (;;) {
                current = previous;
                if (j == 0)
                    break;
                previous = pottery_insertion_sort_access(state, --j);
                if (!pottery_insertion_sort_before(state, temp, previous))
                    break;
                pottery_insertion_sort_move(state, current, previous);
            }

            pottery_insertion_sort_move(state, current, temp);
        }
    }
}
#endif

#if !POTTERY_INSERTION_SORT_USE_MOVE
static inline
void pottery_insertion_sort_by_swap(pottery_insertion_sort_state_t state, size_t count) {
    size_t i;
    for (i = 1; i < count; ++i) {
        pottery_insertion_sort_ref_t current = pottery_insertion_sort_access(state, i);

        size_t j;
        for (j = i; j > 0; --j) {
            pottery_insertion_sort_ref_t previous = pottery_insertion_sort_access(state, j - 1);
            if (!pottery_insertion_sort_before(state, current, previous))
                break;
            pottery_insertion_sort_swap(state, current, previous);
            current = previous;
        }
    }
}
#endif

/*
 * External functions
 */

POTTERY_INSERTION_SORT_EXTERN
void pottery_insertion_sort(
        pottery_insertion_sort_context_t context,
        size_t count
        #if POTTERY_INSERTION_SORT_SEPARATE_LIFECYCLE_CONTEXT
        , pottery_insertion_sort_lifecycle_context_t lifecycle_context
        #endif
        #if POTTERY_INSERTION_SORT_SEPARATE_COMPARE_CONTEXT
        , pottery_insertion_sort_compare_context_t compare_context
        #endif
) {
    pottery_insertion_sort_state_t state = {
        context,
        #if POTTERY_INSERTION_SORT_SEPARATE_LIFECYCLE_CONTEXT
        lifecycle_context,
        #endif
        #if POTTERY_INSERTION_SORT_SEPARATE_COMPARE_CONTEXT
        compare_context,
        #endif
    };

    #if POTTERY_INSERTION_SORT_USE_MOVE
    pottery_insertion_sort_by_move(state, count);
    #else
    pottery_insertion_sort_by_swap(state, count);
    #endif
}
