#include <string.h>
#include <stdio.h>
#include "pottery/pottery_dependencies.h"

// A junk test file to compare the code size of various algorithms.
// Compile with e.g.
//     gcc -Ofast -fno-exceptions -Iinclude -Wall -DPOTTERY_TEST_CODE_SIZE test/src/pottery/test_code_size.cxx -c -o out.o
// Check sizes with e.g.
//     nm --print-size --size-sort --radix=d out.o
//     size -A -d out.o

#if 1
#define POTTERY_SHELL_SORT_PREFIX sort_strings
#define POTTERY_SHELL_SORT_VALUE_TYPE const char*
#define POTTERY_SHELL_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_SHELL_SORT_COMPARE_THREE_WAY(x, y) strcmp(*x, *y)
#include "pottery/shell_sort/pottery_shell_sort_static.t.h"
#elif 0
#define POTTERY_INSERTION_SORT_PREFIX sort_strings
#define POTTERY_INSERTION_SORT_VALUE_TYPE const char*
#define POTTERY_INSERTION_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_INSERTION_SORT_COMPARE_THREE_WAY(x, y) strcmp(*x, *y)
#include "pottery/insertion_sort/pottery_insertion_sort_static.t.h"
#else
#include <algorithm>
pottery_maybe_unused
static void sort_strings(const char** strings, size_t count) {
    std::sort(strings, strings + count, strcmp);
}
#endif

#ifdef POTTERY_TEST_CODE_SIZE
int main(int argc, const char* argv[]) {
    // sort command-line arguments
    ++argv;
    --argc;
    sort_strings(argv, argc);
    for (int i = 0; i < argc; ++i)
        puts(argv[i]);
    return EXIT_SUCCESS;
}
#endif
