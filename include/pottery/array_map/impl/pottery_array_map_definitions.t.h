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

#ifndef POTTERY_ARRAY_MAP_IMPL
#error "This is an internal header. Do not include it."
#endif

POTTERY_ARRAY_MAP_EXTERN
void pottery_array_map_init(pottery_array_map_t* map
    #ifdef POTTERY_ARRAY_MAP_CONTEXT_TYPE
    , pottery_context_t context
    #endif
) {
    pottery_array_map_vector_init(&map->vector);
    #ifdef POTTERY_ARRAY_MAP_CONTEXT_TYPE
    map->context = context;
    #endif
}

#if POTTERY_COMPARE_CAN_ORDER
// Using binary search, finds the array entry with the smallest key that is
// larger than or equal to the given key, or the end of the array if no such
// entry exists.
//
// In other words, if an entry with a matching key doesn't exist, it finds
// the entry where it should be inserted.
//
// Returns true if an entry matching the given key was found.
static
bool pottery_array_map_find_slot(pottery_array_map_t* map,
        pottery_array_map_key_t key, pottery_array_map_entry_t* out_entry)
{
    if (pottery_array_map_is_empty(map)) {
        *out_entry = pottery_array_map_end(map);
        return false;
    }

    size_t l = 0;
    size_t r = pottery_array_map_vector_count(&map->vector);
    pottery_array_map_entry_t entry;
    pottery_array_map_key_t entry_key;

    do {
        size_t m = l + (r - l) / 2;
        entry = pottery_array_map_vector_select(&map->vector, m);
        pottery_assert(pottery_array_map_entry_exists(map, entry));
        entry_key = pottery_array_map_value_key(
                pottery_array_map_vector_entry_ref(&map->vector, entry));
        if (pottery_array_map_compare_greater(key, entry_key)) {
            l = m + 1;
        } else {
            r = m;
        }
    } while (l < r);

    entry = pottery_array_map_vector_select(&map->vector, l);
    *out_entry = entry;
    return pottery_array_map_entry_exists(map, entry) &&
            pottery_array_map_compare_equal(key, pottery_array_map_value_key(
                pottery_array_map_vector_entry_ref(&map->vector, entry)));
}
#endif

POTTERY_ARRAY_MAP_EXTERN
pottery_array_map_value_t* pottery_array_map_find(pottery_array_map_t* map,
        pottery_array_map_key_t key)
{
    #if POTTERY_COMPARE_CAN_ORDER

        // binary search. O(logn)
        pottery_array_map_entry_t entry;
        if (pottery_array_map_find_slot(map, key, &entry))
            return entry;
        return pottery_array_map_end(map);

    #else

        // linear search. O(n)
        size_t i;
        for (i = 0; i < pottery_array_map_vector_count(&map->vector); ++i) {
            pottery_array_map_value_t* value = pottery_array_map_vector_at(&map->vector, i);
            if (pottery_array_map_compare_equal(key, pottery_array_map_value_key(value)))
                return value;
        }
        return pottery_array_map_end(map);

    #endif
}

POTTERY_ARRAY_MAP_EXTERN
pottery_error_t pottery_array_map_emplace_key(pottery_array_map_t* map,
        pottery_array_map_key_t key, pottery_array_map_entry_t* entry,
        bool* /*nullable*/ created)
{
    #if POTTERY_COMPARE_CAN_ORDER

        // find the entry by key, or the entry where this should be inserted
        bool found = pottery_array_map_find_slot(map, key, entry);
        if (found) {
            if (created)
                *created = false;
            return POTTERY_OK;
        } else {
            if (created)
                *created = true;
            // insert at this entry
            // TODO arrays are supposed to be able to emplace at an entry
            // (emplace_before().) for now we do it by index.
            return pottery_array_map_vector_emplace_at(&map->vector,
                    pottery_array_map_vector_index(&map->vector, *entry), entry);
        }

    #else

        // see if the key already exists
        pottery_array_map_value_t* value = pottery_array_map_find(map, key);
        if (pottery_array_map_entry_exists(map, value)) {
            if (created)
                *created = false;
            *entry = value;
            return POTTERY_OK;
        }

        // append a new value at the end
        if (created)
            *created = true;
        return pottery_array_map_vector_emplace_last(&map->vector, entry);

    #endif
}

POTTERY_ARRAY_MAP_EXTERN
bool pottery_array_map_displace_key(pottery_array_map_t* map, pottery_array_map_key_t key) {
    pottery_array_map_entry_t entry = pottery_array_map_find(map, key);
    if (!pottery_array_map_entry_exists(map, entry))
        return false;
    pottery_array_map_displace(map, entry);
    return true;
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_ARRAY_MAP_EXTERN
void pottery_array_map_remove(pottery_array_map_t* map,
        pottery_array_map_entry_t entry)
{
    pottery_array_map_lifecycle_destroy(pottery_array_map_entry_ref(map, entry));
    pottery_array_map_displace(map, entry);
}

POTTERY_ARRAY_MAP_EXTERN
bool pottery_array_map_remove_key(pottery_array_map_t* map, pottery_array_map_key_t key) {
    pottery_array_map_entry_t entry = pottery_array_map_find(map, key);
    if (!pottery_array_map_entry_exists(map, entry))
        return false;
    pottery_array_map_remove(map, entry);
    return true;
}
#endif

POTTERY_ARRAY_MAP_EXTERN
void pottery_array_map_displace(pottery_array_map_t* map,
        pottery_array_map_value_t* value)
{
    pottery_assert(pottery_array_map_entry_exists(map, value));

    #if POTTERY_COMPARE_CAN_ORDER

        // displace the element from its location in the vector, keeping
        // everything sorted
        pottery_array_map_vector_displace(&map->vector, value);

    #else

        // move the last element into this element's place and truncate
        // (the move pointers here may be the same if this is already the last
        // element; move() checks this for  us)
        pottery_array_map_lifecycle_move(value, pottery_array_map_vector_last(&map->vector));
        pottery_array_map_vector_displace_last(&map->vector);

    #endif
}

#if POTTERY_LIFECYCLE_CAN_PASS && POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_ARRAY_MAP_EXTERN
pottery_error_t pottery_array_map_insert(pottery_array_map_t* map,
        pottery_array_map_value_t value)
{
    bool created;
    pottery_array_map_entry_t entry;
    pottery_error_t error = pottery_array_map_emplace_key(map,
            pottery_array_map_ref_key(&value), &entry, &created);
    if (error == POTTERY_OK) {
        if (!created)
            pottery_array_map_lifecycle_destroy(entry);
        pottery_move_construct(pottery_array_map_value_t, *entry, value);
    }
    return error;
}
#endif // insert criteria
