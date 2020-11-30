/*
 * MIT License
 *
 * Copyright (c) 2020 Nicholas Fraser
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
// Forwards common sort configuration from {SRC} to {DEST}

#if (defined({SRC}_REF_TYPE) && defined({SRC}_VALUE_TYPE)) || \
        !defined({SRC}_REF_TYPE) && !defined({SRC}_VALUE_TYPE)
    #error "Exactly one of {SRC}_REF_TYPE and {SRC}_VALUE_TYPE must be defined."
#endif

#ifdef {SRC}_REF_TYPE
    #define {DEST}_REF_TYPE {SRC}_REF_TYPE
#endif
#ifdef {SRC}_VALUE_TYPE
    #define {DEST}_VALUE_TYPE {SRC}_VALUE_TYPE
#endif
#ifdef {SRC}_CONTEXT_TYPE
    #define {DEST}_CONTEXT_TYPE {SRC}_CONTEXT_TYPE
#endif
// TODO ACCESS is deprecated. This whole metatemplate can be removed
#ifdef {SRC}_ACCESS
    #define {DEST}_ACCESS {SRC}_ACCESS
#endif
