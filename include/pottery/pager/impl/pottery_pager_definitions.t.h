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

#ifndef POTTERY_PAGER_IMPL
#error "This is an internal header. Do not include it."
#endif

static inline size_t pottery_pager_per_page(void) {
    // It's tempting to use malloc_good_size() here, except then we'd have to
    // store it and access it at runtime. We really want this to be a
    // compile-time constant.
    //
    // Note that if you specify a custom PER_PAGE expression, it doesn't
    // actually have to be a compile-time constant (and it will eventually take
    // a context.) It does need to be constant for the lifetime of the pager
    // though. You cannot change your PER_PAGE even if the pager is empty; it
    // has to be destroyed first and re-initialized after.

    #ifdef POTTERY_PAGER_PER_PAGE
    return (POTTERY_PAGER_PER_PAGE);
    #else

    // Offset sizes to try to get glibc allocator to align to page size?
    // ptmalloc keeps a 16-24 byte header along allocations that would
    // otherwise make our page-sized allocations slightly bigger than a page.
    // I'm not sure if this actually improves anything.
    static const size_t offset =
        #ifdef __GLIBC__
        16
        #else
        0
        #endif
        ;

    // These are some hardcoded default values for page sizes based on the
    // value size. If you want some other page size, specify a custom PER_PAGE.
    if (sizeof(pottery_pager_value_t) < 32)
        return ((4*1024-offset) / sizeof(pottery_pager_value_t));
    if (sizeof(pottery_pager_value_t) < 96)
        return ((8*1024-offset) / sizeof(pottery_pager_value_t));
    if (sizeof(pottery_pager_value_t) < 256)
        return ((16*1024-offset) / sizeof(pottery_pager_value_t));
    if (sizeof(pottery_pager_value_t) < 2048)
        return ((32*1024-offset) / sizeof(pottery_pager_value_t));

    // For huge values, we'll just use a minimum of 16 per page (any less and
    // there wouldn't really be a point in using a pager.)
    return 16;

    #endif
}

static inline void pottery_pager_sanity_check(pottery_pager_t* pager) {
    pottery_assert(pottery_pager_is_empty(pager) == (pottery_pager_count(pager) == 0));

    if (pottery_pager_is_empty(pager)) {
        pottery_assert(pager->first_page_start == 0);
        pottery_assert(pager->last_page_end == pottery_pager_per_page());
    } else {
        pottery_assert(pager->first_page_start < pottery_pager_per_page());
        pottery_assert(pager->last_page_end > 0);
        pottery_assert(pager->last_page_end <= pottery_pager_per_page());
    }
}

// Returns null in case of error
static pottery_pager_page_t pottery_pager_acquire_page(pottery_pager_t* pager) {
    if (pager->spare != pottery_null) {
        pottery_pager_page_t page = pager->spare;
        pager->spare = pottery_null;
        return page;
    }

    return pottery_cast(pottery_pager_value_t*,
            pottery_pager_alloc_malloc_array(
                pottery_alignof(pottery_pager_value_t),
                pottery_pager_per_page(),
                sizeof(pottery_pager_value_t)));
}

static void pottery_pager_release_page(pottery_pager_t* pager, pottery_pager_page_t page) {
    if (pager->spare == pottery_null) {
        // We keep a spare page around for a bit of hysteresis. The pager won't
        // need to allocate as long as the count varies within a page-sized
        // range of elements.
        //
        // (Eventually we could make a configurable spare count and store
        // spares in a linked list but this isn't implemented yet.)
        pager->spare = page;
    } else {
        pottery_pager_alloc_free(pottery_alignof(pottery_pager_value_t), page);
    }
}

static void pottery_pager_set_clear(pottery_pager_t* pager) {
    // We set up the bounds at the ends of the non-existent zero page, so
    // appending to either end will cause it to add a page and we don't need
    // special cases for the first append.
    pager->first_page_start = 0;
    pager->last_page_end = pottery_pager_per_page();
}

