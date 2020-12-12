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

#ifndef POTTERY_DEQUE_IMPL
#error "This is an internal header. Do not include it."
#endif

static inline size_t pottery_deque_per_page(void) {
    // It's tempting to use malloc_good_size() here, except then we'd have to
    // store it and access it at runtime. We really want this to be a
    // compile-time constant.
    //
    // Note that if you specify a custom PER_PAGE expression, it doesn't
    // actually have to be a compile-time constant (and it will eventually take
    // a context.) It does need to be constant for the lifetime of the deque
    // though. You cannot change your PER_PAGE even if the deque is empty; it
    // has to be destroyed first and re-initialized after.

    #ifdef POTTERY_DEQUE_PER_PAGE
    return (POTTERY_DEQUE_PER_PAGE);
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
    if (sizeof(pottery_deque_value_t) < 32)
        return ((4*1024-offset) / sizeof(pottery_deque_value_t));
    if (sizeof(pottery_deque_value_t) < 96)
        return ((8*1024-offset) / sizeof(pottery_deque_value_t));
    if (sizeof(pottery_deque_value_t) < 256)
        return ((16*1024-offset) / sizeof(pottery_deque_value_t));
    if (sizeof(pottery_deque_value_t) < 2048)
        return ((32*1024-offset) / sizeof(pottery_deque_value_t));

    // For huge values, we'll just use a minimum of 16 per page (any less and
    // there wouldn't really be a point in using a deque.)
    return 16;

    #endif
}

static inline void pottery_deque_sanity_check(pottery_deque_t* deque) {
    pottery_assert(pottery_deque_is_empty(deque) == (pottery_deque_count(deque) == 0));

    if (pottery_deque_is_empty(deque)) {
        pottery_assert(deque->first_page_start == 0);
        pottery_assert(deque->last_page_end == pottery_deque_per_page());
    } else {
        pottery_assert(deque->first_page_start < pottery_deque_per_page());
        pottery_assert(deque->last_page_end > 0);
        pottery_assert(deque->last_page_end <= pottery_deque_per_page());
    }
}

// Returns null in case of error
static pottery_deque_page_t pottery_deque_acquire_page(pottery_deque_t* deque) {
    if (deque->spare != pottery_null) {
        pottery_deque_page_t page = deque->spare;
        deque->spare = pottery_null;
        return page;
    }

    return pottery_cast(pottery_deque_value_t*,
            pottery_deque_alloc_malloc_array(
                pottery_alignof(pottery_deque_value_t),
                pottery_deque_per_page(),
                sizeof(pottery_deque_value_t)));
}

static void pottery_deque_release_page(pottery_deque_t* deque, pottery_deque_page_t page) {
    if (deque->spare == pottery_null) {
        // We keep a spare page around for a bit of hysteresis. The deque won't
        // need to allocate as long as the count varies within a page-sized
        // range of elements.
        //
        // (Eventually we could make a configurable spare count and store
        // spares in a linked list but this isn't implemented yet.)
        deque->spare = page;
    } else {
        pottery_deque_alloc_free(pottery_alignof(pottery_deque_value_t), page);
    }
}

static void pottery_deque_set_clear(pottery_deque_t* deque) {
    // We set up the bounds at the ends of the non-existent zero page, so
    // appending to either end will cause it to add a page and we don't need
    // special cases for the first append.
    deque->first_page_start = 0;
    deque->last_page_end = pottery_deque_per_page();
}

POTTERY_DEQUE_EXTERN
void pottery_deque_init(pottery_deque_t* deque) {
    pottery_deque_page_ring_init(&deque->pages);
    deque->spare = pottery_null;
    pottery_deque_set_clear(deque);
}

