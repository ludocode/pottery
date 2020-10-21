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

#include "pottery_cxx/pottery_cxx_vector.hxx"

#include "pottery/unit/test_pottery_framework.h"
#include "pottery/common/test_pottery_ufo.hxx"

POTTERY_TEST(pottery_cxx_vector_ints) {
    pottery::vector<int> vector;

    vector.push_back(0);
    vector.push_back(1);
    vector.push_back(3);
    vector.push_back(4);

    vector.insert(vector.begin() + 2, 2);

    for (size_t i = 0; i < vector.size(); ++i)
        pottery_test_assert(vector[i] == (int)i);

    vector.erase(vector.begin());
    vector.erase(vector.begin() + 1);
    vector.erase(vector.begin() + 2);

    for (size_t i = 0; i < vector.size(); ++i)
        pottery_test_assert(vector[i] == ((int)i * 2) + 1);
}

POTTERY_TEST(pottery_cxx_vector_moveonlystring) {
    pottery::vector<UFO> vector;

    vector.emplace_back("alice");
    vector.emplace_back("eve");
    vector.emplace_back("dave");

    pottery_test_assert(vector[0] == "alice");
    pottery_test_assert(vector[1] == "eve");
    pottery_test_assert(vector[2] == "dave");

    vector.emplace(vector.begin() + 2, "carl");
    vector.emplace(vector.begin() + 1, "bob");

    pottery_test_assert(vector[0] == "alice");
    pottery_test_assert(vector[1] == "bob");
    pottery_test_assert(vector[2] == "eve");
    pottery_test_assert(vector[3] == "carl");
    pottery_test_assert(vector[4] == "dave");

    vector.erase(vector.end() - 3);

    pottery_test_assert(vector[0] == "alice");
    pottery_test_assert(vector[1] == "bob");
    pottery_test_assert(vector[2] == "carl");
    pottery_test_assert(vector[3] == "dave");
}

POTTERY_TEST(pottery_cxx_vector_copy) {
    pottery::vector<UFO> vector;

    vector.emplace_back("hello");
    vector.emplace_back("world");

    // copy whole vector
    pottery::vector<UFO> vector2(vector);

    pottery_test_assert(vector[0] == "hello");
    pottery_test_assert(vector[1] == "world");
    pottery_test_assert(vector2[0] == "hello");
    pottery_test_assert(vector2[1] == "world");
}

POTTERY_TEST(pottery_cxx_vector_move) {
    pottery::vector<UFO> vector;

    vector.emplace_back("hello");
    vector.emplace_back("world");

    // move vector
    pottery::vector<UFO> vector2(std::move(vector));

    pottery_test_assert(vector2[0] == "hello");
    pottery_test_assert(vector2[1] == "world");
}

POTTERY_TEST(pottery_cxx_vector_const) {
    pottery::vector<UFO> vector;
    vector.emplace_back("hello");
    vector.emplace_back("world");

    const pottery::vector<UFO>& cv(vector);

    // test operator[] on const reference
    pottery_test_assert(cv[0] == "hello");
    pottery_test_assert(cv[1] == "world");
}
