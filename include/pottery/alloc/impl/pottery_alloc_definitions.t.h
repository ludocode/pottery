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



/*
 * free()
 */

// free with extended alignment
POTTERY_ALLOC_EXTERN
void pottery_alloc_impl_free_ea(POTTERY_ALLOC_CONTEXT_ARG void* ptr) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;

    #if defined(POTTERY_ALLOC_ALIGNED_FREE)
        #ifdef POTTERY_ALLOC_CONTEXT_TYPE
            POTTERY_ALLOC_ALIGNED_FREE((context), (ptr));
        #else
            POTTERY_ALLOC_ALIGNED_FREE((ptr));
        #endif

    #else
        // we've manually aligned this so a pointer to the real allocation is
        // right before the given pointer.
        void** header = pottery_cast(void**, ptr) - 1;
        pottery_alloc_impl_free_fa(POTTERY_ALLOC_CONTEXT_VAL *header);
    #endif
}



/*
 * malloc()
 */

// malloc with extended alignment
POTTERY_ALLOC_EXTERN
void* pottery_alloc_impl_malloc_ea(POTTERY_ALLOC_CONTEXT_ARG size_t alignment, size_t size) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;

    #if defined(POTTERY_ALLOC_ALIGNED_MALLOC)
        #ifdef POTTERY_ALLOC_CONTEXT_TYPE
            return POTTERY_ALLOC_ALIGNED_MALLOC((context), (alignment), (size));
        #else
            return POTTERY_ALLOC_ALIGNED_MALLOC((alignment), (size));
        #endif

    #else
        // manually align and store a pointer to the real allocation right
        // before the aligned pointer.
        size_t offset = alignment - 1;
        char* ptr = pottery_cast(char*,
                pottery_alloc_impl_malloc_fa(POTTERY_ALLOC_CONTEXT_VAL
                    alignment, size + sizeof(void*) + offset));
        uintptr_t address = (pottery_cast(uintptr_t, ptr) + sizeof(void*) + offset) & ~offset;
        void* aligned = pottery_cast(char*, address);
        void** header = pottery_cast(void**, aligned) - 1;
        *header = pottery_cast(void*, ptr);
        return aligned;
    #endif
}



/*
 * malloc_array_at_least()
 */

