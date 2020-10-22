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

//!!! END_LICENSE

#include "pottery_cxx/pottery_cxx_dependencies.hxx"

namespace pottery {

namespace detail {

#undef POTTERY_FORWARD_DECLARATIONS
#define POTTERY_FORWARD_DECLARATIONS 0

template <typename RandomAccessIterator, typename Less>
struct {lower_name}_wrapper {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;

    #define POTTERY_{UPPER_NAME}_PREFIX {lower_name}
    #define POTTERY_{UPPER_NAME}_VALUE_TYPE value_type
    #define POTTERY_{UPPER_NAME}_REF_TYPE RandomAccessIterator
    #define POTTERY_{UPPER_NAME}_LIFECYCLE_MOVE_BY_VALUE 1
    #define POTTERY_{UPPER_NAME}_CONTEXT_TYPE Less
    #define POTTERY_{UPPER_NAME}_COMPARE_LESS(less, left, right) less(*left, *right)
    #include "pottery/{lower_name}/pottery_{lower_name}_static.t.h"
};

#undef POTTERY_FORWARD_DECLARATIONS
#define POTTERY_FORWARD_DECLARATIONS 1

} // namespace detail

template <typename RandomAccessIterator>
inline void {lower_name}(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::less<value_type> Less;
    size_t count = pottery_cast(size_t, last - first);
    detail::{lower_name}_wrapper<RandomAccessIterator, Less>::{lower_name}(Less(), first, count);
}

template <typename RandomAccessIterator, typename Less>
inline void {lower_name}(RandomAccessIterator first, RandomAccessIterator last, Less less) {
    size_t count = pottery_cast(size_t, last - first);
    detail::{lower_name}_wrapper<RandomAccessIterator, Less>::{lower_name}(less, first, count);
}

} // namespace pottery
