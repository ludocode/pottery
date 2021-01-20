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
// Forwards container_types configuration from {SRC} to {DEST}

// Forward the types
#if defined({SRC}_VALUE_TYPE)
    #define {DEST}_VALUE_TYPE {SRC}_VALUE_TYPE
#endif
#if defined({SRC}_REF_TYPE)
    #define {DEST}_REF_TYPE {SRC}_REF_TYPE
#endif
#if defined({SRC}_KEY_TYPE)
    #define {DEST}_KEY_TYPE {SRC}_KEY_TYPE
#endif
#ifdef {SRC}_CONTEXT_TYPE
    #define {DEST}_CONTEXT_TYPE {SRC}_CONTEXT_TYPE
#endif

// Forward the conversion and comparison expressions
#if defined({SRC}_REF_KEY)
    #define {DEST}_REF_KEY {SRC}_REF_KEY
#endif
#if defined({SRC}_REF_EQUAL)
    #define {DEST}_REF_EQUAL {SRC}_REF_EQUAL
#endif