POTTERY_ALLOC_EXTERN
void* pottery_alloc_impl_malloc_array_at_least_ea(POTTERY_ALLOC_CONTEXT_ARG
        size_t alignment, size_t* count, size_t element_size)
{
    size_t size;
    if (pottery_unlikely(pottery_mul_overflow_s(*count, element_size, &size)))
        return pottery_null;

    #if (defined(POTTERY_ALLOC_ALIGNED_MALLOC) || defined(POTTERY_ALLOC_ALIGNED_ZALLOC) || \
            defined(POTTERY_ALLOC_ALIGNED_REALLOC))

        #if defined(POTTERY_ALLOC_ALIGNED_MALLOC_GOOD_SIZE)
            // Calculate the right amount of space to use first.
            #ifdef POTTERY_ALLOC_CONTEXT_TYPE
                size_t new_size = POTTERY_ALLOC_ALIGNED_MALLOC_GOOD_SIZE((context), (alignment), (size));
            #else
                size_t new_size = POTTERY_ALLOC_ALIGNED_MALLOC_GOOD_SIZE((alignment), (size));
            #endif
            pottery_assert(new_size >= size);
            *count = new_size / element_size;
            return pottery_alloc_impl_malloc_ea(POTTERY_ALLOC_CONTEXT_VAL alignment, new_size);

        #elif defined(POTTERY_ALLOC_ALIGNED_MALLOC_USABLE_SIZE) && defined(POTTERY_ALLOC_ALIGNED_MALLOC_EXPAND)
            // Allocate, then expand to fill the space.
            void* ptr = pottery_alloc_impl_malloc_ea(POTTERY_ALLOC_CONTEXT_VAL alignment, *size);

            #ifdef POTTERY_ALLOC_CONTEXT_TYPE
                size_t new_size = POTTERY_ALLOC_ALIGNED_MALLOC_USABLE_SIZE((context), (alignment), (ptr));
            #else
                size_t new_size = POTTERY_ALLOC_ALIGNED_MALLOC_USABLE_SIZE((alignment), (ptr));
            #endif

            // It's common that the usable size is larger but not enough for
            // more elements. We only bother to expand if we'll actually get
            // more elements out of it.
            size_t new_count = new_size / element_size;
            if (new_count > *count) {
                void* new_ptr = ptr;
                #ifdef POTTERY_ALLOC_CONTEXT_TYPE
                    POTTERY_ALLOC_ALIGNED_MALLOC_EXPAND((context), (&ptr), (size));
                #else
                    POTTERY_ALLOC_ALIGNED_MALLOC_EXPAND((&ptr), (size));
                #endif

                // Make sure the pointer didn't change and the size didn't shrink.
                pottery_assert(*size >= old_size);
                pottery_assert(old_ptr == ptr);
                *count = new_count;
                ptr = new_ptr;
            }
            return ptr;

        #else
            // No support for at_least.
            return pottery_alloc_impl_malloc_ea(POTTERY_ALLOC_CONTEXT_VAL alignment, size);
        #endif

    #elif (defined(POTTERY_ALLOC_MALLOC) || defined(POTTERY_ALLOC_ZALLOC) || \
            defined(POTTERY_ALLOC_REALLOC))

        // We need to manually align, but we may also be able to expand the
        // allocation.
        size_t offset = alignment - 1;
        size_t alloc_size = size + sizeof(void*) + offset;

        #if defined(POTTERY_ALLOC_ALIGNED_MALLOC_GOOD_SIZE)
            // Calculate the right amount of space to use first.
            #ifdef POTTERY_ALLOC_CONTEXT_TYPE
                alloc_size = POTTERY_ALLOC_ALIGNED_MALLOC_GOOD_SIZE((context), (alignment), (size));
            #else
                alloc_size = POTTERY_ALLOC_ALIGNED_MALLOC_GOOD_SIZE((alignment), (size));
            #endif
        #endif

        char* ptr = pottery_cast(char*,
                pottery_alloc_impl_malloc_fa(POTTERY_ALLOC_CONTEXT_VAL alignment, alloc_size));

        #if !defined(POTTERY_ALLOC_ALIGNED_MALLOC_GOOD_SIZE) && \
                defined(POTTERY_ALLOC_ALIGNED_MALLOC_USABLE_SIZE) && \
                defined(POTTERY_ALLOC_ALIGNED_MALLOC_EXPAND)
            // Expand ptr to fill the usable space

            #ifdef POTTERY_ALLOC_CONTEXT_TYPE
                size_t new_size = POTTERY_ALLOC_ALIGNED_MALLOC_USABLE_SIZE((context), (alignment), (ptr));
            #else
                size_t new_size = POTTERY_ALLOC_ALIGNED_MALLOC_USABLE_SIZE((alignment), (ptr));
            #endif

            if (new_size > alloc_size) {
                void* new_ptr = ptr;
                #ifdef POTTERY_ALLOC_CONTEXT_TYPE
                    POTTERY_ALLOC_ALIGNED_MALLOC_EXPAND((context), (&new_ptr), (&new_size));
                #else
                    POTTERY_ALLOC_ALIGNED_MALLOC_EXPAND((&new_ptr), (&new_size));
                #endif

                // Make sure the pointer didn't change and the size didn't shrink.
                pottery_assert(new_size >= alloc_size);
                pottery_assert(ptr == new_ptrptr);
                alloc_size = new_size;
            }
        #endif

        uintptr_t alloc_address = pottery_cast(uintptr_t, ptr);
        uintptr_t aligned_address = (alloc_address + sizeof(void*) + offset) & ~offset;
        void* aligned = pottery_cast(char*, aligned_address);
        void** header = pottery_cast(void**, aligned) - 1;
        *header = pottery_cast(void*, ptr);

        // Even if we did not support either of good_size or expanding, this
        // might still be more elements than requested because we may not have
        // needed all the padding we reserved for manual alignment.
        size_t remaining_size = alloc_size - pottery_cast(size_t, aligned_address - alloc_address);
        *count = remaining_size / element_size;

        return aligned;

    #else
        #error "A memory allocation expression is required."
    #endif
}

