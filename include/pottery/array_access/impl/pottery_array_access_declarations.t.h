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

#ifndef POTTERY_ARRAY_ACCESS_INTERNAL
#error "This is an internal header. Do not include it."
#endif


/*
 * Types
 */

#if defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE)
typedef POTTERY_ARRAY_ACCESS_VALUE_TYPE pottery_array_access_value_t;
#endif

#ifdef POTTERY_ARRAY_ACCESS_REF_TYPE
typedef POTTERY_ARRAY_ACCESS_REF_TYPE pottery_array_access_ref_t;
#else
typedef pottery_array_access_value_t* pottery_array_access_ref_t;
#endif

#ifdef POTTERY_ARRAY_ACCESS_ENTRY_TYPE
typedef POTTERY_ARRAY_ACCESS_ENTRY_TYPE pottery_array_access_entry_t;
#else
typedef pottery_array_access_ref_t pottery_array_access_entry_t;
#endif

#ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
typedef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE pottery_array_access_context_t;
#endif



/*
 * Forward declarations
 */

#if POTTERY_FORWARD_DECLARATIONS
static inline
pottery_array_access_entry_t pottery_array_access_select(
        POTTERY_ARRAY_ACCESS_ARGS
        size_t index);

#if POTTERY_ARRAY_ACCESS_INHERENT_COUNT
static inline
pottery_array_access_entry_t pottery_array_access_end(
        POTTERY_ARRAY_ACCESS_SOLE_ARGS);

static inline
size_t pottery_array_access_count(
        POTTERY_ARRAY_ACCESS_SOLE_ARGS);
#endif
#endif



// TODO this has been moved to container_types. This and the above typedefs
// need to be removed since array_access should use container_types.
#if 0
/**
 * Returns a ref for an entry.
 *
 * The entry must exist.
 */
static inline
pottery_array_access_ref_t pottery_array_access_ref(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_entry_t entry)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_ENTRY_REF
        #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
            return (POTTERY_ARRAY_ACCESS_ENTRY_REF((context), (entry)));
        #else
            return (POTTERY_ARRAY_ACCESS_ENTRY_REF((entry)));
        #endif
    #else
        // Without an ENTRY_REF expression, the entry type must implicitly
        // convertible to the ref type (usually it's the same type.)
        return entry;
    #endif
}
#endif



/**
 * Returns the first entry, or the end entry if empty.
 */
static inline
pottery_array_access_entry_t pottery_array_access_begin(
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
 * Returns the end entry.
 */
static inline
pottery_array_access_entry_t pottery_array_access_end(
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
        size_t total_count = pottery_array_access_count(POTTERY_ARRAY_ACCESS_SOLE_VALS);
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
 * Returns the entry with the given index, or the end entry if the given index is
 * the total number of elements.
 */
static inline
pottery_array_access_entry_t pottery_array_access_select(
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
        pottery_array_access_entry_t base = pottery_array_access_begin(
                POTTERY_ARRAY_ACCESS_SOLE_VALS);
        #endif
        return base + index;

    #else
        #error "Pottery template error!"
    #endif
}



/**
 * Returns the index of the given entry.
 */
static inline
size_t pottery_array_access_index(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_entry_t entry)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_INDEX
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_INDEX((context), (entry)));
            #else
                return (POTTERY_ARRAY_ACCESS_INDEX((entry)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_INDEX((context), (base), (entry)));
            #else
                return (POTTERY_ARRAY_ACCESS_INDEX((base), (entry)));
            #endif
        #endif

    #elif defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE)
        // standard C array
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
        pottery_array_access_entry_t base = pottery_array_access_begin(
                POTTERY_ARRAY_ACCESS_SOLE_VALS);
        #endif
        return pottery_cast(size_t, entry - base);

    #else
        #error "Pottery template error!"
    #endif
}



/**
 * Returns the offset of the second entry relative to the first. This is the
 * difference in index between the two given entries (i.e. the number of elements
 * between them plus one.)
 *
 * If second comes before first, the return value is negative. If the entries
 * are equal, the return value is zero.
 *
 * This is the opposite of shift(). Calling shift() on the first entry with the
 * return value of this should return the second entry.
 */
static inline
ptrdiff_t pottery_array_access_offset(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_entry_t first,
        pottery_array_access_entry_t second)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_OFFSET
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_OFFSET((context), (first), (second)));
            #else
                return (POTTERY_ARRAY_ACCESS_OFFSET((first), (second)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_OFFSET((context), (base), (first), (second)));
            #else
                return (POTTERY_ARRAY_ACCESS_OFFSET((base), (first), (second)));
            #endif
        #endif

    #elif defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE) && \
                !defined(POTTERY_ARRAY_ACCESS_SELECT) && !defined(POTTERY_ARRAY_ACCESS_INDEX)
        // standard C array
        return pottery_cast(ptrdiff_t, second - first);

    #else
        // difference between indices
        return pottery_cast(ptrdiff_t, pottery_array_access_index(POTTERY_ARRAY_ACCESS_VALS second))
                -
                pottery_cast(ptrdiff_t, pottery_array_access_index(POTTERY_ARRAY_ACCESS_VALS first));
    #endif
}



