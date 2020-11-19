#define POTTERY_HSEARCH_REAL_API
#include "pottery_hsearch.h"

#include <errno.h>
#include <assert.h>

int main(void) {
    assert(0 != hcreate(32));

    // Unfortunately we have to cast away const to put literal string keys in
    // an hsearch table. They won't actually be modified or freed (as long as
    // we don't call hdestroy1(free, ..) or hdestroy_openbsd().)
    ENTRY alice = {pottery_const_cast(char*, "alice"), NULL};
    ENTRY bob   = {pottery_const_cast(char*, "bob"), NULL};
    ENTRY eve   = {pottery_const_cast(char*, "eve"), NULL};

    assert(NULL == hsearch(alice, FIND));
    assert(NULL == hsearch(bob, FIND));
    assert(NULL == hsearch(eve, FIND));

    assert(NULL != hsearch(alice, ENTER));
    assert(NULL != hsearch(bob, ENTER));

    assert(0 == strcmp("alice", hsearch(alice, FIND)->key));
    assert(0 == strcmp("bob",   hsearch(bob,   FIND)->key));
    assert(NULL == hsearch(eve, FIND));

    hdestroy();
    return EXIT_SUCCESS;
}
