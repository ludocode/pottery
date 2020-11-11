#include "string.h"

#include <stdio.h>
#include <assert.h>

int main(void) {

    // Initialize a blank string
    string_t homer;
    string_init_blank(&homer);

    // Append a C string
    string_append_cstr(&homer, "Homer");
    printf("%s\n", string_cstr(&homer));

    // Append more data
    string_append_char(&homer, ' ');
    string_append_cstr(&homer, "Simpson");
    assert(string_equal_cstr(&homer, "Homer Simpson"));
    printf("%s\n", string_cstr(&homer));

    // Insert data (total of 17 bytes, causing it to allocate)
    string_insert_cstr(&homer, 5, " J.");
    assert(string_equal_cstr(&homer, "Homer J. Simpson"));
    printf("%s\n", string_cstr(&homer));

    // Replace data
    string_remove(&homer, 6, 2);
    string_insert_cstr(&homer, 6, "Jay");
    assert(string_equal_cstr(&homer, "Homer Jay Simpson"));
    printf("%s\n", string_cstr(&homer));

    // Add lots more data (total of 78 bytes, causing it to grow)
    string_append_cstr(&homer, " works in sector 7G at the Springfield Nuclear Power Plant.");
    printf("%s\n", string_cstr(&homer));

    // Clean it up
    string_destroy(&homer);

    return EXIT_SUCCESS;
}
