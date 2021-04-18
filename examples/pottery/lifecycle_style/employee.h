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

#ifndef POTTERY_EXAMPLES_EMPLOYEE_EMPLOYEE_H
#define POTTERY_EXAMPLES_EMPLOYEE_EMPLOYEE_H 1

#include <string.h>

// Pottery's example string
#include "pottery/string/string.h"

#ifdef __cplusplus
extern "C" {
#endif



/**
 * An employee record.
 *
 * To demonstrate different string storage types and how they affect lifecycle
 * functions, we store each string with a different string type. We also store
 * the employee number as an int to demonstrate a trivial field.
 *
 * Note in particular that string_t is not bitwise movable (it contains a
 * self-pointer to internal storage for short strings), so the whole employee
 * record is not bitwise movable. We need to use employee_move() to move it
 * around and we need to give that move function to employee containers.
 */
typedef struct employee_t {
    string_t first_name;  // Pottery's example string container
    char* last_name;      // An allocated C string, never null
    char start_date[32];  // A fixed-size string buffer
    int number;           // A trivial value type
} employee_t;



/*
 * Pottery-style lifecycle functions (a.k.a. rule of eight): the eight
 * fundamental lifecycle operations for a C object type.
 */

/**
 * Initializes an employee.
 *
 * Returns true if successful, false on failure. If initialization fails, the
 * employee is not initialized and must not be destroyed.
 */
bool employee_init(employee_t* employee);

/*
 * Initializes an employee by copying the contents of another into it.
 *
 * The to employee must be uninitialized.
 *
 * Returns true if successful, false on failure. If initialization fails, the
 * employee is not initialized and must not be destroyed.
 */
bool employee_init_copy(employee_t* to, const employee_t* from);

/*
 * Initializes an employee by stealing the contents of another into it.
 *
 * The to employee must be uninitialized. The from employee is left initialized
 * but containing junk; it must still be destroyed.
 *
 * Returns true if successful, false on failure. If initialization fails, the
 * employee is not initialized and must not be destroyed.
 */
bool employee_init_steal(employee_t* to, employee_t* from);

/**
 * Destroys an employee.
 */
void employee_destroy(employee_t* employee);

/**
 * Moves an employee to a different storage location.
 *
 * The to employee must be uninitialized. The from employee will be left
 * uninitialized.
 */
void employee_move(employee_t* to, employee_t* from);

/**
 * Copies the contents of another employee into this one.
 *
 * The to employee must already be initialized; its contents will be discarded.
 *
 * Returns true if successful, false on failure. If the operation fails, the to
 * employee remains initialized but contains junk.
 */
bool employee_copy(employee_t* to, const employee_t* from);

/*
 * Steals the contents of another employee into this one.
 *
 * The to employee must already be initialized; its contents will be discarded.
 * The from employee is left initialized but containing junk; it must still be
 * destroyed.
 */
void employee_steal(employee_t* to, employee_t* from);

/**
 * Swaps two employees.
 */
void employee_swap(employee_t* to, employee_t* from);



/*
 * Additional initialization functions.
 *
 * These must have "init" or "new" in the name to make it easily auditable that
 * they must be paired with a "destroy" (or "emplace").
 */

/**
 * Initializes an employee with the given values.
 */
bool employee_init_values(employee_t* employee, const char* first_name,
        const char* last_name, const char* start_date, int number);



/*
 * Accessors and Mutators
 *
 * These aren't part of the lifecycle functions, but they serve as a
 * demonstration of why wrapping an object in lifecycle functions is useful.
 *
 * Our employee properly encapsulates not just its data, but the implementation
 * of its storage for this data: all of the different contained string types
 * are exposed simply as `const char*`.
 */

/**
 * Returns the employee's first name.
 */
static inline const char* employee_first_name(const employee_t* employee) {
    return string_cstr(&employee->first_name);
}

/**
 * Returns the employee's last name.
 */
static inline const char* employee_last_name(const employee_t* employee) {
    return employee->last_name;
}

/**
 * Returns the employee's start date.
 */
static inline const char* employee_start_date(const employee_t* employee) {
    return employee->start_date;
}

/**
 * Returns the employee's number.
 */
static inline int employee_number(const employee_t* employee) {
    return employee->number;
}

/**
 * Sets the employee's first name.
 */
static inline void employee_set_first_name(employee_t* employee, const char* first_name) {
    string_set_cstr(&employee->first_name, first_name);
}

/**
 * Sets the employee's last name.
 */
static inline void employee_set_last_name(employee_t* employee, const char* last_name) {
    free(employee->last_name);
    employee->last_name = strdup(last_name);
}

/**
 * Sets the employee's start date.
 */
static inline void employee_set_start_date(employee_t* employee, const char* start_date) {

    // Since our start date is in a fixed-size buffer, we have to manually
    // length-check it. We consider it an API usage error to pass a date string
    // that doesn't fit, and if the API is used incorrectly we just abort().
    if (strlen(start_date) > sizeof(employee->start_date) - 1)
        abort();

    strcpy(employee->start_date, start_date);
}

/**
 * Sets the employee's number.
 *
 * @warning If the employee is in a map where the key is the employee number,
 *          changing this number will corrupt the map! You must remove the
 *          employee from any maps before changing this number.
 */
static inline void employee_set_number(employee_t* employee, int number) {
    employee->number = number;
}



#ifdef __cplusplus
}
#endif

#endif