#if POTTERY_LIFECYCLE_CAN_DESTROY
// Destroys all values without displacing them.
static void pottery_deque_destroy_all(pottery_deque_t* deque) {
    size_t page_count = pottery_deque_page_ring_count(&deque->pages);
    if (page_count == 0) {
        return;
    }

    if (page_count == 1) {
        // destroy contents of sole page
        pottery_deque_page_t page = *pottery_deque_page_ring_first(&deque->pages);
        pottery_deque_lifecycle_destroy_bulk(
                page + deque->first_page_start,
                deque->last_page_end - deque->first_page_start);
        return;
    }

    // destroy contents of first and last page
    pottery_deque_page_t* first = pottery_deque_page_ring_first(&deque->pages);
    pottery_deque_page_t* last = pottery_deque_page_ring_last(&deque->pages);
    size_t per_page = pottery_deque_per_page();
    pottery_deque_lifecycle_destroy_bulk(
            *first + deque->first_page_start, per_page - deque->first_page_start);
    pottery_deque_lifecycle_destroy_bulk(
            *last, deque->last_page_end);

    // destroy contents of all other pages
    pottery_deque_page_t* page = first;
    while (last != (page = pottery_deque_page_ring_next(&deque->pages, page))) {
        pottery_deque_lifecycle_destroy_bulk(*page, per_page);
    }
}

POTTERY_DEQUE_EXTERN
void pottery_deque_remove_all(pottery_deque_t* deque) {
    pottery_deque_destroy_all(deque);
    pottery_deque_displace_all(deque);
}

// For the next two functions, it's probably not faster to iterate in bulk and
// destroy in bulk since the bulk destroy is almost certainly just a loop
// anyway, so it would be two nested loops. Instead we just having the single
// outer loop and destroy one by one.

POTTERY_DEQUE_EXTERN
void pottery_deque_remove_first_bulk(pottery_deque_t* deque, size_t count) {
    pottery_deque_ref_t ref = pottery_deque_first(deque);
    size_t i;
    for (i = 0; i < count; ++i) {
        pottery_deque_lifecycle_destroy(pottery_deque_ref_value(deque, ref));
        ref = pottery_deque_next(deque, ref);
    }
    pottery_deque_displace_first_bulk(deque, count);
}

POTTERY_DEQUE_EXTERN
void pottery_deque_remove_last_bulk(pottery_deque_t* deque, size_t count) {
    pottery_deque_ref_t ref = pottery_deque_last(deque);
    size_t i;
    for (i = 0; i < count; ++i) {
        pottery_deque_lifecycle_destroy(pottery_deque_ref_value(deque, ref));
        ref = pottery_deque_previous(deque, ref);
    }
    pottery_deque_displace_last_bulk(deque, count);
}
#endif

POTTERY_DEQUE_EXTERN
void pottery_deque_destroy(pottery_deque_t* deque) {

    #if POTTERY_LIFECYCLE_CAN_DESTROY
    pottery_deque_remove_all(deque);
    #else
    // Without a destroy expression it is an error to destroy a non-empty
    // container.
    pottery_assert(pottery_deque_is_empty(deque));
    #endif

    pottery_deque_page_ring_destroy(&deque->pages);
    if (deque->spare != pottery_null)
        pottery_deque_alloc_free(pottery_alignof(pottery_deque_value_t), deque->spare);
}

POTTERY_DEQUE_EXTERN
size_t pottery_deque_count(pottery_deque_t* deque) {
    size_t page_count = pottery_deque_page_ring_count(&deque->pages);
    if (page_count == 0)
        return 0;
    return (deque->last_page_end - deque->first_page_start) +
            (page_count - 1) * pottery_deque_per_page();
}

POTTERY_DEQUE_EXTERN
void pottery_deque_displace_all(pottery_deque_t* deque) {
    size_t i;
    for (i = 0; i < pottery_deque_page_ring_count(&deque->pages); ++i)
        pottery_deque_release_page(deque, *pottery_deque_page_ring_at(&deque->pages, i));
    pottery_deque_page_ring_remove_all(&deque->pages);
    pottery_deque_set_clear(deque);
    pottery_deque_sanity_check(deque);
}

