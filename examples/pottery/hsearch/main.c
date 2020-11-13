#define POTTERY_HSEARCH_REAL_API
#include "pottery_hsearch.h"

#include <errno.h>
#include <assert.h>

int main(void) {
    assert(0 != hcreate(32));

    ENTRY alice = {"alice", NULL};
    ENTRY bob   = {"bob", NULL};
    ENTRY eve   = {"eve", NULL};

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
