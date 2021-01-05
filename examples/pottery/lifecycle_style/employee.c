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

#include "employee.h"


/*
 * Fundamental Lifecycle Functions
 */

bool employee_init(employee_t* employee) {
    string_init(&employee->first_name);

    // Our requirement that last_name is never null greatly simplifies the
    // code, but it is obviously not efficient in cases like this. We could
    // allow null as an optimization for blank, but this would complicate the
    // code. The string_t container of course handles all of this for us.
    employee->last_name = strdup("");

    employee->start_date[0] = '\0';
    employee->number = -1;

    return true;
}

bool employee_init_copy(employee_t* to, const employee_t* from) {
    // Copy initialization is super straightforward: we just copy each field.
    string_init_copy(&to->first_name, &from->first_name);
    to->last_name = strdup(from->last_name);
    strcpy(to->start_date, from->start_date);
    to->number = from->number;

    return true;
}

bool employee_init_steal(employee_t* to, employee_t* from) {

    // The pottery string implements this, so we just forward it along.
    string_init_steal(&to->first_name, &from->first_name);

    // We steal the from employee's last name, but we must leave the from
    // employee initialized so we give it a new blank string.
    to->last_name = from->last_name;
    from->last_name = strdup("");

    // Other fields are simply copied.
    strcpy(to->start_date, from->start_date);
    to->number = from->number;

    return true;
}

void employee_destroy(employee_t* employee) {
    string_destroy(&employee->first_name);
    free(employee->last_name);
}

void employee_move(employee_t* to, employee_t* from) {

    // Our Pottery string is not bitwise-movable. We must explicitly move it.
    string_move(&to->first_name, &from->first_name);

    // Other fields are bitwise movable so we just copy them.
    to->last_name = from->last_name;
    strcpy(to->start_date, from->start_date);
    to->number = from->number;

    // Note that we don't need to clear from->last_name even though it's an
    // allocated pointer! We're leaving the from employee uninitialized in this
    // function. (We aren't technically destroying it either; we're moving the
    // storage.) Since the from object is not going to be destroyed, it can
    // contain garbage, even pointers owned by other objects.
}

void employee_steal(employee_t* to, employee_t* from) {
    string_steal(&to->first_name, &from->first_name);

    #if 0
    // We steal the from employee's last name, but we must leave the from
    // employee initialized so it needs a new blank string. We also need to
    // free our own string first. To optimize this we only do it if the strings
    // differ.
    if (0 != strcmp(to->last_name, from->last_name)) {
        free(to->last_name);
        to->last_name = from->last_name;
        from->last_name = strdup("");
    }
    #else

    // An alternative implementation is to simply swap the last name pointers,
    // thus avoiding the extra allocation. We don't have to leave the from
    // object blank or with its original data; it can contain anything. It just
    // needs to still be initialized. (This has another advantage which is that
    // it can't fail due to an allocation failure. Pottery does not support
    // failures from steal() functions.)
    char* last_name = to->last_name;
    to->last_name = from->last_name;
    from->last_name = last_name;
    #endif

    // Other fields are bitwise copyable so we just copy them. Note that we
    // aren't clearing those of the from object because it doesn't need to be
    // blank, just initialized.
    strcpy(to->start_date, from->start_date);
    to->number = from->number;
}

bool employee_copy(employee_t* to, const employee_t* from) {
    string_copy(&to->first_name, &from->first_name);

    // We need to replace our string with a copy of the from employee's string,
    // so to optimize this we only do it if the strings differ.
    if (0 != strcmp(to->last_name, from->last_name)) {
        free(to->last_name);
        to->last_name = strdup(from->last_name);
    }

    // Other fields are bitwise copyable so we just copy them.
    strcpy(to->start_date, from->start_date);
    to->number = from->number;

    return true;
}

void employee_swap(employee_t* to, employee_t* from) {
    #if 0
    // Our string_t is not bitwise movable so it's not necessarily safe to just
    // swap the whole employee struct. Instead we swap each individual field.
    string_swap(&to->first_name, &from->first_name);

    char* last_name = to->last_name;
    to->last_name = from->last_name;
    from->last_name = last_name;

    char start_date[sizeof(to->start_date)];
    strcpy(start_date, to->start_date);
    strcpy(to->start_date, from->start_date);
    strcpy(from->start_date, start_date);

    int number = to->number;
    to->number = from->number;
    from->number = number;
    #else

    // Here's an alternative implementation: we declare an uninitialized
    // employee struct on the stack and use a sequence of moves through it.
    employee_t temp;
    employee_move(&temp, to);
    employee_move(to, from);
    employee_move(from, &temp);
    // Note that this is better than the C++-style std::swap() because we did
    // not have to initialize the temporary. This would have incurred an
    // unnecessary strdup("") and free(), as well as various unnecessary
    // assignments.
    #endif
}



/*
 * Additional Lifecycle Functions
 */

bool employee_init_values(employee_t* employee, const char* first_name,
        const char* last_name, const char* start_date, int number)
{
    string_init_cstr(&employee->first_name, first_name);
    employee->last_name = strdup(last_name);

    // Since our start date is in a fixed-size buffer, we have to manually
    // length-check it. We consider it an API usage error to pass a date string
    // that doesn't fit, and if the API is used incorrectly we just abort().
    if (strlen(start_date) > sizeof(employee->start_date) - 1)
        abort();
    strcpy(employee->start_date, start_date);

    employee->number = number;

    return true;
}
