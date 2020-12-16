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

// Undefines (forwardable/forwarded) lifecycle configuration on {PREFIX}

#undef {PREFIX}_VALUE_TYPE
#undef {PREFIX}_REF_TYPE
#undef {PREFIX}_KEY_TYPE

#undef {PREFIX}_KEY_HASH
#undef {PREFIX}_KEY_EQUAL
#undef {PREFIX}_REF_KEY

#undef {PREFIX}_IS_EMPTY
#undef {PREFIX}_SET_EMPTY
#undef {PREFIX}_EMPTY_IS_ZERO

#undef {PREFIX}_IS_TOMBSTONE
#undef {PREFIX}_SET_TOMBSTONE

#undef {PREFIX}_IS_VALUE

#undef {PREFIX}_LINEAR_PROBING
#undef {PREFIX}_QUADRATIC_PROBING
#undef {PREFIX}_DOUBLE_HASHING
#undef {PREFIX}_DOUBLE_HASH_PROBE