POTTERY_ALLOC_EXTERN
void* pottery_alloc_impl_malloc_array_at_least_fa(POTTERY_ALLOC_CONTEXT_ARG
        size_t alignment, size_t* count, size_t element_size)
{
    size_t size;
    if (pottery_unlikely(pottery_mul_overflow_s(*count, element_size, &size)))
        return pottery_null;

    #if (defined(POTTERY_ALLOC_MALLOC) || defined(POTTERY_ALLOC_ZALLOC) || \
            defined(POTTERY_ALLOC_REALLOC))

        #if defined(POTTERY_ALLOC_MALLOC_GOOD_SIZE)
            // Calculate the right amount of space to use first.
            #ifdef POTTERY_ALLOC_CONTEXT_TYPE
                size_t new_size = POTTERY_ALLOC_MALLOC_GOOD_SIZE((context), (size));
            #else
                size_t new_size = POTTERY_ALLOC_MALLOC_GOOD_SIZE((size));
            #endif
            pottery_assert(new_size >= size);
            *count = new_size / element_size;
            return pottery_alloc_impl_malloc_fa(POTTERY_ALLOC_CONTEXT_VAL alignment, new_size);

        #elif defined(POTTERY_ALLOC_MALLOC_USABLE_SIZE) && defined(POTTERY_ALLOC_MALLOC_EXPAND)
            // Allocate, then expand to fill the space.
            void* ptr = pottery_alloc_impl_malloc_fa(POTTERY_ALLOC_CONTEXT_VAL alignment, size);

            #ifdef POTTERY_ALLOC_CONTEXT_TYPE
                size_t new_size = POTTERY_ALLOC_MALLOC_USABLE_SIZE((context), (ptr));
            #else
                size_t new_size = POTTERY_ALLOC_MALLOC_USABLE_SIZE((ptr));
            #endif

            // It's common that the usable size is larger but not enough for
            // more elements. We only bother to expand if we'll actually get
            // more elements out of it.
            size_t new_count = new_size / element_size;
            if (new_count > *count) {
                void* new_ptr = ptr;
                #ifdef POTTERY_ALLOC_CONTEXT_TYPE
                    POTTERY_ALLOC_MALLOC_EXPAND((context), (&new_ptr), (&new_size));
                #else
                    POTTERY_ALLOC_MALLOC_EXPAND((&new_ptr), (&new_size));
                #endif

                // Make sure the pointer didn't change and the size didn't shrink.
                pottery_assert(new_size >= size);
                pottery_assert(new_ptr == ptr);
                *count = new_count;
                ptr = new_ptr;
            }
            return ptr;

        #else
            // No support for at_least.
            return pottery_alloc_impl_malloc_fa(POTTERY_ALLOC_CONTEXT_VAL alignment, size);
        #endif

    #elif (defined(POTTERY_ALLOC_ALIGNED_MALLOC) || defined(POTTERY_ALLOC_ALIGNED_ZALLOC) || \
            defined(POTTERY_ALLOC_ALIGNED_REALLOC))
        return pottery_alloc_impl_malloc_array_at_least_ea(POTTERY_ALLOC_CONTEXT_VAL alignment, size);

    #else
        #error "A memory allocation expression is required."
    #endif
}



// TODO realloc not done yet
#if 0

void* pottery_alloc_realloc_at_least(POTTERY_ALLOC_CONTEXT_ARG
        void* ptr, size_t old_size, size_t* new_size
) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;
    void* ptr = pottery_alloc_realloc(POTTERY_ALLOC_CONTEXT_VAL ptr, old_size, *new_size);
    return pottery_alloc_impl_expand_fa(POTTERY_ALLOC_CONTEXT_VAL ptr, new_size);
}

void* pottery_alloc_realloc_array_at_least(POTTERY_ALLOC_CONTEXT_ARG
        void* old_ptr, size_t old_count, size_t* new_count, size_t element_size
) {
    POTTERY_ALLOC_CONTEXT_MAYBE_UNUSED;

    size_t new_size;
    if (pottery_unlikely(pottery_mul_overflow_s(*new_count, element_size, &new_size)))
        return pottery_null;
    size_t old_size = old_count * element_size;

    void* ptr = pottery_alloc_realloc_at_least(POTTERY_ALLOC_CONTEXT_VAL old_ptr, old_size, &new_size);
    *new_count = new_size / element_size;
    reeturn ptr;
}
#endif

