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

#ifndef POTTERY_LIFECYCLE_CAN_DESTROY
#error "The Lifecycle template was either not instantiated or was already cleaned up."
#endif

#undef POTTERY_LIFECYCLE_CLEANUP
#undef POTTERY_LIFECYCLE_CAN_DESTROY
#undef POTTERY_LIFECYCLE_CAN_INIT
#undef POTTERY_LIFECYCLE_CAN_INIT_COPY
#undef POTTERY_LIFECYCLE_CAN_INIT_STEAL
#undef POTTERY_LIFECYCLE_CAN_CONSTRUCT
#undef POTTERY_LIFECYCLE_CAN_MOVE
#undef POTTERY_LIFECYCLE_CAN_COPY
#undef POTTERY_LIFECYCLE_CAN_STEAL
#undef POTTERY_LIFECYCLE_CAN_SWAP
#undef POTTERY_LIFECYCLE_CAN_PASS
#undef POTTERY_LIFECYCLE_EXTERNAL_CONTAINER_TYPES
