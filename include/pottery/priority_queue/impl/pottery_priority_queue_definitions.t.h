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

#if POTTERY_PRIORITY_QUEUE_CAN_PASS
POTTERY_PRIORITY_QUEUE_EXTERN
pottery_error_t pottery_pq_insert(pottery_pq_t* pq,
        pottery_pq_value_t value)
{
    size_t old_count = pottery_pq_array_count(&pq->array);
    pottery_error_t error = pottery_pq_array_insert_last(&pq->array, value);
    if (pottery_likely(error == POTTERY_OK)) {
        pottery_pq_heap_expand_bulk(&pq->array, old_count, 1);
    }
    return error;
}

POTTERY_PRIORITY_QUEUE_EXTERN
pottery_error_t pottery_pq_insert_bulk(pottery_pq_t* pq,
        pottery_pq_value_t* values, size_t count)
{
    size_t old_count = pottery_pq_count(pq);
    pottery_error_t error = pottery_pq_array_insert_last_bulk(&pq->array, values, count);
    if (pottery_likely(error == POTTERY_OK)) {

        // If we're inserting a sizeable chunk of elements, it's faster to
        // append in bulk and repair. This fraction could be a tunable
        // parameter.
        if (count > old_count) {
            pottery_pq_repair(pq);
        } else {
            pottery_pq_heap_expand_bulk(&pq->array, old_count, count);
        }

    }
    return error;
}

POTTERY_PRIORITY_QUEUE_EXTERN
pottery_pq_value_t pottery_pq_extract_at(pottery_pq_t* pq, size_t index) {
    pottery_pq_heap_contract_at(&pq->array, pottery_pq_count(pq), index);
    return pottery_pq_array_extract_last(&pq->array);
}
#endif

POTTERY_PRIORITY_QUEUE_EXTERN
void pottery_pq_displace_at(pottery_pq_t* pq, size_t index) {
    pottery_pq_heap_contract_at(&pq->array, pottery_pq_count(pq), index);
    pottery_pq_array_displace_last(&pq->array);
}

POTTERY_PRIORITY_QUEUE_EXTERN
void pottery_pq_remove_at(pottery_pq_t* pq, size_t index) {
    pottery_pq_heap_contract_at(&pq->array, pottery_pq_count(pq), index);
    pottery_pq_array_remove_last(&pq->array);
}
