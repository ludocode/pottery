#include <stdio.h>

#define POTTERY_VECTOR_PREFIX int_vector
#define POTTERY_VECTOR_ELEMENT_TYPE int
#define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1
#include "pottery/vector/pottery_vector_static.t.h"

int main(void) {
    int_vector_t vector;
    int_vector_init(&vector);

    int_vector_insert_first(&vector, 10);
    int_vector_insert_last(&vector, 20);
    int_vector_insert_at(&vector, 1, 15);
    int_vector_remove_at(&vector, 0);

    for (size_t i = 0; i < int_vector_count(&vector); ++i)
        printf("%i\n", *int_vector_at(&vector, i));

    int_vector_destroy(&vector);
    return EXIT_SUCCESS;
}
