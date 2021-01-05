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

#ifndef TEST_POTTERY_UFO_HXX
#define TEST_POTTERY_UFO_HXX 1

#include "test_pottery_ufo.h"

/**
 * An unidentified flying object.
 *
 * This is the C++ equivalent of ufo_t; it wraps ufo_t to implement everything.
 *
 * The UFO performs allocations, detects memory leaks, and detects improper
 * (bitwise) moves. Call check() to make sure the UFO is valid.
 *
 * The UFO has a non-explicit C string constructor so it can be initialized as
 * a string and compared against strings. This makes it convenient for writing
 * unit tests.
 */
class UFO {
public:
    ~UFO() {
        check();
        ufo_destroy(&m_ufo);
    }

    UFO(const UFO& other) {
        other.check();
        ufo_init_copy(&m_ufo, &other.m_ufo);
    }

    UFO(UFO&& other) {
        other.check();
        ufo_init_steal(&m_ufo, &other.m_ufo);
    }

    UFO& operator=(const UFO& other) {
        check();
        other.check();
        ufo_copy(&m_ufo, &other.m_ufo);
        return *this;
    }

    UFO& operator=(UFO&& other) {
        check();
        other.check();
        ufo_steal(&m_ufo, &other.m_ufo);
        return *this;
    }

    UFO() {
        check();
        ufo_init(&m_ufo, "", 0);
    }

    // Not explicit. We want to be able to initialize this with literal
    // strings, and we want operator== below with a literal to create a
    // temporary
    UFO(const char* m_str) {
        ufo_init(&m_ufo, m_str, 0);
    }

    bool operator<(const UFO& other) const {
        return ufo_compare(&m_ufo, &other.m_ufo) < 0;
    }

    bool operator==(const UFO& other) const {
        return ufo_compare(&m_ufo, &other.m_ufo) == 0;
    }

private:
    void check() const {
        ufo_check(&m_ufo);
    }

    ufo_t m_ufo;
};

#endif
