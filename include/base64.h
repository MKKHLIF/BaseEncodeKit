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
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Error codes for base64 operations
 */
typedef enum {
 BASE64_SUCCESS = 0,
 BASE64_ERROR_INVALID_INPUT, // Input contains invalid characters
 BASE64_ERROR_INVALID_LENGTH, // Input length is invalid
 BASE64_ERROR_BUFFER_TOO_SMALL, // Output buffer is too small
 BASE64_ERROR_NULL_POINTER, // NULL pointer provided
 BASE64_ERROR_PADDING, // Invalid padding
 BASE64_ERROR_MEMORY // Memory allocation failure
} base64_error_t;

/**
 * @brief Configuration options for base64 operations
 */
typedef struct {
 int use_padding; // Whether to use padding characters (=)
 int url_safe; // Use URL-safe alphabet (-_ instead of +/)
 int line_length; // Length of lines (0 for no line breaks)
 char line_ending[3]; // Line ending sequence (e.g., "\r\n")
} base64_config_t;

/**
 * @brief Base64 context structure
 */
typedef struct base64_ctx_t base64_ctx_t;

/**
 * @brief Initialize a base64 context with the given configuration
 *
 * @param ctx Pointer to context pointer to be initialized
 * @param config Configuration options (NULL for defaults)
 * @return base64_error_t Error code
 */
base64_error_t base64_init(base64_ctx_t **ctx, const base64_config_t *config);

/**
 * @brief Calculate required buffer size for encoding
 *
 * @param input_length Length of input data
 * @param ctx Base64 context
 * @param output_size Pointer to store required output size
 * @return base64_error_t Error code
 */
base64_error_t base64_get_encode_size(size_t input_length,
                                      const base64_ctx_t *ctx,
                                      size_t *output_size);

/**
 * @brief Calculate required buffer size for decoding
 *
 * @param input_length Length of base64 input string
 * @param ctx Base64 context
 * @param output_size Pointer to store required output size
 * @return base64_error_t Error code
 */
base64_error_t base64_get_decode_size(size_t input_length,
                                      const base64_ctx_t *ctx,
                                      size_t *output_size);

/**
 * @brief Encode binary data to base64 string
 *
 * @param ctx Base64 context
 * @param input Input binary data
 * @param input_length Length of input data
 * @param output Output buffer for base64 string
 * @param output_size Size of output buffer
 * @param output_length Pointer to store actual output length
 * @return base64_error_t Error code
 */
base64_error_t base64_encode(const base64_ctx_t *ctx,
                             const uint8_t *input,
                             size_t input_length,
                             char *output,
                             size_t output_size,
                             size_t *output_length);

/**
 * @brief Decode base64 string to binary data
 *
 * @param ctx Base64 context
 * @param input Input base64 string
 * @param input_length Length of input string
 * @param output Output buffer for binary data
 * @param output_size Size of output buffer
 * @param output_length Pointer to store actual output length
 * @return base64_error_t Error code
 */
base64_error_t base64_decode(base64_ctx_t *ctx,
                             const char *input,
                             size_t input_length,
                             uint8_t *output,
                             size_t output_size,
                             size_t *output_length);

/**
 * @brief Get string description of error code
 *
 * @param error Error code
 * @return const char* Error description
 */
const char *base64_error_string(base64_error_t error);

/**
 * @brief Free base64 context and associated resources
 *
 * @param ctx Base64 context to free
 */
void base64_free(base64_ctx_t *ctx);

#endif //BASE64_H
