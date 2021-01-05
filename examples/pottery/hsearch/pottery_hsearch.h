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

#ifndef POTTERY_HSEARCH_H
#define POTTERY_HSEARCH_H 1

#include "pottery/pottery_dependencies.h"

#ifdef __cplusplus
extern "C" {
#endif



/*
 * POSIX and extension types
 */

typedef enum {
    POTTERY_FIND,
    POTTERY_ENTER,
} POTTERY_ACTION;

typedef struct {
    char *key;
    void *data;
} POTTERY_ENTRY;

// This implementation makes the Pottery map visible in the public API. You
// could instead hide the implementation and declare the public struct to
// contain a pointer to it (or the appropriate amount of reserved space if you
// want binary compatibility with existing implementations.) You would probably
// also want to un-inline any inline functions here.

static inline size_t pottery_hsearch_fnv1a(const char* p) {
    uint32_t hash = 2166136261;
    for (; *p != 0; ++p)
        hash = (hash ^ (uint8_t)*p) * 16777619;
    return hash;
}

#include "pottery_hsearch_ohm.t.h"
#include "pottery/open_hash_map/pottery_open_hash_map_declare.t.h"

struct pottery_hsearch_data {
    pottery_hsearch_impl_t impl;
};

extern struct pottery_hsearch_data pottery_hsearch_global;


/*
 * NetBSD extensions
 */

void pottery_hdestroy1_r(struct pottery_hsearch_data *htab,
        void (*free_key)(void*), void (*free_data)(void*));

static inline
void pottery_hdestroy1(void (*free_key)(void*), void (*free_data)(void*)) {
    pottery_hdestroy1_r(&pottery_hsearch_global, free_key, free_data);
}


/*
 * Reentrant extensions
 */

// This is the musl/BSD version which does not require zeroing. It will not
// detect a double-create error as GNU's hcreate_r() does.
int pottery_hcreate_r(size_t number_of_elements, struct pottery_hsearch_data* table);

int pottery_hsearch_r(POTTERY_ENTRY item, POTTERY_ACTION action,
        POTTERY_ENTRY** entry, struct pottery_hsearch_data* table);

static inline
void pottery_hdestroy_r(struct pottery_hsearch_data *htab) {
    pottery_hdestroy1_r(htab, NULL, NULL);
}


/*
 * Original POSIX API
 */

int pottery_hcreate(size_t number_of_elements);

static inline
POTTERY_ENTRY *pottery_hsearch(POTTERY_ENTRY item, POTTERY_ACTION action) {
    POTTERY_ENTRY* entry;
    pottery_hsearch_r(item, action, &entry, &pottery_hsearch_global);
    return entry;
}

void pottery_hdestroy(void);


/*
 * OpenBSD API
 */

static inline
void pottery_hdestroy_openbsd(void) {
    pottery_hdestroy1(free, NULL);
}


/*
 * Optional identifier renaming
 */

#if defined(POTTERY_HSEARCH_REAL_API) && !defined(POTTERY_HSEARCH_IMPL)
    // Original POSIX API
    #define ENTRY POTTERY_ENTRY
    #define ACTION POTTERY_ACTION
    #define FIND POTTERY_FIND
    #define ENTER POTTERY_ENTER
    #define hcreate pottery_hcreate
    #define hsearch pottery_hsearch
    #define hdestroy pottery_hdestroy

    // Reentrant extensions
    #define hsearch_data pottery_hsearch_data
    #define hcreate_r pottery_hcreate_r
    #define hsearch_r pottery_hsearch_r
    #define hdestroy_r pottery_hdestroy_r

    // NetBSD extensions
    #define hdestroy1 pottery_hdestroy1
    #define hdestroy1_r pottery_hdestroy1_r

    // OpenBSD API
    #define hdestroy_openbsd pottery_hdestroy_openbsd
#endif


#ifdef __cplusplus
}
#endif

#endif
