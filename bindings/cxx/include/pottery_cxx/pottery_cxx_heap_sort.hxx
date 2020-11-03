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

#ifndef POTTERY_CXX_HEAP_SORT_HXX
#define POTTERY_CXX_HEAP_SORT_HXX

/*
 * Wraps Pottery's heap_sort in a C++ template.
 *
 * Use pottery::heap_sort() as a mostly drop-in replacement for std::sort()
 * with a few caveats:
 *
 * - Heap sort has safe worst case behaviour and relatively small code size but
 *   is generally much slower than quick sort or intro sort on large arrays due
 *   to poor cache locality. pottery::intro_sort() or your platform's
 *   std::sort() is likely faster with the same worst case time complexity.
 *
 * - Throwing move and throwing swap are not supported. If a move constructor
 *   or swap throws, the program aborts. It is strongly recommended that your
 *   move constructor be marked noexcept.
 *
 * This may be useful for small to medium size arrays where you want consistent
 * performance and guaranteed time bounds.
 */

#include "pottery_cxx/pottery_cxx_dependencies.hxx"

namespace pottery {

namespace impl {

#undef POTTERY_FORWARD_DECLARATIONS
#define POTTERY_FORWARD_DECLARATIONS 0

template <typename RandomAccessIterator, typename Less>
struct heap_sort_wrapper {
    #define POTTERY_HEAP_SORT_PREFIX heap_sort
    #define POTTERY_HEAP_SORT_VALUE_TYPE typename std::iterator_traits<RandomAccessIterator>::value_type
    #define POTTERY_HEAP_SORT_REF_TYPE RandomAccessIterator
    #define POTTERY_HEAP_SORT_LIFECYCLE_MOVE_BY_VALUE 1 // call move constructor
    #define POTTERY_HEAP_SORT_LIFECYCLE_SWAP_BY_VALUE 1 // call ADL-swap or std::swap
    #define POTTERY_HEAP_SORT_CONTEXT_TYPE Less
    #define POTTERY_HEAP_SORT_COMPARE_LESS(less, left, right) less(*left, *right)
    #include "pottery/heap_sort/pottery_heap_sort_static.t.h"
};

#undef POTTERY_FORWARD_DECLARATIONS
#define POTTERY_FORWARD_DECLARATIONS 1

} // namespace detail

#define POTTERY_CXX_SORT_NAME heap_sort
#include "pottery_cxx/impl/pottery_cxx_sort.t.h"

} // namespace pottery

#endif
