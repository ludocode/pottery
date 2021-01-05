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

typedef POTTERY_DEQUE_VALUE_TYPE pottery_deque_value_t;

// TODO maybe don't bother with this, it just makes it confusing.
typedef pottery_deque_value_t* pottery_deque_page_t;

typedef struct pottery_deque_entry_t {
    pottery_deque_page_t* page;
    pottery_deque_value_t* value;
} pottery_deque_entry_t;

typedef struct pottery_deque_t pottery_deque_t;

#if POTTERY_FORWARD_DECLARATIONS
static inline
bool pottery_deque_entry_exists(pottery_deque_t* deque, pottery_deque_entry_t entry);
#endif

static inline
pottery_deque_entry_t pottery_deque_entry_make(
        pottery_deque_page_t* page,
        pottery_deque_value_t* value)
{
    pottery_deque_entry_t entry = {page, value};
    return entry;
}

static inline
pottery_deque_value_t* pottery_deque_entry_value(
        pottery_deque_t* deque,
        pottery_deque_entry_t entry)
{
    (void)deque;
    pottery_assert(pottery_deque_entry_exists(deque, entry));
    return entry.value;
}