POTTERY_DEQUE_EXTERN
void pottery_deque_displace_first_bulk(pottery_deque_t* deque, size_t count) {
    pottery_deque_sanity_check(deque);
    pottery_assert(count <= pottery_deque_count(deque));

    // If we're displacing all elements, use the alternate displace code
    // instead to release all pages and reset the deque bounds. (We would need
    // to reset the deque bounds anyway if the deque is being emptied, and
    // there would need to be special behaviour for a single page (the left
    // calculation below would be wrong), so it's simpler to do it this way.)
    if (count == pottery_deque_count(deque)) {
        pottery_deque_displace_all(deque);
        return;
    }

    size_t per_page = pottery_deque_per_page();

    // This is only the amount left if there is more than one page. If there is
    // only one page, this isn't really the amount left, but count can't be
    // greater than or equal to the amount left anyway.
    size_t left = per_page - deque->first_page_start;
    if (count < left) {
        // There's enough elements in this page, plus elements left over so we
        // don't need to remove a page.
        deque->first_page_start += count;
        pottery_deque_sanity_check(deque);
        return;
    }

    // Release the first page
    pottery_deque_release_page(deque,
            pottery_deque_page_ring_extract_first(&deque->pages));
    count -= left;

    // Release additional full pages
    while (count >= per_page) {
        pottery_deque_release_page(deque,
                pottery_deque_page_ring_extract_first(&deque->pages));
        count -= per_page;
    }

    // Skip past any more elements left over in the new first page.
    deque->first_page_start = count;
    pottery_deque_sanity_check(deque);
}

POTTERY_DEQUE_EXTERN
void pottery_deque_displace_last_bulk(pottery_deque_t* deque, size_t count) {
    pottery_deque_sanity_check(deque);
    pottery_assert(pottery_deque_count(deque) >= count);
    size_t per_page = pottery_deque_per_page();

    // If we're displacing all elements, use the alternate displace code
    // instead. See similar note above.
    if (count == pottery_deque_count(deque)) {
        pottery_deque_displace_all(deque);
        return;
    }

    // See note in displace_first_bulk() above: even if this is the sole page
    // and the actual number of elements in this page is less, count can't be
    // greater or equal to it anyway.
    if (count < deque->last_page_end) {
        // There's enough elements in this page, plus elements left over so we
        // don't need to remove a page.
        deque->last_page_end -= count;
        pottery_deque_sanity_check(deque);
        return;
    }

    // Release the last page
    pottery_deque_release_page(deque,
            pottery_deque_page_ring_extract_last(&deque->pages));
    count -= deque->last_page_end;

    // Release additional full pages
    while (count >= per_page) {
        pottery_deque_release_page(deque,
                pottery_deque_page_ring_extract_last(&deque->pages));
        count -= per_page;
    }

    // Skip past any more elements left over in the new last page.
    deque->last_page_end = per_page - count;
    pottery_deque_sanity_check(deque);
}

POTTERY_DEQUE_EXTERN
pottery_deque_ref_t pottery_deque_at(pottery_deque_t* deque, size_t index) {
    pottery_deque_sanity_check(deque);
    pottery_assert(index < pottery_deque_count(deque));

    size_t page_count = pottery_deque_page_ring_count(&deque->pages);
    size_t per_page = pottery_deque_per_page();

    size_t first_page_count = ((page_count == 1) ? deque->last_page_end : per_page) -
            deque->first_page_start;

    if (index < first_page_count) {
        pottery_deque_page_t* page = pottery_deque_page_ring_at(&deque->pages, 0);
        return pottery_deque_ref_make(page, *page + deque->first_page_start + index);
    }

    index -= first_page_count;
    size_t page_index = 1 + index / per_page;
    pottery_deque_page_t* page = pottery_deque_page_ring_at(&deque->pages, page_index);
    return pottery_deque_ref_make(page, *page + index % per_page);
}

