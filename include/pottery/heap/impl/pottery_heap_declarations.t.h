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

#if POTTERY_FORWARD_DECLARATIONS
/**
 * Builds a heap in a sub-range of the given array.
 */
POTTERY_HEAP_EXTERN
void pottery_heap_build_range(POTTERY_HEAP_ARGS
        size_t offset, size_t count);

/**
 * Expands a heap in a sub-range of the given array.
 */
POTTERY_HEAP_EXTERN
void pottery_heap_expand_bulk_range(POTTERY_HEAP_ARGS
        size_t offset, size_t current_count, size_t expand_count);

/**
 * Contracts a heap in a sub-range of the given array.
 */
POTTERY_HEAP_EXTERN
void pottery_heap_contract_bulk_range(POTTERY_HEAP_ARGS
        size_t offset, size_t current_count, size_t contract_count);

/**
 * Contracts one element out of a heap in a sub-range of the given array.
 */
POTTERY_HEAP_EXTERN
void pottery_heap_contract_at_range(POTTERY_HEAP_ARGS
        size_t offset, size_t current_count, size_t index_to_contract);

/**
 * Returns the number of elements in the given sub-range that form a valid heap.
 */
POTTERY_HEAP_EXTERN
size_t pottery_heap_valid_count_range(POTTERY_HEAP_ARGS
        size_t offset, size_t count);
#endif

/**
 * Returns true if the given sub-range of elements for a valid heap (i.e. every
 * parent node is not less than its children.)
 */
static inline
bool pottery_heap_valid_range(
        POTTERY_HEAP_ARGS
        size_t offset,
        size_t count
) {
    return count == pottery_heap_valid_count_range(POTTERY_HEAP_VALS offset, count);
}

/**
 * Builds a heap in the given array out of the given number of elements.
 */
static inline
void pottery_heap_build(
        POTTERY_HEAP_ARGS
        size_t count)
{
    pottery_heap_build_range(POTTERY_HEAP_VALS 0, count);
}

/**
 * Expands a heap of size `current_count` to encompass an additional
 * `expand_count` elements at the end of the range.
 *
 * The resulting size of the heap will be (current_count + expand_count).
 *
 * Note that this API is different from C++ std::push_heap. std::push_heap
 * takes as arguments the bounds you want the heap to be after pushing the
 * last element. This is somewhat inconsistent from std::pop_heap which takes
 * the current bounds of the heap before popping the largest element. Pottery
 * avoids this ambiguity by always taking the current count as the argument.
 */
static inline
void pottery_heap_expand_bulk(
        POTTERY_HEAP_ARGS
        size_t current_count, size_t expand_count)
{
    pottery_heap_expand_bulk_range(POTTERY_HEAP_VALS 0, current_count, expand_count);
}

/**
 * Contracts a heap of size `current_count` by the `contract_count` largest
 * elements, moving them to the end of the range.
 *
 * The resulting size of the heap will be (current_count - contract_count).
 */
static inline
void pottery_heap_contract_bulk(
        POTTERY_HEAP_ARGS
        size_t current_count, size_t contract_count)
{
    pottery_heap_contract_bulk_range(POTTERY_HEAP_VALS 0, current_count, contract_count);
}

/**
 * Contracts a heap of size `current_count` by moving a single element at the
 * given index to the end of the range.
 *
 * The resulting size of the heap will be (current_count - 1).
 */
static inline
void pottery_heap_contract_at(
        POTTERY_HEAP_ARGS
        size_t current_count, size_t index_to_contract)
{
    pottery_heap_contract_at_range(POTTERY_HEAP_VALS 0, current_count, index_to_contract);
}

/**
 * Returns the number of elements in the given range that form a valid heap.
 */
static inline
size_t pottery_heap_valid_count(
        POTTERY_HEAP_ARGS
        size_t count)
{
    return pottery_heap_valid_count_range(POTTERY_HEAP_VALS 0, count);
}

/**
 * Returns true if the heap is valid (i.e. every parent node is not less than
 * its children.)
 */
static inline
bool pottery_heap_valid(
        POTTERY_HEAP_ARGS
        size_t count)
{
    return count == pottery_heap_valid_count(POTTERY_HEAP_VALS count);
}
