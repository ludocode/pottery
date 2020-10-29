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

#ifndef POTTERY_CXX_ALGORITHMS_HEAP_HXX
#define POTTERY_CXX_ALGORITHMS_HEAP_HXX 1

#include "pottery_cxx/pottery_cxx_dependencies.hxx"

namespace pottery {

namespace impl {

#undef POTTERY_FORWARD_DECLARATIONS
#define POTTERY_FORWARD_DECLARATIONS 0

template <typename Accessor, typename Less>
struct Heap {
    #define POTTERY_HEAP_EXTERN static
    #define POTTERY_HEAP_DECLARE_INTERNALS
    #define POTTERY_HEAP_PREFIX heap
    #define POTTERY_HEAP_VALUE_TYPE typename std::iterator_traits<Accessor>::value_type
    #define POTTERY_HEAP_LIFECYCLE_BY_VALUE 1
    #define POTTERY_HEAP_CONTEXT_TYPE Less
    #define POTTERY_HEAP_COMPARE_LESS(less, x, y) less(*x, *y)
    #include "pottery/heap/pottery_heap_static.t.h"
};

#undef POTTERY_FORWARD_DECLARATIONS
#define POTTERY_FORWARD_DECLARATIONS 1

}

/*
 * make_heap
 */

template <class RandomAccessIterator, class Less>
void make_heap(RandomAccessIterator first, RandomAccessIterator last, Less less) {
    size_t count = pottery_cast(size_t, last - first);
    pottery::impl::Heap<RandomAccessIterator, Less>::heap_build(less, first, count);
}

template <class RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::less<value_type> Less;
    pottery::make_heap<RandomAccessIterator, Less>(first, last, Less());
}

/*
 * push_heap
 */

template <class RandomAccessIterator, class Less>
void push_heap(RandomAccessIterator first, RandomAccessIterator last, Less less) {
    size_t count = pottery_cast(size_t, last - first);

    // Invalid arguments are undefined behavior. We'll check anyway and abort()
    // to prevent an overflow on count below.
    if (count == 0)
        pottery_abort();

    pottery::impl::Heap<RandomAccessIterator, Less>::heap_push(less, first, count - 1, 1);
}

template <class RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::less<value_type> Less;
    pottery::push_heap<RandomAccessIterator, Less>(first, last, Less());
}

/*
 * pop_heap
 */

template <class RandomAccessIterator, class Less>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Less less) {
    size_t count = pottery_cast(size_t, last - first);

    // cppreference doesn't say what we do if first and last are equal. We'll
    // take the aggressive route and abort.
    if (count == 0)
        abort();

    pottery::impl::Heap<RandomAccessIterator, Less>::heap_pop(less, first, count, 1);
}

template <class RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::less<value_type> Less;
    pottery::pop_heap<RandomAccessIterator, Less>(first, last, Less());
}

/*
 * sort_heap
 */

template <class RandomAccessIterator, class Less>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Less less) {
    size_t count = pottery_cast(size_t, last - first);

    // cppreference doesn't say what we do if first and last are equal. We'll
    // take the conservative route and abort.
    if (count == 0)
        abort();

    pottery::impl::Heap<RandomAccessIterator, Less>::heap_pop(less, first, count, count);
}

template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::less<value_type> Less;
    pottery::sort_heap<RandomAccessIterator, Less>(first, last, Less());
}

/*
 * is_heap
 */

template <class RandomAccessIterator, class Less>
bool is_heap(RandomAccessIterator first, RandomAccessIterator last, Less less) {
    size_t count = pottery_cast(size_t, last - first);
    return pottery::impl::Heap<RandomAccessIterator, Less>::heap_valid(less, first, count);
}

template <class RandomAccessIterator>
bool is_heap(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::less<value_type> Less;
    return pottery::is_heap<RandomAccessIterator, Less>(first, last, Less());
}

/*
 * is_heap_until
 */

template <class RandomAccessIterator, class Less>
RandomAccessIterator is_heap_until(RandomAccessIterator first, RandomAccessIterator last, Less less) {
    size_t count = pottery_cast(size_t, last - first);
    size_t valid_count = pottery::impl::Heap<RandomAccessIterator, Less>::heap_valid_count(less, first, count);
    return first + valid_count;
}

template <class RandomAccessIterator>
RandomAccessIterator is_heap_until(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::less<value_type> Less;
    return pottery::is_heap_until<RandomAccessIterator, Less>(first, last, Less());
}

}

#undef POTTERY_INTERNAL
#endif
