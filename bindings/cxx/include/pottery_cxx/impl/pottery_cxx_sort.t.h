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

/*
 * Implements wrappers for a Pottery C++ sort template.
 *
 * This mostly just avoids copy-pasting this into each C++ sort header.
 */

#define POTTERY_CXX_SORT_WRAPPER POTTERY_CONCAT(POTTERY_CXX_SORT_NAME, _wrapper)

template <typename RandomAccessIterator>
inline void POTTERY_CXX_SORT_NAME(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::less<value_type> Less;
    size_t count = pottery_cast(size_t, last - first);
    impl::POTTERY_CXX_SORT_WRAPPER<RandomAccessIterator, Less>::POTTERY_CXX_SORT_NAME(Less(), first, count);
}

template <typename RandomAccessIterator, typename Less>
inline void POTTERY_CXX_SORT_NAME(RandomAccessIterator first, RandomAccessIterator last, Less less) {
    size_t count = pottery_cast(size_t, last - first);
    impl::POTTERY_CXX_SORT_WRAPPER<RandomAccessIterator, Less>::POTTERY_CXX_SORT_NAME(less, first, count);
}

#undef POTTERY_CXX_SORT_WRAPPER
#undef POTTERY_CXX_SORT_NAME
