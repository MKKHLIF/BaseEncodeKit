// MIT License
//
// Copyright (c) 2024 MKKHLIF
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// File: base64.h

#ifndef BASE64_H
#define BASE64_H
#include <stdint.h>

/**
 * @brief Encodes binary data using Base64 encoding scheme
 *
 * @param out Output buffer to store the encoded string
 *            Must be at least base64_encode_len(in_sz) bytes
 * @param out_sz Size of the output buffer
 * @param in Input binary data to encode
 *           Using uint8_t makes the binary nature explicit
 * @param in_sz Size of input data in bytes
 *              Limited to SIZE_MAX/4 - 1 to prevent overflow
 *
 * @return Number of bytes written (excluding null terminator) on success
 *         -1 on NULL pointers or insufficient buffer size
 *         -2 on input size too large
 *
 * @note Output is always null-terminated if out_sz > 0
 */
ssize_t base64_encode(char *restrict out,
                      size_t out_sz,
                      const uint8_t *restrict in,
                      size_t in_sz);

#endif //BASE64_H
