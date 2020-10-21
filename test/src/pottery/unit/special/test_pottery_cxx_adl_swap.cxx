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

#include "pottery/pottery_dependencies.h"
#include "pottery/unit/test_pottery_framework.h"

static bool adl_swap_called;

namespace test_pottery {

class AdlSwapTestObject {
public:
    AdlSwapTestObject(int x) :
        m_x(x)
    {}

    AdlSwapTestObject(AdlSwapTestObject&&) {
        // no move! only swap!
        pottery_test_assert(false);
    }

    bool operator<(const AdlSwapTestObject& other) const {
        return this->m_x < other.m_x;
    }

    int operator*() {
        return m_x;
    }

    void swap(AdlSwapTestObject& other) {
        std::swap(this->m_x, other.m_x);
    }

private:
    int m_x;
};

static void swap(AdlSwapTestObject& left, AdlSwapTestObject& right) {
    adl_swap_called = true;
    left.swap(right);
}

} // namespace test_pottery

#define POTTERY_INSERTION_SORT_PREFIX adl_swap_sort
#define POTTERY_INSERTION_SORT_VALUE_TYPE test_pottery::AdlSwapTestObject

// SWAP_BY_VALUE must call swap() found by ADL, which is our
// test_pottery::swap() defined above.
#define POTTERY_INSERTION_SORT_LIFECYCLE_SWAP_BY_VALUE 1

// We set MOVE_BY_VALUE to 1 because we want to make sure that with USE_MOVE 0
// insertion_sort won't use move even if it's available.
#define POTTERY_INSERTION_SORT_LIFECYCLE_MOVE_BY_VALUE 1

// Insertion sort must swap, not move. (This is the default if you specify only
// swap and not move, but we're providing move above. You can comment out this
// line and the previous one to verify that it still works.)
#define POTTERY_INSERTION_SORT_USE_MOVE 0

#define POTTERY_INSERTION_SORT_COMPARE_LESS(x, y) *x < *y
#include "pottery/insertion_sort/pottery_insertion_sort_static.t.h"

/*
 * Tests that we can force insertion sort to swap by value, and it will call
 * our custom swap via argument-dependent lookup.
 */
POTTERY_TEST(pottery_cxx_adl_swap) {
    test_pottery::AdlSwapTestObject values[] = {2, 1};
    size_t count = sizeof(values) / sizeof(*values);

    adl_swap_called = false;
    adl_swap_sort(values, count);

    // make sure the thing actually sorted
    pottery_test_assert(*values[0] == 1);
    pottery_test_assert(*values[1] == 2);

    // make sure our swap was used
    pottery_test_assert(adl_swap_called);
}
