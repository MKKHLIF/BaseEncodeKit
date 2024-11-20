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
 *            Must be at least base64_encoded_size(in_sz) bytes
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


/**
 * @brief Decodes a Base64-encoded string into binary data.
 *
 * @param out         Output buffer to store the decoded binary data.
 *                   Must be at least `base64_decoded_size(in_sz)` bytes.
 * @param out_sz      Size of the output buffer.
 * @param in          Input Base64-encoded string.
 * @param in_sz       Size of the input string (excluding null terminator).
 *
 * @return On success, the number of bytes written to the output buffer.
 *         On error:
 *         - `BASE64_ERR_NULL_PTR` if any input or output pointers are `NULL`.
 *         - `BASE64_ERR_SMALL_BUFFER` if the output buffer is too small (returns required size).
 *         - `BASE64_ERR_INVALID_INPUT` if the input contains invalid Base64 characters or padding errors.
 *         - `BASE64_ERR_EMPTY_INPUT` if the input string is empty.
 *
 * @note The input string may contain `=` padding characters at the end. The output is always
 *       truncated to the correct size and returned. If the output buffer is too small, the
 *       required buffer size is returned as a positive value.
 */
ssize_t base64_decode(uint8_t *restrict out,
                      size_t out_sz,
                      const char *restrict in,
                      size_t in_sz);


/**
 * @brief Calculate the required buffer size for Base64 encoding
 * @param input_length Length of the input data in bytes
 * @return Size needed for the Base64 encoded string including null terminator
 * @note Returns 0 if input_length would cause size_t overflow
 */
size_t base64_encoded_size(const size_t input_length);


#endif //BASE64_H
