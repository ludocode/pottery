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

#ifndef POTTERY_CONTAINER_TYPES_INTERNAL
#error "This is an internal header. Do not include it."
#endif

#if defined(POTTERY_CONTAINER_TYPES_VALUE_TYPE)
typedef POTTERY_CONTAINER_TYPES_VALUE_TYPE pottery_container_types_value_t;
#endif

#ifdef POTTERY_CONTAINER_TYPES_REF_TYPE
// With a custom ref type, const is disabled.
typedef POTTERY_CONTAINER_TYPES_REF_TYPE pottery_container_types_ref_t;
typedef pottery_container_types_ref_t pottery_container_types_const_ref_t;
#else
// If a value type is provided, the ref type is a pointer to it.
typedef pottery_container_types_value_t* pottery_container_types_ref_t;
typedef const pottery_container_types_ref_t pottery_container_types_const_ref_t;
#endif

#ifdef POTTERY_CONTAINER_TYPES_ENTRY_TYPE
typedef POTTERY_CONTAINER_TYPES_ENTRY_TYPE pottery_container_types_entry_t;
#else
// The entry type defaults to the ref type.
typedef pottery_container_types_ref_t pottery_container_types_entry_t;
#endif

#ifdef POTTERY_CONTAINER_TYPES_KEY_TYPE
typedef POTTERY_CONTAINER_TYPES_KEY_TYPE pottery_container_types_key_t;
#endif

#ifdef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE
typedef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE pottery_container_types_context_t;
#endif

/**
 * Returns a ref for an entry.
 *
 * The entry must exist.
 */
static inline
pottery_container_types_ref_t pottery_container_types_entry_ref(
        #ifdef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE
        pottery_container_types_context_t context,
        #endif
        pottery_container_types_entry_t entry)
{
    #ifdef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE
    (void)context;
    #endif

    #ifdef POTTERY_CONTAINER_TYPES_ENTRY_REF
        #ifdef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE
            return (POTTERY_CONTAINER_TYPES_ENTRY_REF((context), (entry)));
        #else
            return (POTTERY_CONTAINER_TYPES_ENTRY_REF((entry)));
        #endif
    #else
        // Without an ENTRY_REF expression, the entry type must implicitly
        // convertible to the ref type.
        return entry;
    #endif
}

/**
 * Returns true if the given refs are equal.
 *
 * Note that this compares the *refs*, not the values they point to! If this is
 * passed two different refs with the same value, it returns false.
 */
static inline
bool pottery_container_types_ref_equal(
        #ifdef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE
        pottery_container_types_context_t context,
        #endif
        pottery_container_types_ref_t left,
        pottery_container_types_ref_t right)
{
    #ifdef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE
    (void)context;
    #endif

    #ifdef POTTERY_CONTAINER_TYPES_REF_EQUAL
        #ifdef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE
            return (POTTERY_CONTAINER_TYPES_REF_EQUAL((context), (left), (right)));
        #else
            return (POTTERY_CONTAINER_TYPES_REF_EQUAL((left), (right)));
        #endif
    #else
        // Without a REF_EQUAL expression, the refs must be comparable with
        // operator ==.
        return left == right;
    #endif
}

#ifdef POTTERY_CONTAINER_TYPES_KEY_TYPE
/**
 * Returns the key for the given ref.
 */
static inline
pottery_container_types_key_t pottery_container_types_ref_key(
        #ifdef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE
        pottery_container_types_context_t context,
        #endif
        pottery_container_types_ref_t ref)
{
    #ifdef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE
    (void)context;
    #endif

    #ifdef POTTERY_CONTAINER_TYPES_REF_KEY
        #ifdef POTTERY_CONTAINER_TYPES_CONTEXT_TYPE
            return (POTTERY_CONTAINER_TYPES_REF_KEY((context), (ref)));
        #else
            return (POTTERY_CONTAINER_TYPES_REF_KEY((ref)));
        #endif
    #else
        // Without a REF_KEY expression, the ref type must be implicitly
        // convertible to the key type.
        return ref;
    #endif
}
#endif
