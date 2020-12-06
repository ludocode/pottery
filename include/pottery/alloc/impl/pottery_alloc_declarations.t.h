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

#ifndef POTTERY_ALLOC_INTERNAL
#error "This is an internal header. Do not include it."
#endif

#ifdef POTTERY_ALLOC_CONTEXT_TYPE
typedef POTTERY_ALLOC_CONTEXT_TYPE pottery_alloc_context_t;
#endif

#ifndef pottery_alignmax
#error "Maximum fundamental alignment is not known! Define pottery_alignmax."
#endif




/*
 * free()
 */

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_ALLOC_EXTERN
void pottery_alloc_impl_free_ea(POTTERY_ALLOC_CONTEXT_ARG void* ptr);
#endif

static pottery_always_inline
void pottery_alloc_impl_free_fa(POTTERY_ALLOC_CONTEXT_ARG void* ptr) {

    #if defined(POTTERY_ALLOC_FREE)
        #ifdef POTTERY_ALLOC_CONTEXT_TYPE
            POTTERY_ALLOC_FREE((context), (ptr));
        #else
            POTTERY_ALLOC_FREE((ptr));
        #endif

    #elif defined(POTTERY_ALLOC_ALIGNED_FREE)
        pottery_alloc_impl_free_ea(POTTERY_ALLOC_CONTEXT_VAL ptr);

    #else
        #error "A FREE() or ALIGNED_FREE() expression is required."
    #endif

}

static pottery_always_inline
void pottery_alloc_free(POTTERY_ALLOC_CONTEXT_ARG size_t alignment, void* ptr) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;

    // Pottery never frees NULL.
    pottery_assert(ptr != pottery_null);
    pottery_assert(pottery_ispow2_s(alignment)); // Alignment must be power-of-two.

    if (alignment > pottery_alignmax) {
        pottery_alloc_impl_free_ea(POTTERY_ALLOC_CONTEXT_VAL ptr);
        return;
    }

    pottery_alloc_impl_free_fa(POTTERY_ALLOC_CONTEXT_VAL ptr);
}



/*
 * malloc()
 */

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_ALLOC_EXTERN
void* pottery_alloc_impl_malloc_ea(POTTERY_ALLOC_CONTEXT_ARG size_t alignment, size_t size);
#endif

static pottery_always_inline
void* pottery_alloc_impl_malloc_fa(POTTERY_ALLOC_CONTEXT_ARG size_t alignment, size_t size) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;
    (void)alignment;

    #if defined(POTTERY_ALLOC_MALLOC)
        #ifdef POTTERY_ALLOC_CONTEXT_TYPE
            return POTTERY_ALLOC_MALLOC((context), (size));
        #else
            return POTTERY_ALLOC_MALLOC((size));
        #endif

    #elif defined(POTTERY_ALLOC_REALLOC)
        #ifdef POTTERY_ALLOC_CONTEXT_TYPE
            return POTTERY_ALLOC_REALLOC((context), (pottery_null), (size));
        #else
            return POTTERY_ALLOC_REALLOC((pottery_null), (size));
        #endif

    #elif defined(POTTERY_ALLOC_ZALLOC)
        #ifdef POTTERY_ALLOC_CONTEXT_TYPE
            return POTTERY_ALLOC_ZALLOC((context), (size));
        #else
            return POTTERY_ALLOC_ZALLOC((size));
        #endif

    #elif defined(POTTERY_ALLOC_ALIGNED_MALLOC) || \
            defined(POTTERY_ALLOC_ALIGNED_ZALLOC) || \
            defined(POTTERY_ALLOC_ALIGNED_REALLOC)
        return pottery_alloc_impl_malloc_ea(POTTERY_ALLOC_CONTEXT_VAL alignment, size);

    #else
        #error "A memory allocation expression is required."
    #endif
}

static pottery_always_inline
void* pottery_alloc_malloc(POTTERY_ALLOC_CONTEXT_ARG size_t alignment, size_t size) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;

    pottery_assert(size != 0); // Zero-size allocation is not allowed.
    pottery_assert(pottery_ispow2_s(alignment)); // Alignment must be power-of-two.

    if (alignment > pottery_alignmax) {
        return pottery_alloc_impl_malloc_ea(POTTERY_ALLOC_CONTEXT_VAL alignment, size);
    }

    return pottery_alloc_impl_malloc_fa(POTTERY_ALLOC_CONTEXT_VAL alignment, size);
}



/*
 * malloc_zero()
 */

