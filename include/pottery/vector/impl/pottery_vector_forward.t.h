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

#ifndef POTTERY_VECTOR_IMPL
#error "This is an internal header. Do not include it."
#endif

#ifndef POTTERY_VECTOR_INTERNAL_CAPACITY
    #define POTTERY_VECTOR_INTERNAL_CAPACITY 0
#endif

typedef struct pottery_vector_t {
    pottery_vector_value_t* storage;
    #if POTTERY_VECTOR_DOUBLE_ENDED
        pottery_vector_value_t* begin;
    #endif
    size_t count;

    union {
        size_t capacity;
        #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
            pottery_vector_value_t internal[POTTERY_VECTOR_INTERNAL_CAPACITY];
        #endif
    } u;

    #if POTTERY_DEBUG
        #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
            struct pottery_vector_t* self_check; // internal capacity makes this not bitwise movable
        #endif
        #if POTTERY_LEAK_CHECK
            void* leak_check;
        #endif
    #endif

    #ifdef POTTERY_VECTOR_CONTEXT_TYPE
        pottery_vector_context_t context;
    #endif
} pottery_vector_t;
