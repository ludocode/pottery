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

#ifndef POTTERY_HEAP_SORT_IMPL
#error "This is an internal header. Do not include it."
#endif

POTTERY_HEAP_SORT_EXTERN
void pottery_heap_sort(
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
        pottery_heap_sort_context_t context,
        #endif
        pottery_heap_sort_ref_t first,
        size_t count)
{
    pottery_heap_sort_heap_build(
            #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            context,
            #endif
            first,
            count);
    pottery_heap_sort_heap_contract_bulk(
            #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            context,
            #endif
            first,
            count,
            count);
}
