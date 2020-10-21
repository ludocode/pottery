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

#include <algorithm>

// std::sort

extern "C"
void std_sort_wrapper(int* ints, size_t count) {
    std::sort(ints, ints + count);
}


#if __has_include(<boost/sort/sort.hpp>)
#include <boost/sort/sort.hpp>

extern "C"
void boost_pdqsort_wrapper(int* ints, size_t count) {
    boost::sort::pdqsort(ints, ints + count);
}

extern "C"
void boost_spinsort_wrapper(int* ints, size_t count) {
    boost::sort::spinsort(ints, ints + count);
}

extern "C"
void boost_flat_stable_sort_wrapper(int* ints, size_t count) {
    boost::sort::flat_stable_sort(ints, ints + count);
}

#endif
