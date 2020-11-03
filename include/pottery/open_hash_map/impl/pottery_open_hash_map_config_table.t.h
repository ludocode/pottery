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

// The context for the internal open_hash_table is our open_hash_map. We give
// it empty and tombstone accessors for our metadata.

#define POTTERY_OPEN_HASH_TABLE_REF_TYPE pottery_ohm_ref_t
#define POTTERY_OPEN_HASH_TABLE_KEY_TYPE pottery_ohm_key_t
#define POTTERY_OPEN_HASH_TABLE_CONTEXT_TYPE pottery_ohm_t*

// Note that we don't forward along lifecycle configuration. Instead we give it
// our wrappers for move and destroy.

#if defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_MOVE) || \
        defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_MOVE_BY_VALUE) || \
        defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_BY_VALUE)
    #define POTTERY_OPEN_HASH_TABLE_LIFECYCLE_MOVE pottery_ohm_ref_move
#endif
#if defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_DESTROY) || \
        defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_DESTROY_BY_VALUE) || \
        defined(POTTERY_OPEN_HASH_MAP_LIFECYCLE_BY_VALUE)
    #define POTTERY_OPEN_HASH_TABLE_LIFECYCLE_DESTROY pottery_ohm_ref_destroy
#endif

// We also manually forward along the hash table configuration. Anything
// configured on the hash map must be forwarded to the table through a wrapper
// that changes the context.

#define POTTERY_OPEN_HASH_TABLE_KEY_HASH pottery_ohm_ref_key_hash
#define POTTERY_OPEN_HASH_TABLE_KEY_FOR_VALUE pottery_ohm_ref_key
#define POTTERY_OPEN_HASH_TABLE_KEY_EQUAL pottery_ohm_ref_key_equal

#define POTTERY_OPEN_HASH_TABLE_IS_EMPTY pottery_ohm_ref_is_empty
#define POTTERY_OPEN_HASH_TABLE_SET_EMPTY pottery_ohm_ref_set_empty

#if POTTERY_OPEN_HASH_MAP_TOMBSTONES
#define POTTERY_OPEN_HASH_TABLE_IS_TOMBSTONE pottery_ohm_ref_is_tombstone
#define POTTERY_OPEN_HASH_TABLE_SET_TOMBSTONE pottery_ohm_ref_set_tombstone
#endif

// TODO rename IS_ELEMENT to IS_VALUE
#define POTTERY_OPEN_HASH_TABLE_IS_ELEMENT pottery_ohm_ref_is_value

#define POTTERY_OPEN_HASH_TABLE_LINEAR_PROBING POTTERY_OPEN_HASH_MAP_LINEAR_PROBING
#define POTTERY_OPEN_HASH_TABLE_QUADRATIC_PROBING POTTERY_OPEN_HASH_MAP_QUADRATIC_PROBING
#define POTTERY_OPEN_HASH_TABLE_DOUBLE_HASHING POTTERY_OPEN_HASH_MAP_DOUBLE_HASHING