POTTERY_DEQUE_EXTERN
pottery_error_t pottery_deque_emplace_first_bulk(pottery_deque_t* deque, pottery_deque_ref_t* ref, size_t count) {
    pottery_deque_sanity_check(deque);
    size_t per_page = pottery_deque_per_page();

    bool had_first_page_space = false;
    size_t value_index;

    if (deque->first_page_start > 0) {
        size_t left = deque->first_page_start;
        if (count <= left) {
            // the entire emplacement fits in the first page so we don't need to
            // add any pages.
            pottery_deque_page_t* page = pottery_deque_page_ring_first(&deque->pages);
            *ref = pottery_deque_ref_make(page, *page + deque->first_page_start - 1);
            deque->first_page_start -= count;
            pottery_deque_sanity_check(deque);
            return POTTERY_OK;
        }

        count -= left;
        had_first_page_space = true;
        value_index = deque->first_page_start - 1;
    } else {
        value_index = per_page - 1;
    }

    // add additional pages to fit the rest of the emplacement
    size_t pages_added = 0;
    while (true) {
        pottery_deque_page_t* page;
        int error = pottery_deque_page_ring_emplace_first(&deque->pages, &page);
        if (error == POTTERY_OK) {
            *page = pottery_deque_acquire_page(deque);
            if (*page == pottery_null) {
                error = POTTERY_ERROR_ALLOC;
                pottery_deque_page_ring_displace_first(&deque->pages);
            }
        }

        // on failure remove any pages we added
        if (error != POTTERY_OK) {
            while (pages_added > 0) {
                --pages_added;
                pottery_deque_release_page(deque,
                        pottery_deque_page_ring_extract_first(&deque->pages));
            }
            pottery_deque_sanity_check(deque);
            return error;
        }

        ++pages_added;
        if (count <= per_page)
            break;

        count -= per_page;
    }

    // Now that we're done adding pages, we can set the ref.
    pottery_deque_page_t* page = pottery_deque_page_ring_at(&deque->pages,
            pages_added - (had_first_page_space ? 0 : 1));
    *ref = pottery_deque_ref_make(page, *page + value_index);
    deque->first_page_start = per_page - count;
    pottery_deque_sanity_check(deque);
    return POTTERY_OK;
}

POTTERY_DEQUE_EXTERN
pottery_error_t pottery_deque_emplace_last_bulk(pottery_deque_t* deque, pottery_deque_ref_t* ref, size_t count) {
    pottery_deque_sanity_check(deque);
    size_t per_page = pottery_deque_per_page();

    // We need to store the index of the page that will hold the first ref to
    // our new emplaced values to re-reference it after the ring is done
    // growing. It's either the last page or the one we will add after it.
    size_t page_index = pottery_deque_page_ring_count(&deque->pages);
    size_t value_index = 0;

    if (deque->last_page_end < per_page) {
        size_t left = per_page - deque->last_page_end;
        if (count <= left) {
            // the entire emplacement fits in the last page so we don't need to
            // add any pages.
            pottery_deque_page_t* page = pottery_deque_page_ring_last(&deque->pages);
            *ref = pottery_deque_ref_make(page, *page + deque->last_page_end);
            deque->last_page_end += count;
            pottery_deque_sanity_check(deque);
            return POTTERY_OK;
        }

        count -= left;
        --page_index;
        value_index = deque->last_page_end;
    }

    // add additional pages to fit the rest of the emplacement
    size_t pages_added = 0;
    while (true) {
        pottery_deque_page_t* page;
        int error = pottery_deque_page_ring_emplace_last(&deque->pages, &page);
        if (error == POTTERY_OK) {
            *page = pottery_deque_acquire_page(deque);
            if (*page == pottery_null) {
                error = POTTERY_ERROR_ALLOC;
                pottery_deque_page_ring_displace_last(&deque->pages);
            }
        }

        // on failure remove any pages we added
        if (error != POTTERY_OK) {
            while (pages_added > 0) {
                --pages_added;
                pottery_deque_release_page(deque,
                        pottery_deque_page_ring_extract_last(&deque->pages));
            }
            pottery_deque_sanity_check(deque);
            return error;
        }

        ++pages_added;
        if (count <= per_page)
            break;

        count -= per_page;
    }

    // Now that we're done adding pages, we can set the ref.
    pottery_deque_page_t* page = pottery_deque_page_ring_at(&deque->pages, page_index);
    *ref = pottery_deque_ref_make(page, *page + value_index);
    deque->last_page_end = count;
    pottery_deque_sanity_check(deque);
    return POTTERY_OK;
}

