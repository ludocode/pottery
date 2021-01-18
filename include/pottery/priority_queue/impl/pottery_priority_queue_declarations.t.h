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

#ifndef POTTERY_PRIORITY_QUEUE_IMPL
#error "This is an internal header. Do not include it."
#endif

typedef struct pottery_pq_t {
    pottery_pq_array_t array;
} pottery_pq_t;

static inline
void pottery_pq_init(pottery_pq_t* pq
        #if POTTERY_PRIORITY_QUEUE_HAS_CONTEXT
        , p_context_t context
        #endif
) {
    pottery_pq_array_init(&pq->array
            #if POTTERY_PRIORITY_QUEUE_HAS_CONTEXT
            , context
            #endif
            );
}

static inline
void pottery_pq_destroy(pottery_pq_t* pq) {
    pottery_pq_array_destroy(&pq->array);
}

static inline
size_t pottery_pq_count(pottery_pq_t* pq) {
    return pottery_pq_array_count(&pq->array);
}

static inline
bool pottery_pq_is_empty(pottery_pq_t* pq) {
    return pottery_pq_array_is_empty(&pq->array);
}

#if POTTERY_PRIORITY_QUEUE_CAN_DESTROY
static inline
void pottery_pq_remove_all(pottery_pq_t* pq) {
    pottery_pq_array_remove_all(&pq->array);
}
#endif

static inline
void pottery_pq_displace_all(pottery_pq_t* pq) {
    pottery_pq_array_displace_all(&pq->array);
}

static inline
void pottery_pq_swap(pottery_pq_t* left, pottery_pq_t* right) {
    pottery_pq_array_swap(&left->array, &right->array);
}

#if POTTERY_PRIORITY_QUEUE_CAN_PASS
/**
 * Extracts the value at the given index.
 */
POTTERY_PRIORITY_QUEUE_EXTERN
pottery_pq_value_t pottery_pq_extract_at(pottery_pq_t* pq, size_t index);

/**
 * Extracts the first value of the priority queue (the one that comes before
 * all others, i.e. the one that compares greatest.)
 */
static inline
pottery_pq_value_t pottery_pq_extract_first(pottery_pq_t* pq) {
    return pottery_pq_extract_at(pq, 0);
}
#endif

/**
 * Displaces the value at the given index.
 */
POTTERY_PRIORITY_QUEUE_EXTERN
void pottery_pq_displace_at(pottery_pq_t* pq, size_t index);

/**
 * Displaces the first (greatest) value.
 */
static inline
void pottery_pq_displace_first(pottery_pq_t* pq) {
    pottery_pq_displace_at(pq, 0);
}

/**
 * Returns a pointer to the first (greatest) value.
 */
static inline
pottery_pq_value_t* pottery_pq_first(pottery_pq_t* pq) {
    return pottery_pq_array_first(&pq->array);
}

#if POTTERY_PRIORITY_QUEUE_CAN_DESTROY
/**
 * Removes the value at the given index, destroying it.
 */
POTTERY_PRIORITY_QUEUE_EXTERN
void pottery_pq_remove_at(pottery_pq_t* pq, size_t index);

/**
 * Removes the first (greatest) value, destroying it.
 */
static inline
void pottery_pq_remove_first(pottery_pq_t* pq) {
    pottery_pq_remove_at(pq, 0);
}
#endif

#if POTTERY_PRIORITY_QUEUE_CAN_PASS
POTTERY_PRIORITY_QUEUE_EXTERN
pottery_error_t pottery_pq_insert(pottery_pq_t* pq, pottery_pq_value_t value);

/**
 * Appends a value to the end of the priority queue without restoring the heap
 * property.
 *
 * Use this if you want to insert many elements and repair the priority queue
 * afterwards rather than sifting the priority queue for each value.
 *
 * You must call pottery_pq_repair() before extracting elements and before
 * using the index of any element (since indices will not be set until the
 * queue is repaired.)
 *
 * @see pottery_pq_repair()
 */
static inline
pottery_error_t pottery_pq_insert_broken(pottery_pq_t* pq, pottery_pq_value_t value) {
    return pottery_pq_array_insert_last(&pq->array, value);
}

POTTERY_PRIORITY_QUEUE_EXTERN
pottery_error_t pottery_pq_insert_bulk(pottery_pq_t* pq,
        pottery_pq_value_t* values, size_t count);

/**
 * Appends values to the end of the priority queue without restoring the heap
 * property.
 *
 * Use this if you want to insert many elements and repair the priority queue
 * afterwards rather than sifting the priority queue for each value.
 *
 * You must call pottery_pq_repair() before extracting elements and before
 * using the index of any element (since indices will not be set until the
 * queue is repaired.)
 *
 * @see pottery_pq_repair()
 */
static inline
pottery_error_t pottery_pq_insert_broken_bulk(pottery_pq_t* pq,
        pottery_pq_value_t* values, size_t count)
{
    return pottery_pq_array_insert_last_bulk(&pq->array, values, count);
}
#endif

/**
 * Repairs a priority queue after adding or removing values using the broken
 * variants, or after changing the sort conditions.
 *
 * In particular, with a comparison context (or global variables) it is
 * possible to modify the comparison criteria so that it gives different values
 * for the same operands. It is safe to change the comparison criteria as long
 * as you call pottery_pq_repair() before extracting elements.
 */
static inline
void pottery_pq_repair(pottery_pq_t* pq) {
    pottery_pq_heap_build(&pq->array, pottery_pq_array_count(&pq->array));
}
