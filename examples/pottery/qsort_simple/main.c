#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pottery_simple_qsort.h"

static inline int strcmp_wrapper(const void* left, const void* right) {
    return strcmp(*(const char* const*)left, *(const char* const*)right);
}

int main(void) {
    const char* players[] = {
        "fred", "quincy", "alice", "eve", "zack", "ned", "paul", "bob", "gary",
        "ursula", "yves", "carl", "olivia", "steve", "rob", "mike", "wade", "dave",
        "jake", "helen", "xavier", "karen", "tammy", "laura", "isaac", "vick",
    };
    size_t count = sizeof(players) / sizeof(*players);

    pottery_simple_qsort((void*)players, count, sizeof(players[0]), strcmp_wrapper);

    size_t i;
    for (i = 0; i < count; ++i)
        puts(players[i]);

    return EXIT_SUCCESS;
}
