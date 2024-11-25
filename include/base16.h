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
// File: base16.h

#ifndef BASE16_H
#define BASE16_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Error codes for base16 operations
 */
typedef enum {
    BASE16_SUCCESS = 0,
    BASE16_ERROR_INVALID_INPUT,    // Input contains invalid characters
    BASE16_ERROR_INVALID_LENGTH,   // Input length is invalid
    BASE16_ERROR_BUFFER_TOO_SMALL, // Output buffer is too small
    BASE16_ERROR_NULL_POINTER,     // NULL pointer provided
    BASE16_ERROR_MEMORY            // Memory allocation failure
} base16_error_t;

/**
 * @brief Configuration options for base16 operations
 */
typedef struct {
    int uppercase;             // Use uppercase (A-F) or lowercase (a-f) letters
    int line_length;           // Length of lines (0 for no line breaks)
    char line_ending[3];       // Line ending sequence (e.g., "\r\n")
} base16_config_t;

/**
 * @brief Base16 context structure
 */
typedef struct base16_ctx_t base16_ctx_t;

/**
 * @brief Initialize a base16 context with the given configuration
 *
 * @param ctx Pointer to context pointer to be initialized
 * @param config Configuration options (NULL for defaults)
 * @return base16_error_t Error code
 */
base16_error_t base16_init(base16_ctx_t **ctx, const base16_config_t *config);

/**
 * @brief Calculate required buffer size for encoding
 *
 * @param input_length Length of input data
 * @param ctx Base16 context
 * @param output_size Pointer to store required output size
 * @return base16_error_t Error code
 */
base16_error_t base16_get_encode_size(size_t input_length,
                                      const base16_ctx_t *ctx,
                                      size_t *output_size);

/**
 * @brief Calculate required buffer size for decoding
 *
 * @param input_length Length of base16 input string
 * @param ctx Base16 context
 * @param output_size Pointer to store required output size
 * @return base16_error_t Error code
 */
base16_error_t base16_get_decode_size(size_t input_length,
                                      const base16_ctx_t *ctx,
                                      size_t *output_size);

/**
 * @brief Encode binary data to base16 string
 *
 * @param ctx Base16 context
 * @param input Input binary data
 * @param input_length Length of input data
 * @param output Output buffer for base16 string
 * @param output_size Size of output buffer
 * @param output_length Pointer to store actual output length
 * @return base16_error_t Error code
 */
base16_error_t base16_encode(base16_ctx_t *ctx,
                             const uint8_t *input,
                             size_t input_length,
                             char *output,
                             size_t output_size,
                             size_t *output_length);

/**
 * @brief Decode base16 string to binary data
 *
 * @param ctx Base16 context
 * @param input Input base16 string
 * @param input_length Length of input string
 * @param output Output buffer for binary data
 * @param output_size Size of output buffer
 * @param output_length Pointer to store actual output length
 * @return base16_error_t Error code
 */
base16_error_t base16_decode(base16_ctx_t *ctx,
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
const char *base16_error_string(base16_error_t error);

/**
 * @brief Free base16 context and associated resources
 *
 * @param ctx Base16 context to free
 */
void base16_free(base16_ctx_t *ctx);

#endif //BASE16_H
