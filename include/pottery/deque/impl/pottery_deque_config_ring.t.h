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

// deque uses an internal ring to store its page list.
#define POTTERY_RING_PREFIX POTTERY_DEQUE_NAME(_page_ring)
#define POTTERY_RING_VALUE_TYPE pottery_deque_page_t

// The ring will use the same allocator as the deque (but not lifecycle
// since it's storing a different type.)
#define POTTERY_RING_EXTERNAL_ALLOC POTTERY_DEQUE_NAME(_alloc)
#define POTTERY_RING_LIFECYCLE_BY_VALUE 1

// This makes deque not bitwise movable. Maybe this should be configurable.
#define POTTERY_RING_INTERNAL_CAPACITY 4