#if POTTERY_LIFECYCLE_CAN_PASS
POTTERY_DEQUE_EXTERN
pottery_error_t pottery_deque_insert_last_bulk(pottery_deque_t* deque,
        const pottery_deque_value_t* values, size_t count)
{
    pottery_deque_sanity_check(deque);

    // create space
    pottery_deque_ref_t ref;
    pottery_error_t error = pottery_deque_emplace_last_bulk(deque, &ref, count);
    if (error != POTTERY_OK)
        return error;

    // move values in bulk
    while (count > 0) {
        size_t step = count;
        pottery_deque_value_t* step_values = pottery_deque_next_bulk(deque, &ref, &step);

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

    pottery_deque_sanity_check(deque);
    return POTTERY_OK;
}

POTTERY_DEQUE_EXTERN
pottery_error_t pottery_deque_insert_first_bulk(pottery_deque_t* deque, const pottery_deque_value_t* values, size_t count) {
    pottery_deque_sanity_check(deque);

    // create space
    pottery_deque_ref_t ref;
    pottery_error_t error = pottery_deque_emplace_first_bulk(deque, &ref, count);
    if (error != POTTERY_OK)
        return error;

    // move values in bulk
    while (count > 0) {
        size_t step = count;
        pottery_deque_value_t* step_values = pottery_deque_previous_bulk(deque, &ref, &step);

        // Use move construction (rather than the lifecycle move() operation)
        // to move values out of the array. We need to leave them constructed
        // in C++. See notes in pottery_vector_extract() for more details.
        size_t i = 0;
        for (; i < step; ++i)
            *(step_values--) = pottery_move_if_cxx(values[count - 1 - i]);

        // TODO add lifecycle function move_construct_bulk. we'd like to do
        // this instead of the above loop
        //pottery_lifecycle_move_construct_bulk(step_values, values, step);
        //values += step;

        count -= step;
    }

    pottery_deque_sanity_check(deque);
    return POTTERY_OK;
}

POTTERY_DEQUE_EXTERN
void pottery_deque_extract_first_bulk(pottery_deque_t* deque, pottery_deque_value_t* values, size_t count) {
    // TODO performance
    while (count > 0) {
        *(values++) = pottery_deque_extract_first(deque);
        --count;
    }
}

POTTERY_DEQUE_EXTERN
void pottery_deque_extract_last_bulk(pottery_deque_t* deque, pottery_deque_value_t* values, size_t count) {
    // TODO performance
    values += count;
    while (count > 0) {
        --values;
        *values = pottery_deque_extract_last(deque);
        --count;
    }
}
#endif

POTTERY_DEQUE_EXTERN
pottery_deque_value_t* pottery_deque_next_bulk(pottery_deque_t* deque,
        pottery_deque_ref_t* ref, size_t* count)
{
    pottery_deque_sanity_check(deque);
    pottery_assert(pottery_deque_ref_exists(deque, *ref));

    pottery_deque_value_t* value = ref->value;

    if (ref->page == pottery_deque_page_ring_last(&deque->pages)) {
        // last page. we may or may not consume it all.
        size_t left = pottery_cast(size_t, *ref->page + deque->last_page_end - value);
        pottery_assert(*count <= left);
        ref->value += left;
    } else {
        size_t left = pottery_cast(size_t, *ref->page + pottery_deque_per_page() - value);
        if (*count >= left) {
            // consume rest of page, move to next page
            *count = left;
            ref->page = pottery_deque_page_ring_next(&deque->pages, ref->page);
            ref->value = *ref->page;
        } else {
            // staying in same page
            ref->value += *count;
        }
    }

    return value;
}

POTTERY_DEQUE_EXTERN
pottery_deque_value_t* pottery_deque_previous_bulk(pottery_deque_t* deque,
        pottery_deque_ref_t* ref, size_t* count)
{
    pottery_deque_sanity_check(deque);
    pottery_assert(pottery_deque_ref_exists(deque, *ref));

    pottery_deque_value_t* value = ref->value;
    size_t left = pottery_cast(size_t, value - *ref->page) + 1;

    if (ref->page == pottery_deque_page_ring_first(&deque->pages)) {
        // first page. we may or may not consume it all.
        pottery_assert(*count <= left);
        if (*count == left) {
            *ref = pottery_deque_end(deque);
        } else {
            ref->value -= left;
        }
    } else {
        if (*count >= left) {
            // consume rest of page, move to previous page
            *count = left;
            ref->page = pottery_deque_page_ring_previous(&deque->pages, ref->page);
            ref->value = *ref->page + pottery_deque_per_page() - 1;
        } else {
            // staying in same page
            ref->value -= *count;
        }
    }

    return value;
}
