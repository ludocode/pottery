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

#ifndef POTTERY_ARRAY_ACCESS_INTERNAL
#error "This is an internal header. Do not include it."
#endif

#if defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE)
typedef POTTERY_ARRAY_ACCESS_VALUE_TYPE pottery_array_access_value_t;
#endif

#ifdef POTTERY_ARRAY_ACCESS_REF_TYPE
typedef POTTERY_ARRAY_ACCESS_REF_TYPE pottery_array_access_ref_t;
#else
typedef pottery_array_access_value_t* pottery_array_access_ref_t;
#endif

#ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
typedef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE pottery_array_access_context_t;
#endif



/*
 * Forward declarations
 */

#if POTTERY_FORWARD_DECLARATIONS
static inline
pottery_array_access_ref_t pottery_array_access_select(
        POTTERY_ARRAY_ACCESS_ARGS
        size_t index);

#if POTTERY_ARRAY_ACCESS_INHERENT_COUNT
static inline
pottery_array_access_ref_t pottery_array_access_end(
        POTTERY_ARRAY_ACCESS_SOLE_ARGS);

static inline
size_t pottery_array_access_count(
        POTTERY_ARRAY_ACCESS_SOLE_ARGS);
#endif
#endif



/**
 * Returns the first ref, or the end ref if empty.
 */
static inline
pottery_array_access_ref_t pottery_array_access_begin(
        POTTERY_ARRAY_ACCESS_SOLE_ARGS
) {
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_BEGIN
        #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
            return (POTTERY_ARRAY_ACCESS_BEGIN((context)));
        #else
            return (POTTERY_ARRAY_ACCESS_BEGIN());
        #endif

    #else
        return base;
    #endif
}



#if POTTERY_ARRAY_ACCESS_INHERENT_COUNT
/**
 * Returns the end ref.
 */
static inline
pottery_array_access_ref_t pottery_array_access_end(
        POTTERY_ARRAY_ACCESS_SOLE_ARGS
) {
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_END
        #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
            return (POTTERY_ARRAY_ACCESS_END((context)));
        #else
            return (POTTERY_ARRAY_ACCESS_END());
        #endif

    #else
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
        size_t total_count = pottery_array_access_count(
                POTTERY_ARRAY_ACCESS_SOLE_VALS);
        #endif
        return pottery_array_access_select(
                POTTERY_ARRAY_ACCESS_VALS
                total_count);

    #endif
}
#endif



#if POTTERY_ARRAY_ACCESS_INHERENT_COUNT
/**
 * Returns the total number of elements.
 */
static inline
size_t pottery_array_access_count(
        POTTERY_ARRAY_ACCESS_SOLE_ARGS
) {
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_COUNT
        #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
            return (POTTERY_ARRAY_ACCESS_COUNT((context)));
        #else
            return (POTTERY_ARRAY_ACCESS_COUNT());
        #endif

    #elif !POTTERY_ARRAY_ACCESS_INHERENT_BASE
        return total_count;

    #elif defined(POTTERY_ARRAY_ACCESS_END)
        return pottery_array_access_index(
                POTTERY_ARRAY_ACCESS_VALS
                pottery_array_access_end(
                    POTTERY_ARRAY_ACCESS_SOLE_VALS));

    #else
        #error "Pottery template error!"
    #endif
}
#endif



/**
 * Returns the ref with the given index, or the end ref if the given index is
 * the total number of elements.
 */
static inline
pottery_array_access_ref_t pottery_array_access_select(
        POTTERY_ARRAY_ACCESS_ARGS
        size_t index)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_SELECT
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_SELECT((context), (index)));
            #else
                return (POTTERY_ARRAY_ACCESS_SELECT((index)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_SELECT((context), (base), (index)));
            #else
                return (POTTERY_ARRAY_ACCESS_SELECT((base), (index)));
            #endif
        #endif

    #elif defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE)
        // standard C array
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
        pottery_array_access_ref_t base = pottery_array_access_begin(
                POTTERY_ARRAY_ACCESS_SOLE_VALS);
        #endif
        return base + index;

    #else
        #error "Pottery template error!"
    #endif
}



/**
 * Returns the index of the given ref.
 */
