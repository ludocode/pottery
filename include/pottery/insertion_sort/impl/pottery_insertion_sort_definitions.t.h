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

// Gets the ref for the element at index.
static pottery_always_inline
pottery_insertion_sort_ref_t pottery_insertion_sort_access(pottery_insertion_sort_state_t state, size_t index) {
    #ifndef POTTERY_INSERTION_SORT_ACCESS
        // With no defined access expression, it's a simple array.
        return state.first + index;
    #elif defined(POTTERY_INSERTION_SORT_CONTEXT_TYPE)
        return POTTERY_INSERTION_SORT_ACCESS(state.context, state.first, index);
    #else
        return POTTERY_INSERTION_SORT_ACCESS(state.first, index);
    #endif
}



/*
 * Implementation
 */

// There are actually two implementations of insertion sort here, one that
// moves through a temporary and one that swaps.

#ifndef POTTERY_INSERTION_SORT_USE_MOVE
    #if POTTERY_LIFECYCLE_CAN_MOVE && defined(POTTERY_INSERTION_SORT_VALUE_TYPE)
        #define POTTERY_INSERTION_SORT_USE_MOVE 1
    #else
        #define POTTERY_INSERTION_SORT_USE_MOVE 0
    #endif
#endif

#if POTTERY_INSERTION_SORT_USE_MOVE
static inline
void pottery_insertion_sort_impl(pottery_insertion_sort_state_t state, size_t count) {
    POTTERY_DECLARE_UNCONSTRUCTED(pottery_insertion_sort_value_t, temp);

    size_t i;
    for (i = 1; i < count; ++i) {
        pottery_insertion_sort_ref_t current = pottery_insertion_sort_access(state, i);
        pottery_insertion_sort_ref_t previous = pottery_insertion_sort_access(state, i - 1);

        if (pottery_insertion_sort_compare_less(POTTERY_INSERTION_SORT_CONTEXT_VAL(state) current, previous)) {
            pottery_insertion_sort_lifecycle_move(POTTERY_INSERTION_SORT_CONTEXT_VAL(state) temp, current);
            pottery_insertion_sort_lifecycle_move(POTTERY_INSERTION_SORT_CONTEXT_VAL(state) current, previous);

            size_t j = i - 1;
            for (;;) {
                current = previous;
                if (j == 0)
                    break;
                previous = pottery_insertion_sort_access(state, --j);
                if (!pottery_insertion_sort_compare_less(POTTERY_INSERTION_SORT_CONTEXT_VAL(state) temp, previous))
                    break;
                pottery_insertion_sort_lifecycle_move(POTTERY_INSERTION_SORT_CONTEXT_VAL(state) current, previous);
            }

            pottery_insertion_sort_lifecycle_move(POTTERY_INSERTION_SORT_CONTEXT_VAL(state) current, temp);
        }
    }
}
#endif

#if !POTTERY_INSERTION_SORT_USE_MOVE
static inline
void pottery_insertion_sort_impl(pottery_insertion_sort_state_t state, size_t count) {
    size_t i;
    for (i = 1; i < count; ++i) {
        pottery_insertion_sort_ref_t current = pottery_insertion_sort_access(state, i);

        size_t j;
        for (j = i; j > 0; --j) {
            pottery_insertion_sort_ref_t previous = pottery_insertion_sort_access(state, j - 1);
            if (!pottery_insertion_sort_compare_less(POTTERY_INSERTION_SORT_CONTEXT_VAL(state) current, previous))
                break;
            pottery_insertion_sort_lifecycle_swap(POTTERY_INSERTION_SORT_CONTEXT_VAL(state) current, previous);
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
        #ifdef POTTERY_INSERTION_SORT_CONTEXT_TYPE
        pottery_insertion_sort_context_t context,
        #endif
        pottery_insertion_sort_ref_t first,
        size_t count
) {
    pottery_insertion_sort_state_t state = {
        #ifdef POTTERY_INSERTION_SORT_CONTEXT_TYPE
        context,
        #endif
        first,
    };

    pottery_insertion_sort_impl(state, count);
}
