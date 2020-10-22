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

#if !POTTERY_LIFECYCLE_CAN_MOVE && !POTTERY_LIFECYCLE_CAN_SWAP
    #error "A move or swap expression is required."
#endif
#if !POTTERY_COMPARE_CAN_ORDER
    #error "An ordering comparison expression is required."
#endif

// TODO this can't go here, needs to be available in decls
#ifndef POTTERY_INSERTION_SORT_USE_MOVE
    // Decide whether to use move or swap. We need a move expression to move
    // and a value type to define a temporary to move to.
    //
    // We prefer to move rather than swap if possible since this should result
    // in fewer moves. This could in theory be slower than swap if you have
    // some major optimization in swap, like if most of the contents of your
    // objects are identical and don't need to be swapped. If you don't want
    // insertion_sort to use move, define POTTERY_INSERTION_SORT_USE_MOVE to 0,
    // or just don't give it a move expression.
    #if POTTERY_LIFECYCLE_CAN_MOVE && defined(POTTERY_INSERTION_SORT_VALUE_TYPE)
        #define POTTERY_INSERTION_SORT_USE_MOVE 1
    #else
        #define POTTERY_INSERTION_SORT_USE_MOVE 0
    #endif
#endif

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_INSERTION_SORT_EXTERN
void pottery_insertion_sort(
        #ifdef POTTERY_INSERTION_SORT_CONTEXT_TYPE
        pottery_insertion_sort_context_t context,
        #endif
        pottery_insertion_sort_ref_t first,
        size_t count);
#endif