/**
 * Returns a entry shifted forwards or backwards by a signed number of elements.
 */
static inline
pottery_array_access_entry_t pottery_array_access_shift(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_entry_t entry,
        ptrdiff_t offset)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_SHIFT
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_SHIFT((context), (entry), (offset)));
            #else
                return (POTTERY_ARRAY_ACCESS_SHIFT((entry), (offset)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_SHIFT((context), (base), (entry), (offset)));
            #else
                return (POTTERY_ARRAY_ACCESS_SHIFT((base), (entry), (offset)));
            #endif
        #endif

    #elif defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE) && \
                !defined(POTTERY_ARRAY_ACCESS_SELECT) && !defined(POTTERY_ARRAY_ACCESS_INDEX)
        // standard C array
        return entry + offset;

    #else
        // add offset to index
        return pottery_array_access_select(
                POTTERY_ARRAY_ACCESS_VALS
                pottery_cast(size_t,
                    pottery_cast(ptrdiff_t, pottery_array_access_index(
                        POTTERY_ARRAY_ACCESS_VALS
                        entry)) + offset));
    #endif
}



/**
 * Returns the entry following the given entry.
 */
static inline
pottery_array_access_entry_t pottery_array_access_next(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_entry_t entry)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_NEXT
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_NEXT((context), (entry)));
            #else
                return (POTTERY_ARRAY_ACCESS_NEXT((entry)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_NEXT((context), (base), (entry)));
            #else
                return (POTTERY_ARRAY_ACCESS_NEXT((base), (entry)));
            #endif
        #endif

    #else
        // shift by +1
        return pottery_array_access_shift(
                POTTERY_ARRAY_ACCESS_VALS
                entry, 1);
    #endif
}



/**
 * Returns the entry preceding the given entry.
 */
static inline
pottery_array_access_entry_t pottery_array_access_previous(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_entry_t entry)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_PREVIOUS
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_PREVIOUS((context), (entry)));
            #else
                return (POTTERY_ARRAY_ACCESS_PREVIOUS((entry)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_PREVIOUS((context), (base), (entry)));
            #else
                return (POTTERY_ARRAY_ACCESS_PREVIOUS((base), (entry)));
            #endif
        #endif

    #else
        // shift by -1
        return pottery_array_access_shift(
                POTTERY_ARRAY_ACCESS_VALS
                entry, -1);
    #endif
}



/**
 * Returns true if the given entries are equal; false otherwise.
 */
static inline
bool pottery_array_access_equal(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_entry_t left,
        pottery_array_access_entry_t right)
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
 * Returns true if the given entry exists (i.e. is not the end entry), false if it
 * does not (i.e. is the end entry.)
 */
static inline
bool pottery_array_access_exists(
        POTTERY_ARRAY_ACCESS_ARGS
        pottery_array_access_entry_t entry)
{
    POTTERY_ARRAY_ACCESS_ARGS_UNUSED;

    #ifdef POTTERY_ARRAY_ACCESS_EXISTS
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_EXISTS((context), (entry)));
            #else
                return (POTTERY_ARRAY_ACCESS_EXISTS((entry)));
            #endif
        #else
            #ifdef POTTERY_ARRAY_ACCESS_CONTEXT_TYPE
                return (POTTERY_ARRAY_ACCESS_EXISTS((context), (base), (entry)));
            #else
                return (POTTERY_ARRAY_ACCESS_EXISTS((base), (entry)));
            #endif
        #endif

    #elif defined(POTTERY_ARRAY_ACCESS_VALUE_TYPE) && \
                !defined(POTTERY_ARRAY_ACCESS_SELECT) && !defined(POTTERY_ARRAY_ACCESS_INDEX)
        // standard C array
        #if POTTERY_ARRAY_ACCESS_INHERENT_BASE
        pottery_array_access_entry_t base = pottery_array_access_begin(
                POTTERY_ARRAY_ACCESS_SOLE_VALS);
        #endif
        size_t total_count = pottery_array_access_count(POTTERY_ARRAY_ACCESS_SOLE_VALS);
        return entry != base + total_count;

    #else
        // compare against end entry
        return !pottery_array_access_equal(POTTERY_ARRAY_ACCESS_VALS entry,
                pottery_array_access_end(POTTERY_ARRAY_ACCESS_SOLE_VALS));
    #endif
}
#endif
