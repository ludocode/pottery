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

#if 0
#ifndef POTTERY_OPEN_HASH_MAP_IMPL
#error "This is an internal header. Do not include it."
#endif

// The value is the user type stored in each bucket.
#ifdef POTTERY_OPEN_HASH_MAP_VALUE_TYPE
typedef POTTERY_OPEN_HASH_MAP_VALUE_TYPE pottery_ohm_value_t;
#endif

// The ref is an abstract handle to an element and bucket in the hash table.
#ifdef POTTERY_OPEN_HASH_MAP_REF_TYPE
typedef POTTERY_OPEN_HASH_MAP_REF_TYPE pottery_ohm_ref_t;
#else
typedef pottery_ohm_value_t* pottery_ohm_ref_t;
#endif

// The key type is an abstract handle to a key used for lookups in the hash
// table. Each element in the hash table has a unique key.
#ifdef POTTERY_OPEN_HASH_MAP_KEY_TYPE
typedef POTTERY_OPEN_HASH_MAP_KEY_TYPE pottery_ohm_key_t;
#else
typedef pottery_ohm_ref_t pottery_ohm_key_t;
#endif

#ifdef POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE
typedef POTTERY_OPEN_HASH_MAP_CONTEXT_TYPE pottery_ohm_context_t;
#endif

#if !defined(POTTERY_OPEN_HASH_MAP_IS_EMPTY) && !defined(POTTERY_OPEN_HASH_MAP_IS_TOMBSTONE)
typedef enum pottery_ohm_bucket_state_t {
    pottery_ohm_bucket_state_empty = 0,
    pottery_ohm_bucket_state_element,
    #if !defined(POTTERY_OPEN_HASH_MAP_IS_TOMBSTONE)
    pottery_ohm_bucket_state_tombstone,
    #endif
} pottery_ohm_bucket_state_t;
#endif
#endif
