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
POTTERY_INSERTION_SORT_EXTERN
void pottery_insertion_sort_range(
        POTTERY_INSERTION_SORT_ARGS
        size_t offset,
        size_t range_count)
{
    POTTERY_DECLARE_UNCONSTRUCTED(pottery_insertion_sort_value_t, temp);

    size_t i;
    for (i = offset + 1; i < offset + range_count; ++i) {
        pottery_insertion_sort_ref_t current = pottery_insertion_sort_array_access_select(POTTERY_INSERTION_SORT_VALS i);
        pottery_insertion_sort_ref_t previous = pottery_insertion_sort_array_access_select(POTTERY_INSERTION_SORT_VALS i - 1);

        if (pottery_insertion_sort_compare_less(POTTERY_INSERTION_SORT_CONTEXT_VAL current, previous)) {
            pottery_insertion_sort_lifecycle_move(POTTERY_INSERTION_SORT_CONTEXT_VAL temp, current);
            pottery_insertion_sort_lifecycle_move(POTTERY_INSERTION_SORT_CONTEXT_VAL current, previous);

            size_t j = i - 1;
            for (;;) {
                current = previous;
                if (j == 0)
                    break;
                previous = pottery_insertion_sort_array_access_select(POTTERY_INSERTION_SORT_VALS --j);
                if (!pottery_insertion_sort_compare_less(POTTERY_INSERTION_SORT_CONTEXT_VAL temp, previous))
                    break;
                pottery_insertion_sort_lifecycle_move(POTTERY_INSERTION_SORT_CONTEXT_VAL current, previous);
            }

            pottery_insertion_sort_lifecycle_move(POTTERY_INSERTION_SORT_CONTEXT_VAL current, temp);
        }
    }
}
#endif

#if !POTTERY_INSERTION_SORT_USE_MOVE
POTTERY_INSERTION_SORT_EXTERN
void pottery_insertion_sort_range(
        POTTERY_INSERTION_SORT_ARGS
        size_t offset,
        size_t range_count)
{
    size_t i;
    for (i = offset + 1; i < offset + range_count; ++i) {
        pottery_insertion_sort_ref_t current = pottery_insertion_sort_array_access_select(POTTERY_INSERTION_SORT_VALS i);

        size_t j;
        for (j = i; j > 0; --j) {
            pottery_insertion_sort_ref_t previous = pottery_insertion_sort_array_access_select(POTTERY_INSERTION_SORT_VALS j - 1);
            if (!pottery_insertion_sort_compare_less(POTTERY_INSERTION_SORT_CONTEXT_VAL current, previous))
                break;
            pottery_insertion_sort_lifecycle_swap(POTTERY_INSERTION_SORT_CONTEXT_VAL current, previous);
            current = previous;
        }
    }
}
#endif
