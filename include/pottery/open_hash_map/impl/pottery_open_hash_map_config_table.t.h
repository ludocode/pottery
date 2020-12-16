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

#ifndef POTTERY_OPEN_HASH_MAP_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif

#define POTTERY_OPEN_HASH_TABLE_PREFIX POTTERY_CONCAT(POTTERY_OPEN_HASH_MAP_PREFIX, _table)

#define POTTERY_OPEN_HASH_TABLE_VALUE_TYPE pottery_ohm_value_t
#define POTTERY_OPEN_HASH_TABLE_KEY_TYPE pottery_ohm_key_t

// Our table takes only the map as context, and it will have an inherent base
// because we give it a BEGIN expression which gets our map's values. This
// means we just need to pass the map as the only common argument to all table
// functions.
#define POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_BEGIN pottery_ohm_impl_values
#define POTTERY_OPEN_HASH_TABLE_ARRAY_ACCESS_COUNT pottery_ohm_bucket_count
#define POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE pottery_ohm_t*

// Note that we don't forward along lifecycle configuration because we need to
// provide our map as context. Instead we give it our wrappers for move and
// destroy.

#if defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_MOVE) || \
        defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_MOVE_BY_VALUE) || \
        defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_BY_VALUE)
    #define POTTERY_OPEN_HASH_TABLE_LIFECYCLE_MOVE pottery_ohm_entry_move
#endif
#if defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_DESTROY) || \
        defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_DESTROY_BY_VALUE) || \
        defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_BY_VALUE)
    #define POTTERY_OPEN_HASH_TABLE_LIFECYCLE_DESTROY pottery_ohm_entry_destroy
#endif

// We also manually forward along the hash table configuration. Anything
// configured on the hash map must be forwarded to the table through a wrapper
// that changes the context.

#define POTTERY_OPEN_HASH_TABLE_KEY_HASH pottery_ohm_entry_key_hash
#define POTTERY_OPEN_HASH_TABLE_REF_KEY pottery_ohm_entry_key
#define POTTERY_OPEN_HASH_TABLE_KEY_EQUAL pottery_ohm_entry_key_equal
#ifdef POTTERY_OPEN_HASH_TABLE_KEY_DOUBLE_HASH
    #define POTTERY_OPEN_HASH_TABLE_KEY_DOUBLE_HASH pottery_ohm_entry_key_double_hash
#endif

#define POTTERY_OPEN_HASH_TABLE_IS_EMPTY pottery_ohm_entry_is_empty
#define POTTERY_OPEN_HASH_TABLE_SET_EMPTY pottery_ohm_entry_set_empty

#if POTTERY_OPEN_HASH_MAP_TOMBSTONES
#define POTTERY_OPEN_HASH_TABLE_IS_TOMBSTONE pottery_ohm_entry_is_tombstone
#define POTTERY_OPEN_HASH_TABLE_SET_TOMBSTONE pottery_ohm_entry_set_tombstone
#endif

#define POTTERY_OPEN_HASH_TABLE_IS_VALUE pottery_ohm_entry_is_value

#define POTTERY_OPEN_HASH_TABLE_LINEAR_PROBING POTTERY_OPEN_HASH_MAP_LINEAR_PROBING
#define POTTERY_OPEN_HASH_TABLE_QUADRATIC_PROBING POTTERY_OPEN_HASH_MAP_QUADRATIC_PROBING
#define POTTERY_OPEN_HASH_TABLE_DOUBLE_HASHING POTTERY_OPEN_HASH_MAP_DOUBLE_HASHING
