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

#ifndef POTTERY_VECTOR_IMPL
#error "This is an internal header. Do not include it."
#endif

#ifndef POTTERY_VECTOR_INTERNAL_CAPACITY
    #define POTTERY_VECTOR_INTERNAL_CAPACITY 0
#endif

typedef POTTERY_VECTOR_ELEMENT_TYPE pottery_vector_element_t;
typedef pottery_vector_element_t* pottery_vector_entry_t;

#ifdef POTTERY_VECTOR_CONTEXT_TYPE
typedef POTTERY_VECTOR_CONTEXT_TYPE pottery_vector_context_t;
#endif

typedef struct pottery_vector_t {
    pottery_vector_element_t* values;
    size_t count;

    union {
        size_t capacity;
        #if POTTERY_VECTOR_INTERNAL_CAPACITY > 0
        pottery_vector_element_t internal[POTTERY_VECTOR_INTERNAL_CAPACITY];
        #endif
    } u;

    #ifdef POTTERY_DEBUG
    struct pottery_vector_t* self_check;
    void* leak_check;
    #endif

    #ifdef POTTERY_VECTOR_CONTEXT_TYPE
    pottery_vector_context_t context;
    #endif
} pottery_vector_t;
