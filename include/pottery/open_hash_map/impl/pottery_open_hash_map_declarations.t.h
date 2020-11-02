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
#error "This is an internal header. Do not include it."
#endif

typedef pottery_ohm_oht_value_t pottery_ohm_value_t;
typedef pottery_ohm_oht_ref_t pottery_ohm_ref_t;
typedef pottery_ohm_oht_key_t pottery_ohm_key_t;

typedef struct pottery_ohm_t {
    // TODO shouldn't actually store table here, don't need self-pointer
    pottery_ohm_oht_t table;
    pottery_ohm_oht_value_t* values;
    uint8_t* metadata;
} pottery_ohm_t;

POTTERY_OPEN_HASH_MAP_EXTERN
pottery_error_t pottery_ohm_init(pottery_ohm_t* ohm);

POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_destroy(pottery_ohm_t* ohm);

/**
 *
 * Returns a non-existent ref in case of error.
 */
POTTERY_OPEN_HASH_MAP_EXTERN
pottery_ohm_ref_t pottery_ohm_emplace(pottery_ohm_t* ohm,
        pottery_ohm_key_t key, bool* /*nullable*/ created);

static inline
pottery_ohm_ref_t pottery_ohm_find(pottery_ohm_t* ohm, pottery_ohm_key_t key) {
    return pottery_ohm_oht_find(&ohm->table, key);
}

POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_displace(pottery_ohm_t* ohm, pottery_ohm_ref_t ref);

// TODO if can remove
POTTERY_OPEN_HASH_MAP_EXTERN
void pottery_ohm_remove(pottery_ohm_t* ohm, pottery_ohm_ref_t ref);

// TODO if can remove
/**
 * Returns true if an entry matching the given key was removed.
 */
POTTERY_OPEN_HASH_MAP_EXTERN
bool pottery_ohm_remove_key(pottery_ohm_t* ohm, pottery_ohm_key_t key);

static inline
bool pottery_ohm_ref_exists(pottery_ohm_t* ohm, pottery_ohm_ref_t ref) {
    return pottery_ohm_oht_ref_exists(&ohm->table, ref);
}
