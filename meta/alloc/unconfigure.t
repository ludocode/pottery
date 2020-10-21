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
// Undefines (forwardable/forwarded) alloc configuration on {PREFIX}
#undef {PREFIX}_CONTEXT_TYPE

#undef {PREFIX}_FREE
#undef {PREFIX}_MALLOC
#undef {PREFIX}_ZALLOC
#undef {PREFIX}_REALLOC
#undef {PREFIX}_MALLOC_EXPAND
#undef {PREFIX}_MALLOC_USABLE_SIZE
#undef {PREFIX}_MALLOC_GOOD_SIZE

#undef {PREFIX}_ALIGNED_FREE
#undef {PREFIX}_ALIGNED_MALLOC
#undef {PREFIX}_ALIGNED_ZALLOC
#undef {PREFIX}_ALIGNED_REALLOC
#undef {PREFIX}_ALIGNED_MALLOC_EXPAND
#undef {PREFIX}_ALIGNED_MALLOC_USABLE_SIZE
#undef {PREFIX}_ALIGNED_MALLOC_GOOD_SIZE
