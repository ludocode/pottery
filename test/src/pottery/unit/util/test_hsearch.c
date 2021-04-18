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

#define POTTERY_HSEARCH_REAL_API
#include "pottery/hsearch/pottery_hsearch.h"

#include "pottery/unit/test_pottery_framework.h"

POTTERY_TEST(pottery_hsearch) {
    pottery_test_assert(0 != hcreate(32));

    // Unfortunately we have to cast away const to put literal string keys in
    // an hsearch table. They won't actually be modified or freed (as long as
    // we don't call hdestroy1(free, ..) or hdestroy_openbsd().)
    ENTRY alice = {pottery_const_cast(char*, "alice"), pottery_null};
    ENTRY bob   = {pottery_const_cast(char*, "bob"), pottery_null};
    ENTRY eve   = {pottery_const_cast(char*, "eve"), pottery_null};

    pottery_test_assert(pottery_null == hsearch(alice, FIND));
    pottery_test_assert(pottery_null == hsearch(bob, FIND));
    pottery_test_assert(pottery_null == hsearch(eve, FIND));

    pottery_test_assert(pottery_null != hsearch(alice, ENTER));
    pottery_test_assert(pottery_null != hsearch(bob, ENTER));

    pottery_test_assert(0 == strcmp("alice", hsearch(alice, FIND)->key));
    pottery_test_assert(0 == strcmp("bob",   hsearch(bob,   FIND)->key));
    pottery_test_assert(pottery_null == hsearch(eve, FIND));

    hdestroy();
}
