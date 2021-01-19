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

#include <stdio.h>
#include "employee.h"



/*
 * Let's define a Pottery container to store our employee records. We will
 * build a map of employee numbers to employees.
 *
 * Since our employee type is relatively small and we have a proper set of
 * lifecycle functions, we'll store it by value in the map. This means the map
 * will be responsible for allocating storage for them, and the map will move
 * them around as needed with our move() function. We'll also provide it a
 * destroy() function, so the map will own our employees and will destroy them
 * when it is itself destroyed.
 *
 * Note that our employee type owns the key that the map uses to index it. If
 * we change the number of an employee stored in the map, it will corrupt the
 * map.
 */

// Our map will be called "employees". The type is employees_t and all
// functions are prefixed with "employees_".
#define POTTERY_OPEN_HASH_MAP_PREFIX employees

// The map contains employee_t keyed by int, and the hash is just the int value
// itself. (The map is responsible for giving us a good distribution even if
// employee numbers are sequential.)
#define POTTERY_OPEN_HASH_MAP_VALUE_TYPE employee_t
#define POTTERY_OPEN_HASH_MAP_KEY_TYPE int
#define POTTERY_OPEN_HASH_MAP_REF_KEY(v) v->number
#define POTTERY_OPEN_HASH_MAP_KEY_HASH(x) (size_t)x
#define POTTERY_OPEN_HASH_MAP_KEY_EQUAL(x, y) x == y

// move() is the only lifecycle parameter necessary to give us a valid map.
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_MOVE employee_move

// destroy() is the only lifecycle parameter necessary for the map to properly
// own our employees. With this, we can remove() elements or destroy() the
// whole map and it will clean up our employees for us.
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_DESTROY employee_destroy

// These aren't strictly necessary, but they provide additional map
// functionality. For example with an INIT_COPY expression we'll be able to
// create a copy of the entire map with employees_init_copy(). They can also
// provide some optimizations.
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_INIT employee_init
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_INIT_COPY employee_init_copy
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_INIT_STEAL employee_init_steal
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_ASSIGN employee_set
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_SWAP employee_swap
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_STEAL employee_steal

#include "pottery/open_hash_map/pottery_open_hash_map_static.t.h"



/*
 * The rest of this file is an example of using the above container.
 */

static void print_employee(employee_t* employee) {
    printf("<employee #%i: %s %s, started %s>", employee->number,
            string_cstr(&employee->first_name), employee->last_name,
            employee->start_date);
}

// Adds some employees to the map. This demonstrates a bunch of different ways
// of adding elements. Since our employee type is not bitwise-movable,
// emplace() is the only way to insert a new entry in the map, but there are
// many ways we can get our employee into that entry.
static void add_employees(employees_t* employees) {
    {
        employee_t homer;
        employee_init_values(&homer, "Homer", "Simpson", "1986-01-01", 26);

        // Moves homer into the container
        employee_t* entry = pottery_null;
        employees_emplace_key(employees, employee_number(&homer), &entry, pottery_null);
        employee_move(entry, &homer);

        printf("Added: ");
        print_employee(entry);
        printf("\n");

        // homer is now uninitialized! We don't destroy it.
    }

    {
        // Initializes kyle in-place
        int kyle_number = 31;
        employee_t* kyle = pottery_null;
        employees_emplace_key(employees, kyle_number, &kyle, pottery_null);
        employee_init_values(kyle, "Kyle", "Broslovsky", "1999-01-01", kyle_number);

        printf("Added: ");
        print_employee(kyle);
        printf("\n");
    }

    {
        employee_t bob;
        employee_init_values(&bob, "Bob", "Belcher", "2020-01-01", 17);

        // Inserts a copy of bob
        bool created = false;
        employee_t* entry = pottery_null;
        employees_emplace_key(employees, employee_number(&bob), &entry, &created);
        pottery_assert(created);
        employee_init_copy(entry, &bob);

        printf("Added: ");
        print_employee(entry);
        printf("\n");

        // We still need to destroy our original bob.
        employee_destroy(&bob);
    }

    {
        int stan_number = 42;
        employee_t stan;
        employee_init_values(&stan, "Stan", "Smith", "1990-01-01", stan_number);

        // Inserts stan, stealing its contents
        employee_t* entry = pottery_null;
        employees_emplace_key(employees, employee_number(&stan), &entry, pottery_null);
        employee_init_steal(entry, &stan);

        printf("Added: ");
        print_employee(entry);
        printf("\n");

        // Stan is still initialized, but it contains junk. We still need to
        // destroy it.
        employee_destroy(&stan);
    }
}

// Does some stuff with employees (lookup, removal, replacement, etc.)
static void manipulate_employees(employees_t* employees) {

    // Find Stan
    int stan_number = 42;
    employee_t* employee = employees_find(employees, stan_number);
    if (employees_entry_exists(employees, employee)) {
        printf("Found: ");
        print_employee(employee);
        printf("\n");
    } else {
        abort();
    }

    // Replace with Roger
    // (This is only allowed because they have the same employee number. If we
    // change the employee number without removing and re-inserting the entry,
    // we would corrupt the map.)
    employee_t roger;
    employee_init_values(&roger, "Roger", "Smith", "1999-01-01", stan_number);
    employee_steal(employee, &roger); // Replaces in-place, as a value assignment
    employee_destroy(&roger); // roger is still initialized. We must destroy it.
    printf("Replaced with: ");
    print_employee(employee);
    printf("\n");

    // Remove Roger (destroying it)
    employees_remove(employees, employee);
    printf("Removed.\n");

    // Find and remove Kyle manually
    // (destroy() followed by displace() is equivalent to remove())
    int kyle_number = 31;
    employee = employees_find(employees, kyle_number);
    printf("Found: ");
    print_employee(employee);
    printf("\n");
    employee_destroy(employee);
    employees_displace(employees, employee);
    printf("Removed manually.\n");

    // Check that Kyle no longer exists
    employee = employees_find(employees, kyle_number);
    if (employees_entry_exists(employees, employee))
        abort();
    printf("No employee with number %i exists\n", kyle_number);

    // List the remaining employees
    employee = employees_first(employees);
    for (; employees_entry_exists(employees, employee);
            employee = employees_next(employees, employee)) {
        printf("Remaining employee: ");
        print_employee(employee);
        printf("\n");
    }
}

int main(void) {
    employees_t employees;
    employees_init(&employees);

    add_employees(&employees);
    manipulate_employees(&employees);

    // Destroy the map, cleaning up all remaining employees
    employees_destroy(&employees);
    printf("Done!\n");

    return EXIT_SUCCESS;
}
