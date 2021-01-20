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

//!!! END_LICENSE
// Defines wrappers for common function arguments for functions that access
// arrays via the array_access template.

// ARGS are function arguments
// VALS are the matching values
// SOLE means they are the only arguments to the function (so no trailing comma)
// UNUSED casts them to void to silence unused parameter warnings

#if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
    #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
        #define {CONFIG}_SOLE_ARGS \
                {NAME}_context_t context, {NAME}_entry_t base
        #define {CONFIG}_SOLE_VALS context, base
        #define {CONFIG}_ARGS_UNUSED (void)context, (void)base
    #else
        #define {CONFIG}_SOLE_ARGS {NAME}_context_t context
        #define {CONFIG}_SOLE_VALS context
        #define {CONFIG}_ARGS_UNUSED (void)context
    #endif
#else
    #if !POTTERY_ARRAY_ACCESS_INHERENT_BASE
        #define {CONFIG}_SOLE_ARGS {NAME}_entry_t base
        #define {CONFIG}_SOLE_VALS base
        #define {CONFIG}_ARGS_UNUSED (void)base
    #else
        #define {CONFIG}_SOLE_ARGS void
        #define {CONFIG}_SOLE_VALS /*nothing*/
        #define {CONFIG}_ARGS_UNUSED /*nothing*/
    #endif
#endif
#if POTTERY_CONTAINER_TYPES_HAS_CONTEXT || !POTTERY_ARRAY_ACCESS_INHERENT_BASE
    #define {CONFIG}_ARGS {CONFIG}_SOLE_ARGS ,
    #define {CONFIG}_VALS {CONFIG}_SOLE_VALS ,
#else
    #define {CONFIG}_ARGS /*nothing*/
    #define {CONFIG}_VALS /*nothing*/
#endif

#undef POTTERY_ARRAY_ACCESS_ARGS_HACK_HAS_CONTEXT