static inline
size_t pottery_array_access_index(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_ref_t ref)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_INDEX
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_INDEX((context), (ref)));
            #else
                return (POTTERY_ARRAY_ACCESS_INDEX((ref)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_INDEX((context), (base), (ref)));
            #else
                return (POTTERY_ARRAY_ACCESS_INDEX((base), (ref)));
            #endif
        #endif

    #elif defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE)
        // standard C array
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
        pottery_array_access_ref_t base = pottery_array_access_begin(
                POTTERY_ARRAY_ACCESS_SOLE_VALS);
        #endif
        return pottery_cast(size_t, ref - base);

    #else
        #error "Pottery template error!"
    #endif
}



/**
 * Returns the ref following the given ref.
 */
static inline
pottery_array_access_ref_t pottery_array_access_next(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_ref_t ref)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_NEXT
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_NEXT((context), (ref)));
            #else
                return (POTTERY_ARRAY_ACCESS_NEXT((ref)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_NEXT((context), (base), (ref)));
            #else
                return (POTTERY_ARRAY_ACCESS_NEXT((base), (ref)));
            #endif
        #endif

    #elif defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE) && \
                !defined(POTTERY_ARRAY_ACCESS_SELECT) && !defined(POTTERY_ARRAY_ACCESS_INDEX)
        // standard C array
        return ref + 1;

    #else
        // add 1 to index
        return pottery_array_access_select(
                POTTERY_ARRAY_ACCESS_VALS
                pottery_array_access_index(
                    POTTERY_ARRAY_ACCESS_VALS
                    ref) + 1);
    #endif
}



/**
 * Returns the ref preceding the given ref.
 */
static inline
pottery_array_access_ref_t pottery_array_access_previous(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_ref_t ref)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_PREVIOUS
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_PREVIOUS((context), (ref)));
            #else
                return (POTTERY_ARRAY_ACCESS_PREVIOUS((ref)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_PREVIOUS((context), (base), (ref)));
            #else
                return (POTTERY_ARRAY_ACCESS_PREVIOUS((base), (ref)));
            #endif
        #endif

    #elif defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE) && \
                !defined(POTTERY_ARRAY_ACCESS_SELECT) && !defined(POTTERY_ARRAY_ACCESS_INDEX)
        // standard C array
        return ref - 1;

    #else
        // subtract 1 from index
        return pottery_array_access_select(
                POTTERY_ARRAY_ACCESS_VALS
                pottery_array_access_index(
                    POTTERY_ARRAY_ACCESS_VALS
                    ref) - 1);
    #endif
}



/**
 * Returns true if the given refs are equal; false otherwise.
 */
static inline
bool pottery_array_access_equal(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_ref_t left,
        pottery_array_access_ref_t right)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_EQUAL
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_EQUAL((context), (left), (right)));
            #else
                return (POTTERY_ARRAY_ACCESS_EQUAL((left), (right)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_EQUAL((context), (base), (left), (right)));
            #else
                return (POTTERY_ARRAY_ACCESS_EQUAL((base), (left), (right)));
            #endif
        #endif

    #elif defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE) && \
                !defined(POTTERY_ARRAY_ACCESS_SELECT) && !defined(POTTERY_ARRAY_ACCESS_INDEX)
        // standard C array
        return left == right;

    #else
        // compare indices
        return pottery_array_access_index(POTTERY_ARRAY_ACCESS_VALS left) ==
                pottery_array_access_index(POTTERY_ARRAY_ACCESS_VALS right);
    #endif
}



#if POTTERY_ARRAY_ACCESS_INHERENT_COUNT
/**
 * Returns true if the given ref exists (i.e. is not the end ref), false if it
 * does not (i.e. is the end ref.)
 */
static inline
bool pottery_array_access_exists(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_ref_t ref)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_EXISTS
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_EXISTS((context), (ref)));
            #else
                return (POTTERY_ARRAY_ACCESS_EXISTS((ref)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_EXISTS((context), (base), (ref)));
            #else
                return (POTTERY_ARRAY_ACCESS_EXISTS((base), (ref)));
            #endif
        #endif

    #elif defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE) && \
                !defined(POTTERY_ARRAY_ACCESS_SELECT) && !defined(POTTERY_ARRAY_ACCESS_INDEX)
        // standard C array
        return ref != base + total_count;

    #else
        // compare against end ref
        return !pottery_array_access_equal(POTTERY_ARRAY_ACCESS_VALS ref,
                pottery_array_access_end(POTTERY_ARRAY_ACCESS_SOLE_VALS));
    #endif
}
#endif
