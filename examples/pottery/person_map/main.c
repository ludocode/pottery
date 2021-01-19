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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
 * First let's define a person struct.
 */

typedef struct person_t {
    char* name;
    int age;
} person_t;

static person_t* person_new(const char* name, int age) {
    person_t* person = (person_t*) malloc(sizeof(person_t));
    person->name = strdup(name);
    person->age = age;
    return person;
}

static void person_delete(person_t* person) {
    free(person->name);
    free(person);
}



/*
 * Now we instantiate a map of strings to persons.
 *
 * This example uses array_map. You could also use an open_hash_map (with a
 * hash function), a red_black_map, etc.
 */

// We'll call our map "person_map".
#define POTTERY_ARRAY_MAP_PREFIX person_map

// We'll use `const char*` as the key, not `char*`, that way we can search it
// with const strings.
#define POTTERY_ARRAY_MAP_KEY_TYPE const char*
#define POTTERY_ARRAY_MAP_COMPARE_THREE_WAY strcmp

// We are storing `person_t*` as the value type so the ref type is a
// double-pointer `person_t**` (as well as the entry type for most map
// types.)
#define POTTERY_ARRAY_MAP_VALUE_TYPE person_t*
#define POTTERY_ARRAY_MAP_REF_KEY(person) (*person)->name

// array_map requires a way to move its values. Pointers are bitwise-movable.
#define POTTERY_ARRAY_MAP_LIFECYCLE_MOVE_BY_VALUE 1

// We'd like our map to own the persons it contains so it needs to be able to
// destroy them when removing them. We'll be able to call remove() or destroy
// the whole map and it will delete values for us.
#define POTTERY_ARRAY_MAP_LIFECYCLE_DESTROY(person) person_delete(*person)

#include "pottery/array_map/pottery_array_map_static.t.h"



/*
 * Here's an example of how you could use the above map.
 */

int main(void) {
    person_map_t map;
    person_map_init(&map);


    // Add some persons to the map
    person_map_insert(&map, person_new("alice", 25));
    person_map_insert(&map, person_new("bob", 32));
    person_map_insert(&map, person_new("carl", 19));
    person_map_insert(&map, person_new("dave", 57));


    // Find alice
    person_t** entry = person_map_find(&map, "alice");
    printf("%s is %i years old.\n", (*entry)->name, (*entry)->age);

    // Remove alice from the map. The map deletes alice.
    person_map_remove(&map, entry);


    // Find bob
    entry = person_map_find(&map, "bob");
    if (!person_map_entry_exists(&map, entry)) {
        fprintf(stderr, "bob not found!\n");
        abort();
    }

    // Extract bob from the map. This returns bob without deleting him.
    person_t* bob = person_map_extract(&map, entry);

    // We now own bob. We have to delete him when done.
    printf("%s is %i years old.\n", bob->name, bob->age);
    person_delete(bob);


    // Remove dave by key. This deletes carl.
    if (person_map_remove_key(&map, "carl")) {
        printf("carl was removed.\n");
    }


    // Try to find eve. Eve should not exist.
    entry = person_map_find(&map, "eve");
    if (person_map_entry_exists(&map, entry)) {
        fprintf(stderr, "eve does not belong in the map!\n");
        abort();
    }
    printf("eve was not found.\n");


    // Destroy the map. This deletes the remaining persons (dave).
    printf("destroying map containing dave.\n");
    person_map_destroy(&map);
    return EXIT_SUCCESS;
}