POTTERY_PAGER_EXTERN
void pottery_pager_init(pottery_pager_t* pager) {
    pottery_pager_page_ring_init(&pager->pages);
    pager->spare = pottery_null;
    pottery_pager_set_clear(pager);
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
// Destroys all values without displacing them.
static void pottery_pager_destroy_all(pottery_pager_t* pager) {
    size_t page_count = pottery_pager_page_ring_count(&pager->pages);
    if (page_count == 0) {
        return;
    }

    if (page_count == 1) {
        // destroy contents of sole page
        pottery_pager_page_t page = *pottery_pager_page_ring_first(&pager->pages);
        pottery_pager_lifecycle_destroy_bulk(
                page + pager->first_page_start,
                pager->last_page_end - pager->first_page_start);
        return;
    }

    // destroy contents of first and last page
    pottery_pager_page_t* first = pottery_pager_page_ring_first(&pager->pages);
    pottery_pager_page_t* last = pottery_pager_page_ring_last(&pager->pages);
    size_t per_page = pottery_pager_per_page();
    pottery_pager_lifecycle_destroy_bulk(
            *first + pager->first_page_start, per_page - pager->first_page_start);
    pottery_pager_lifecycle_destroy_bulk(
            *last, pager->last_page_end);

    // destroy contents of all other pages
    pottery_pager_page_t* page = first;
    while (last != (page = pottery_pager_page_ring_next(&pager->pages, page))) {
        pottery_pager_lifecycle_destroy_bulk(*page, per_page);
    }
}

POTTERY_PAGER_EXTERN
void pottery_pager_remove_all(pottery_pager_t* pager) {
    pottery_pager_destroy_all(pager);
    pottery_pager_displace_all(pager);
}

// For the next two functions, it's probably not faster to iterate in bulk and
// destroy in bulk since the bulk destroy is almost certainly just a loop
// anyway, so it would be two nested loops. Instead we just having the single
// outer loop and destroy one by one.

POTTERY_PAGER_EXTERN
void pottery_pager_remove_first_bulk(pottery_pager_t* pager, size_t count) {
    pottery_pager_entry_t entry = pottery_pager_first(pager);
    size_t i;
    for (i = 0; i < count; ++i) {
        pottery_pager_lifecycle_destroy(pottery_pager_entry_value(pager, entry));
        entry = pottery_pager_next(pager, entry);
    }
    pottery_pager_displace_first_bulk(pager, count);
}

POTTERY_PAGER_EXTERN
void pottery_pager_remove_last_bulk(pottery_pager_t* pager, size_t count) {
    pottery_pager_entry_t entry = pottery_pager_last(pager);
    size_t i;
    for (i = 0; i < count; ++i) {
        pottery_pager_lifecycle_destroy(pottery_pager_entry_value(pager, entry));
        entry = pottery_pager_next(pager, entry);
    }
    pottery_pager_displace_last_bulk(pager, count);
}
#endif

POTTERY_PAGER_EXTERN
void pottery_pager_destroy(pottery_pager_t* pager) {

    #if POTTERY_LIFECYCLE_CAN_DESTROY
    pottery_pager_remove_all(pager);
    #else
    // Without a destroy expression it is an error to destroy a non-empty
    // container.
    pottery_assert(pottery_pager_is_empty(pager));
    #endif

    pottery_pager_page_ring_destroy(&pager->pages);
    if (pager->spare != pottery_null)
        pottery_pager_alloc_free(pottery_alignof(pottery_pager_value_t), pager->spare);
}

POTTERY_PAGER_EXTERN
size_t pottery_pager_count(pottery_pager_t* pager) {
    size_t page_count = pottery_pager_page_ring_count(&pager->pages);
    if (page_count == 0)
        return 0;
    return (pager->last_page_end - pager->first_page_start) +
            (page_count - 1) * pottery_pager_per_page();
}

POTTERY_PAGER_EXTERN
void pottery_pager_displace_all(pottery_pager_t* pager) {
    size_t i;
    for (i = 0; i < pottery_pager_page_ring_count(&pager->pages); ++i)
        pottery_pager_release_page(pager, *pottery_pager_page_ring_at(&pager->pages, i));
    pottery_pager_page_ring_remove_all(&pager->pages);
    pottery_pager_set_clear(pager);
    pottery_pager_sanity_check(pager);
}

POTTERY_PAGER_EXTERN
void pottery_pager_displace_first_bulk(pottery_pager_t* pager, size_t count) {
    pottery_pager_sanity_check(pager);
    pottery_assert(count <= pottery_pager_count(pager));

    // If we're displacing all elements, use the alternate displace code
    // instead to release all pages and reset the pager bounds. (We would need
    // to reset the pager bounds anyway if the pager is being emptied, and
    // there would need to be special behaviour for a single page (the left
    // calculation below would be wrong), so it's simpler to do it this way.)
    if (count == pottery_pager_count(pager)) {
        pottery_pager_displace_all(pager);
        return;
    }

    size_t per_page = pottery_pager_per_page();

    // This is only the amount left if there is more than one page. If there is
    // only one page, this isn't really the amount left, but count can't be
    // greater than or equal to the amount left anyway.
    size_t left = per_page - pager->first_page_start;
    if (count < left) {
        // There's enough elements in this page, plus elements left over so we
        // don't need to remove a page.
        pager->first_page_start += count;
        pottery_pager_sanity_check(pager);
        return;
    }

    // Release the first page
    pottery_pager_release_page(pager,
            pottery_pager_page_ring_extract_first(&pager->pages));
    count -= left;

    // Release additional full pages
    while (count >= per_page) {
        pottery_pager_release_page(pager,
                pottery_pager_page_ring_extract_first(&pager->pages));
        count -= per_page;
    }

    // Skip past any more elements left over in the new first page.
    pager->first_page_start = count;
    pottery_pager_sanity_check(pager);
}

POTTERY_PAGER_EXTERN
void pottery_pager_displace_last_bulk(pottery_pager_t* pager, size_t count) {
    pottery_pager_sanity_check(pager);
    pottery_assert(pottery_pager_count(pager) >= count);
    size_t per_page = pottery_pager_per_page();

    // If we're displacing all elements, use the alternate displace code
    // instead. See similar note above.
    if (count == pottery_pager_count(pager)) {
        pottery_pager_displace_all(pager);
        return;
    }

    // See note in displace_first_bulk() above: even if this is the sole page
    // and the actual number of elements in this page is less, count can't be
    // greater or equal to it anyway.
    if (count < pager->last_page_end) {
        // There's enough elements in this page, plus elements left over so we
        // don't need to remove a page.
        pager->last_page_end -= count;
        pottery_pager_sanity_check(pager);
        return;
    }

    // Release the last page
    pottery_pager_release_page(pager,
            pottery_pager_page_ring_extract_last(&pager->pages));
    count -= pager->last_page_end;

    // Release additional full pages
    while (count >= per_page) {
        pottery_pager_release_page(pager,
                pottery_pager_page_ring_extract_last(&pager->pages));
        count -= per_page;
    }

    // Skip past any more elements left over in the new last page.
    pager->last_page_end = per_page - count;
    pottery_pager_sanity_check(pager);
}

POTTERY_PAGER_EXTERN
pottery_pager_entry_t pottery_pager_select(pottery_pager_t* pager, size_t index) {
    pottery_pager_sanity_check(pager);
    pottery_assert(index <= pottery_pager_count(pager));

    size_t page_count = pottery_pager_page_ring_count(&pager->pages);

    // Check for empty
    if (page_count == 0)
        return pottery_pager_entry_make(pottery_null, pottery_null);

    size_t per_page = pottery_pager_per_page();

    // Check if we're on the first page.
    // Note that if there's only one page, the number of values it contains is
    // actually (pager->last_page_end - pager->first_page_start) but the index
    // is bounded by that range as well so we don't need to check it.
    size_t first_page_count = per_page - pager->first_page_start;
    if (index < first_page_count) {
        pottery_pager_page_t* page = pottery_pager_page_ring_at(&pager->pages, 0);
        return pottery_pager_entry_make(page, *page + pager->first_page_start + index);
    }

    index -= first_page_count;
    size_t page_index = 1 + index / per_page;
    pottery_pager_page_t* page = pottery_pager_page_ring_at(&pager->pages, page_index);
    return pottery_pager_entry_make(page, *page + index % per_page);
}

POTTERY_PAGER_EXTERN
size_t pottery_pager_index(pottery_pager_t* pager, pottery_pager_entry_t entry) {
    // Check for empty
    if (entry.page == pottery_null)
        return 0;

    size_t page_index = pottery_pager_page_ring_index(&pager->pages, entry.page);
    size_t page_offset = pottery_cast(size_t, entry.value - *entry.page);

    // Check if the element is on the first page
    if (page_index == 0)
        return page_offset - pager->first_page_start;

    // Other pages
    size_t per_page = pottery_pager_per_page();
    return (per_page - pager->first_page_start) + per_page * (page_index - 1) + page_offset;
}

POTTERY_PAGER_EXTERN
pottery_error_t pottery_pager_emplace_first_bulk(pottery_pager_t* pager, pottery_pager_entry_t* entry, size_t count) {
    pottery_pager_sanity_check(pager);
    size_t per_page = pottery_pager_per_page();
    pottery_pager_page_t* page;

    // note that if the pager is empty, first_page_start is 0 so we don't need
    // any special case for empty.

    if (count <= pager->first_page_start) {
        // the entire emplacement fits in the first page so we don't need to
        // add any pages.
        page = pottery_pager_page_ring_first(&pager->pages);
        pager->first_page_start -= count;
    } else {
        count -= pager->first_page_start;

        // add additional pages to fit the rest of the emplacement
        size_t pages_added = 0;
        while (true) {
            int error = pottery_pager_page_ring_emplace_first(&pager->pages, &page);
            if (error == POTTERY_OK) {
                *page = pottery_pager_acquire_page(pager);
                if (*page == pottery_null) {
                    error = POTTERY_ERROR_ALLOC;
                    pottery_pager_page_ring_displace_first(&pager->pages);
                }
            }

            // on failure remove any pages we added
            if (error != POTTERY_OK) {
                while (pages_added > 0) {
                    --pages_added;
                    pottery_pager_release_page(pager,
                            pottery_pager_page_ring_extract_first(&pager->pages));
                }
                pottery_pager_sanity_check(pager);
                return error;
            }

            ++pages_added;
            if (count <= per_page)
                break;

            count -= per_page;
        }

        pager->first_page_start = per_page - count;
    }

    *entry = pottery_pager_entry_make(page, *page + pager->first_page_start);
    pottery_pager_sanity_check(pager);
    return POTTERY_OK;
}

POTTERY_PAGER_EXTERN
pottery_error_t pottery_pager_emplace_last_bulk(pottery_pager_t* pager, pottery_pager_entry_t* entry, size_t count) {
    pottery_pager_sanity_check(pager);
    size_t per_page = pottery_pager_per_page();

    // We need to store the index of the page that will hold the first entry to
    // our new emplaced values to re-reference it after the ring is done
    // growing. It's either the last page or the one we will add after it.
    size_t page_index = pottery_pager_page_ring_count(&pager->pages);
    size_t value_index = 0;

    if (pager->last_page_end < per_page) {
        size_t left = per_page - pager->last_page_end;
        if (count <= left) {
            // the entire emplacement fits in the last page so we don't need to
            // add any pages.
            pottery_pager_page_t* page = pottery_pager_page_ring_last(&pager->pages);
            *entry = pottery_pager_entry_make(page, *page + pager->last_page_end);
            pager->last_page_end += count;
            pottery_pager_sanity_check(pager);
            return POTTERY_OK;
        }

        count -= left;
        --page_index;
        value_index = pager->last_page_end;
    }

    // add additional pages to fit the rest of the emplacement
    size_t pages_added = 0;
    while (true) {
        pottery_pager_page_t* page;
        int error = pottery_pager_page_ring_emplace_last(&pager->pages, &page);
        if (error == POTTERY_OK) {
            *page = pottery_pager_acquire_page(pager);
            if (*page == pottery_null) {
                error = POTTERY_ERROR_ALLOC;
                pottery_pager_page_ring_displace_last(&pager->pages);
            }
        }

        // on failure remove any pages we added
        if (error != POTTERY_OK) {
            while (pages_added > 0) {
                --pages_added;
                pottery_pager_release_page(pager,
                        pottery_pager_page_ring_extract_last(&pager->pages));
            }
            pottery_pager_sanity_check(pager);
            return error;
        }

        ++pages_added;
        if (count <= per_page)
            break;

        count -= per_page;
    }

    // Now that we're done adding pages, we can set the entry.
    pottery_pager_page_t* page = pottery_pager_page_ring_at(&pager->pages, page_index);
    *entry = pottery_pager_entry_make(page, *page + value_index);
    pager->last_page_end = count;
    pottery_pager_sanity_check(pager);
    return POTTERY_OK;
}

#if POTTERY_LIFECYCLE_CAN_PASS
POTTERY_PAGER_EXTERN
pottery_error_t pottery_pager_insert_last_bulk(pottery_pager_t* pager,
        const pottery_pager_value_t* values, size_t count)
{
    pottery_pager_sanity_check(pager);

    // create space
    pottery_pager_entry_t entry;
    pottery_error_t error = pottery_pager_emplace_last_bulk(pager, &entry, count);
    if (error != POTTERY_OK)
        return error;

    // move values in bulk
    while (count > 0) {
        size_t step = count;
        pottery_pager_value_t* step_values = pottery_pager_next_bulk(pager, &entry, &step);

        // Use move construction (rather than the lifecycle move() operation)
        // to move values out of the array. We need to leave them constructed
        // in C++. See notes in pottery_vector_extract() for more details.
        size_t i = 0;
        for (; i < step; ++i)
            *(step_values++) = pottery_move_if_cxx(*(values++));

        // TODO add lifecycle function move_construct_bulk. we'd like to do
        // this instead of the above loop
        //pottery_lifecycle_move_construct_bulk(step_values, values, step);
        //values += step;

        count -= step;
    }

    pottery_pager_sanity_check(pager);
    return POTTERY_OK;
}

POTTERY_PAGER_EXTERN
pottery_error_t pottery_pager_insert_first_bulk(pottery_pager_t* pager, const pottery_pager_value_t* values, size_t count) {
    pottery_pager_sanity_check(pager);

    // create space
    pottery_pager_entry_t entry;
    pottery_error_t error = pottery_pager_emplace_first_bulk(pager, &entry, count);
    if (error != POTTERY_OK)
        return error;

    // move values in bulk
    while (count > 0) {
        size_t step = count;
        pottery_pager_value_t* step_values = pottery_pager_next_bulk(pager, &entry, &step);

        // Use move construction (rather than the lifecycle move() operation)
        // to move values out of the array. We need to leave them constructed
        // in C++. See notes in pottery_vector_extract() for more details.
        size_t i = 0;
        for (; i < step; ++i)
            *(step_values++) = pottery_move_if_cxx(*(values++));

        // TODO add lifecycle function move_construct_bulk. we'd like to do
        // this instead of the above loop
        //pottery_lifecycle_move_construct_bulk(step_values, values, step);
        //values += step;

        count -= step;
    }

    pottery_pager_sanity_check(pager);
    return POTTERY_OK;
}

POTTERY_PAGER_EXTERN
void pottery_pager_extract_first_bulk(pottery_pager_t* pager, pottery_pager_value_t* values, size_t count) {
    // TODO performance
    while (count > 0) {
        *(values++) = pottery_pager_extract_first(pager);
        --count;
    }
}

POTTERY_PAGER_EXTERN
void pottery_pager_extract_last_bulk(pottery_pager_t* pager, pottery_pager_value_t* values, size_t count) {
    // TODO performance
    values += count;
    while (count > 0) {
        --values;
        *values = pottery_pager_extract_last(pager);
        --count;
    }
}
#endif

POTTERY_PAGER_EXTERN
pottery_pager_value_t* pottery_pager_next_bulk(pottery_pager_t* pager,
        pottery_pager_entry_t* entry, size_t* count)
{
    pottery_pager_sanity_check(pager);
    pottery_assert(pottery_pager_entry_exists(pager, *entry));

    pottery_pager_value_t* value = entry->value;

    if (entry->page == pottery_pager_page_ring_last(&pager->pages)) {
        // last page. we may or may not consume it all.
        size_t left = pottery_cast(size_t, *entry->page + pager->last_page_end - value);
        (void)left;
        pottery_assert(*count <= left);
        entry->value += *count;
    } else {
        size_t left = pottery_cast(size_t, *entry->page + pottery_pager_per_page() - value);
        if (*count >= left) {
            // consume rest of page, move to next page
            *count = left;
            entry->page = pottery_pager_page_ring_next(&pager->pages, entry->page);
            entry->value = *entry->page;
        } else {
            // staying in same page
            entry->value += *count;
        }
    }

    return value;
}

POTTERY_PAGER_EXTERN
pottery_pager_value_t* pottery_pager_previous_bulk(pottery_pager_t* pager,
        pottery_pager_entry_t* entry, size_t* count)
{
    pottery_pager_sanity_check(pager);
    pottery_assert(pottery_pager_index(pager, *entry) > 0);

    pottery_pager_value_t* value = entry->value;
    size_t left = pottery_cast(size_t, value - *entry->page) + 1;

    if (entry->page == pottery_pager_page_ring_first(&pager->pages)) {
        // first page. we may or may not consume it all.
        pottery_assert(*count <= left);
        if (*count == left) {
            *entry = pottery_pager_end(pager);
        } else {
            entry->value -= *count;
        }
    } else {
        if (*count >= left) {
            // consume rest of page, move to previous page
            *count = left;
            entry->page = pottery_pager_page_ring_previous(&pager->pages, entry->page);
            entry->value = *entry->page + pottery_pager_per_page() - 1;
        } else {
            // staying in same page
            entry->value -= *count;
        }
    }

    return value;
}
