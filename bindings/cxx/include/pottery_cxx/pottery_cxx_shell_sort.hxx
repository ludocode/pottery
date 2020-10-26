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

#ifndef POTTERY_CXX_SHELL_SORT_HXX
#define POTTERY_CXX_SHELL_SORT_HXX

/*
 * Wraps Pottery's shell_sort in a C++ template.
 *
 * Use pottery::shell_sort() as a mostly drop-in replacement for std::sort()
 * with a few caveats:
 *
 * - The algorithm has greater worst-case complexity than that promised by
 *   std::sort(). (The actual worst case complexity of Ciura's gap sequence is
 *   not clear.)
 *
 * - Throwing move and throwing swap are not supported. If a move constructor
 *   or swap throws, the program aborts. It is strongly recommended that your
 *   move constructor be marked noexcept.
 */

#include "pottery_cxx/pottery_cxx_dependencies.hxx"

namespace pottery {

namespace impl {

#undef POTTERY_FORWARD_DECLARATIONS
#define POTTERY_FORWARD_DECLARATIONS 0

template <typename RandomAccessIterator, typename Less>
struct shell_sort_wrapper {
    #define POTTERY_SHELL_SORT_PREFIX shell_sort
    #define POTTERY_SHELL_SORT_VALUE_TYPE typename std::iterator_traits<RandomAccessIterator>::value_type
    #define POTTERY_SHELL_SORT_REF_TYPE RandomAccessIterator
    #define POTTERY_SHELL_SORT_LIFECYCLE_MOVE_BY_VALUE 1 // call move constructor
    #define POTTERY_SHELL_SORT_LIFECYCLE_SWAP_BY_VALUE 1 // call ADL-swap or std::swap
    #define POTTERY_SHELL_SORT_CONTEXT_TYPE Less
    #define POTTERY_SHELL_SORT_COMPARE_LESS(less, left, right) less(*left, *right)
    #include "pottery/shell_sort/pottery_shell_sort_static.t.h"
};

#undef POTTERY_FORWARD_DECLARATIONS
#define POTTERY_FORWARD_DECLARATIONS 1

} // namespace detail

#define POTTERY_CXX_SORT_NAME shell_sort
#include "pottery_cxx/impl/pottery_cxx_sort.t.h"

} // namespace pottery

#endif
