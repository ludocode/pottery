#include <stdio.h>

#define POTTERY_INTRO_SORT_PREFIX sort_strings
#define POTTERY_INTRO_SORT_VALUE_TYPE const char*
#define POTTERY_INTRO_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY(x, y) strcmp(*x, *y)
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"

int main(void) {
    const char* players[] = {
        "fred", "quincy", "alice", "eve", "zack", "ned", "paul", "bob", "gary",
        "ursula", "yves", "carl", "olivia", "steve", "rob", "mike", "wade", "dave",
        "jake", "helen", "xavier", "karen", "tammy", "laura", "isaac", "vick",
    };
    size_t count = sizeof(players) / sizeof(*players);

    sort_strings(players, count);

    for (size_t i = 0; i < count; ++i)
        puts(players[i]);

    return EXIT_SUCCESS;
}
