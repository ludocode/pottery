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

#ifndef POTTERY_INTRO_SORT_IMPL
#error "This is an internal header. Do not include it."
#endif

/**
 * Sorts a sub-range of elements within an array.
 */
static inline
void pottery_intro_sort_range(
        POTTERY_INTRO_SORT_ARGS
        size_t offset,
        size_t range_count)
{
    // Just call into quick_sort. We've configured it to fallback to
    // insertion_sort for small lists and heap_sort when too deep.
    pottery_intro_sort_quick_sort_range(
            POTTERY_INTRO_SORT_VALS
            offset, range_count);
}

/**
 * Sorts an array.
 */
static inline
void pottery_intro_sort(
        #if POTTERY_ARRAY_ACCESS_INHERENT_COUNT
        POTTERY_INTRO_SORT_SOLE_ARGS
        #else
        POTTERY_INTRO_SORT_ARGS
        size_t total_count
        #endif
) {
    #if POTTERY_ARRAY_ACCESS_INHERENT_COUNT
    size_t total_count = pottery_intro_sort_array_access_count(
            POTTERY_INTRO_SORT_SOLE_VALS);
    #endif

    pottery_intro_sort_range(POTTERY_INTRO_SORT_VALS
            0, total_count);
}
