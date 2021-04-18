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

#define POTTERY_HSEARCH_IMPL
#include "pottery_hsearch.h"

#include <errno.h>

#include "pottery_hsearch_ohm.t.h"
#include "pottery/open_hash_map/pottery_open_hash_map_define.t.h"

struct pottery_hsearch_data pottery_hsearch_global;
static bool pottery_hsearch_global_table_exists = false;

int pottery_hcreate_r(size_t number_of_elements, struct pottery_hsearch_data* table) {

    // GNU checks for NULL. Not a BSD or POSIX requirement.
    if (table == pottery_null) {
        errno = EINVAL;
        return 0;
    }

    // We ignore the given number of elements and just grow as needed.
    (void)number_of_elements;
    pottery_hsearch_impl_init(&table->impl);

    return 1;
}

int pottery_hsearch_r(POTTERY_ENTRY item, POTTERY_ACTION action,
        POTTERY_ENTRY** entry, struct pottery_hsearch_data* table)
{
    // The GNU man page says it checks the table for NULL on hcreate_r() and
    // hdestroy_r(), but not hsearch_r()? We do it anyway. NULL checks are not
    // a BSD or POSIX requirement.
    if (table != pottery_null) {
        if (action == POTTERY_FIND) {
            pottery_hsearch_impl_entry_t impl_entry =
                    pottery_hsearch_impl_find(&table->impl, item.key);
            if (!pottery_hsearch_impl_entry_exists(&table->impl, impl_entry)) {
                *entry = pottery_null;
                // GNU and BSD require that we set errno to ESRCH. This is not
                // actually a POSIX requirement.
                errno = ESRCH;
            } else {
                *entry = impl_entry;
            }
            return 1;

        } else if (action == POTTERY_ENTER) {
            pottery_hsearch_impl_entry_t impl_entry;
            if (POTTERY_OK != pottery_hsearch_impl_emplace_key(&table->impl, item.key, &impl_entry, pottery_null)) {
                // The BSDs require that entry be set to NULL on error. This is also
                // necessary to allow our hsearch() wrapper to ignore the return value.
                *entry = pottery_null;
                errno = ENOMEM;
                return 0;
            }
            *entry = impl_entry;
            **entry = item;
            return 1;
        }
    }

    // null table or unrecognized action
    *entry = pottery_null;
    errno = EINVAL;
    return 0;
}

void pottery_hdestroy1_r(struct pottery_hsearch_data *table,
        void (*free_key)(void*), void (*free_data)(void*))
{
    // GNU checks for pottery_null.
    if (table == pottery_null) {
        errno = EINVAL;
        return;
    }

    // NetBSD's free functions. Note that we are freeing keys without removing
    // them from the map. The only operation we do after this is destroying the
    // map which does not involve any key comparisons so it's safe.
    if (free_key != pottery_null && free_data != pottery_null) {
        pottery_hsearch_impl_entry_t impl_entry = pottery_hsearch_impl_begin(&table->impl);
        while (pottery_hsearch_impl_entry_exists(&table->impl, impl_entry)) {
            POTTERY_ENTRY* entry = impl_entry;
            if (free_key)
                free_key(entry->key);
            if (free_data)
                free_data(entry->data);
            impl_entry = pottery_hsearch_impl_next(&table->impl, impl_entry);
        }
    }

    pottery_hsearch_impl_destroy(&table->impl);
}

int pottery_hcreate(size_t number_of_elements) {

    // FreeBSD specifies that this should set errno to EINVAL if a table
    // already exists. The GNU and NetBSD man pages do not specify this.
    if (pottery_hsearch_global_table_exists) {
        errno = EINVAL;
        return 0;
    }

    // If this fails hcreate_r() will set errno.
    if (!pottery_hcreate_r(number_of_elements, &pottery_hsearch_global))
        return 0;

    pottery_hsearch_global_table_exists = true;
    return 1;
}

void pottery_hdestroy(void) {

    // None of the man pages I've found specify the behaviour of hdestroy() if
    // the table does not exist. Since we have the flag for FreeBSD we may as
    // well check it and set EINVAL.
    if (!pottery_hsearch_global_table_exists) {
        errno = EINVAL;
        return;
    }

    pottery_hdestroy_r(&pottery_hsearch_global);
    pottery_hsearch_global_table_exists = false;
}