// extended alignment allocation with zeroing
static inline
void* pottery_alloc_impl_malloc_zero_ea(POTTERY_ALLOC_CONTEXT_ARG size_t alignment, size_t size) {
    void* ptr = pottery_alloc_impl_malloc_ea(POTTERY_ALLOC_CONTEXT_VAL alignment, size);
    pottery_memset(ptr, 0, size);
    return ptr;
}

#if !defined(POTTERY_ALLOC_ZALLOC) && ( \
            (defined(POTTERY_ALLOC_MALLOC) || defined(POTTERY_ALLOC_REALLOC)) || \
    (!defined(POTTERY_ALLOC_ALIGNED_ZALLOC) && \
            (defined(POTTERY_ALLOC_ALIGNED_MALLOC) || defined(POTTERY_ALLOC_ALIGNED_REALLOC))))
// fundamental alignment allocation with manual zeroing (i.e. we don't have calloc())
static inline
void* pottery_alloc_impl_malloc_zero_fa_wrap(POTTERY_ALLOC_CONTEXT_ARG size_t alignment, size_t size) {
    void* ptr = pottery_alloc_impl_malloc_fa(POTTERY_ALLOC_CONTEXT_VAL alignment, size);
    pottery_memset(ptr, 0, size);
    return ptr;
}
#endif

// fundamental alignment allocation with zeroing
static pottery_always_inline
void* pottery_alloc_impl_malloc_zero_fa(POTTERY_ALLOC_CONTEXT_ARG size_t alignment, size_t size) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;
    (void)alignment;

    #if defined(POTTERY_ALLOC_ZALLOC)
        #ifdef POTTERY_ALLOC_CONTEXT_TYPE
            return POTTERY_ALLOC_ZALLOC((context), (size));
        #else
            return POTTERY_ALLOC_ZALLOC((size));
        #endif

    #elif defined(POTTERY_ALLOC_MALLOC) || defined(POTTERY_ALLOC_REALLOC)
        return pottery_alloc_impl_malloc_zero_fa_wrap(POTTERY_ALLOC_CONTEXT_VAL alignment, size);

    #elif defined(POTTERY_ALLOC_ALIGNED_ZALLOC)
        #ifdef POTTERY_ALLOC_CONTEXT_TYPE
            return POTTERY_ALLOC_ALIGNED_ZALLOC((context), (alignment), (size));
        #else
            return POTTERY_ALLOC_ALIGNED_ZALLOC((alignment), (size));
        #endif

    #elif defined(POTTERY_ALLOC_ALIGNED_MALLOC) || defined(POTTERY_ALLOC_ALIGNED_REALLOC)
        return pottery_alloc_impl_malloc_zero_fa_wrap(POTTERY_ALLOC_CONTEXT_VAL alignment, size);

    #else
        #error "A memory allocation expression is required."
    #endif
}

static pottery_always_inline
void* pottery_alloc_malloc_zero(POTTERY_ALLOC_CONTEXT_ARG size_t alignment, size_t size) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;

    pottery_assert(size != 0); // Zero-size allocation is not allowed.
    pottery_assert(pottery_ispow2_s(alignment)); // Alignment must be power-of-two.

    if (alignment > pottery_alignmax) {
        return pottery_alloc_impl_malloc_zero_ea(POTTERY_ALLOC_CONTEXT_VAL alignment, size);
    }

    return pottery_alloc_impl_malloc_zero_fa(POTTERY_ALLOC_CONTEXT_VAL alignment, size);
}



/*
 * malloc_array_at_least()
 */

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_ALLOC_EXTERN
void* pottery_alloc_impl_malloc_array_at_least_ea(POTTERY_ALLOC_CONTEXT_ARG
        size_t alignment, size_t* count, size_t element_size);

POTTERY_ALLOC_EXTERN
void* pottery_alloc_impl_malloc_array_at_least_fa(POTTERY_ALLOC_CONTEXT_ARG
        size_t alignment, size_t* count, size_t element_size);
#endif

static pottery_always_inline
void* pottery_alloc_malloc_array_at_least(POTTERY_ALLOC_CONTEXT_ARG
        size_t alignment, size_t* count, size_t element_size)
{
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;

    // Zero-size allocation is not allowed.
    pottery_assert(*count != 0);
    pottery_assert(element_size != 0);

    // Alignment must be power-of-two.
    pottery_assert(pottery_ispow2_s(alignment));

    if (alignment > pottery_alignmax) {
        return pottery_alloc_impl_malloc_array_at_least_ea(POTTERY_ALLOC_CONTEXT_VAL
                alignment, count, element_size);
    }

    return pottery_alloc_impl_malloc_array_at_least_fa(POTTERY_ALLOC_CONTEXT_VAL
                alignment, count, element_size);
}



/*
 * malloc_array()
 */

static inline
void* pottery_alloc_malloc_array(POTTERY_ALLOC_CONTEXT_ARG size_t alignment, size_t count, size_t element_size) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;

    size_t size;
    if (pottery_unlikely(pottery_mul_overflow_s(count, element_size, &size)))
        return pottery_null;
    return pottery_alloc_malloc(POTTERY_ALLOC_CONTEXT_VAL alignment, size);
}



/*
 * malloc_array_zero()
 */

static inline
void* pottery_alloc_malloc_array_zero(POTTERY_ALLOC_CONTEXT_ARG size_t alignment, size_t count, size_t element_size) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;

    size_t size;
    if (pottery_unlikely(pottery_mul_overflow_s(count, element_size, &size)))
        return pottery_null;
    return pottery_alloc_malloc_zero(POTTERY_ALLOC_CONTEXT_VAL alignment, size);
}






// TODO realloc not done yet
#if 0


static inline
void* pottery_alloc_realloc(POTTERY_ALLOC_CONTEXT_ARG
        void* old_ptr, size_t old_size, size_t new_size
) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;

    #if defined(POTTERY_ALLOC_REALLOC)
        #ifdef POTTERY_ALLOC_CONTEXT_TYPE
            return POTTERY_ALLOC_REALLOC((context), (old_ptr), (old_size), (new_size));
        #else
            return POTTERY_ALLOC_REALLOC((old_ptr), (old_size), (new_size));
        #endif

    #elif defined(POTTERY_REALLOC)
        return POTTERY_REALLOC((old_ptr), (old_size), (new_size));

    #else
        void* new_ptr = pottery_alloc_malloc(POTTERY_ALLOC_CONTEXT_VAL new_size);
        if (new_ptr == pottery_null)
            return pottery_null;
        pottery_memcpy(new_ptr, old_ptr, old_size);
        pottery_free(POTTERY_ALLOC_CONTEXT_VAL old_ptr);
        return new_ptr;
    #endif
}

static inline
void* pottery_alloc_realloc_array(POTTERY_ALLOC_CONTEXT_ARG
        void* old_ptr, size_t old_count, size_t new_count, size_t element_size
) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;

    size_t new_size;
    if (pottery_unlikely(pottery_mul_overflow_s(new_count, element_size, &new_size)))
        return pottery_null;
    size_t old_size = old_count * element_size;

    return pottery_alloc_realloc(POTTERY_ALLOC_CONTEXT_VAL old_ptr, old_size, new_size);
}

#if POTTERY_ALLOC_CAN_ALLOC_AT_LEAST
void* pottery_alloc_malloc_at_least(POTTERY_ALLOC_CONTEXT_ARG size_t* size);

void* pottery_alloc_malloc_at_least(POTTERY_ALLOC_CONTEXT_ARG
        size_t* count, size_t element_size);

void* pottery_alloc_realloc_at_least(POTTERY_ALLOC_CONTEXT_ARG
        void* ptr, size_t old_size, size_t* new_size);

void* pottery_alloc_realloc_at_least(POTTERY_ALLOC_CONTEXT_ARG
        void* old_ptr, size_t old_count, size_t* new_count, size_t element_size);
#else
static inline
void* pottery_alloc_malloc_at_least(POTTERY_ALLOC_CONTEXT_ARG size_t* size) {
    return pottery_alloc_malloc(POTTERY_ALLOC_CONTEXT_VAL *size);
}

static inline
void* pottery_alloc_malloc_at_least(POTTERY_ALLOC_CONTEXT_ARG
        size_t* count, size_t element_size)
{
    return pottery_alloc_malloc_array(POTTERY_ALLOC_CONTEXT_VAL *count, element_size);
}

static inline
void* pottery_alloc_realloc_at_least(POTTERY_ALLOC_CONTEXT_ARG
        void* ptr, size_t old_size, size_t* new_size)
{
    return pottery_alloc_realloc(POTTERY_ALLOC_CONTEXT_VAL old_ptr, old_size, *new_size);
}

static inline
void* pottery_alloc_realloc_at_least(POTTERY_ALLOC_CONTEXT_ARG
        void* old_ptr, size_t old_count, size_t* new_count, size_t element_size)
{
    return pottery_alloc_realloc_array(POTTERY_ALLOC_CONTEXT_VAL old_ptr, old_count, *new_count, element_size);
}
#endif

#endif
