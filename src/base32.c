#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base32.h"

// Standard base32 and base32hex alphabets
static const char BASE32_STANDARD_ALPHABET[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
static const char BASE32_HEX_ALPHABET[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUV";

// Internal context structure
struct base32_ctx_t {
    char alphabet[32];
    int use_padding;
    int use_hex;
    int line_length;
    char line_ending[3];
    int current_line_length;
};

// Default configuration
static const base32_config_t DEFAULT_CONFIG = {
    .use_padding = 1,
    .use_hex = 0,
    .line_length = 76,
    .line_ending = "\n"
};

base32_error_t base32_init(base32_ctx_t **ctx, const base32_config_t *config) {
    if (ctx == NULL) {
        return BASE32_ERROR_NULL_POINTER;
    }

    // Allocate context
    *ctx = malloc(sizeof(base32_ctx_t));
    if (*ctx == NULL) {
        return BASE32_ERROR_MEMORY;
    }

    // Use default config if not provided
    const base32_config_t *effective_config = config ? config : &DEFAULT_CONFIG;

    // Set alphabet based on config
    memcpy((*ctx)->alphabet,
           effective_config->use_hex ? BASE32_HEX_ALPHABET : BASE32_STANDARD_ALPHABET,
           32);

    // Copy configuration
    (*ctx)->use_padding = effective_config->use_padding;
    (*ctx)->use_hex = effective_config->use_hex;
    (*ctx)->line_length = effective_config->line_length;
    (*ctx)->current_line_length = 0;
    strncpy((*ctx)->line_ending, effective_config->line_ending, sizeof((*ctx)->line_ending) - 1);

    return BASE32_SUCCESS;
}

base32_error_t base32_get_encode_size(size_t input_length,
                                      const base32_ctx_t *ctx,
                                      size_t *output_size) {
    if (ctx == NULL || output_size == NULL) {
        return BASE32_ERROR_NULL_POINTER;
    }

    // Calculate base32 encoded size
    size_t base_size = 8 * ((input_length + 4) / 5);

    // Add line breaks if needed
    if (ctx->line_length > 0) {
        size_t line_breaks = (base_size / ctx->line_length) * strlen(ctx->line_ending);
        base_size += line_breaks;
    }

    *output_size = base_size + 1; // +1 for null terminator
    return BASE32_SUCCESS;
}

base32_error_t base32_get_decode_size(size_t input_length,
                                      const base32_ctx_t *ctx,
                                      size_t *output_size) {
    if (ctx == NULL || output_size == NULL) {
        return BASE32_ERROR_NULL_POINTER;
    }

    // Estimate max decoded size
    *output_size = (input_length * 5) / 8 + 2;
    return BASE32_SUCCESS;
}

// Helper function to find index in alphabet
static int find_alphabet_index(char c, const char *alphabet) {
    for (int i = 0; i < 32; i++) {
        if (alphabet[i] == c) return i;
    }
    return -1;
}

base32_error_t base32_encode(const base32_ctx_t *ctx,
                             const uint8_t *input,
                             const size_t input_length,
                             char *output,
                             const size_t output_size,
                             size_t *output_length) {
    if (ctx == NULL || input == NULL || output == NULL || output_length == NULL) {
        return BASE32_ERROR_NULL_POINTER;
    }

    size_t required_size;
    const base32_error_t size_check = base32_get_encode_size(input_length, ctx, &required_size);
    if (size_check != BASE32_SUCCESS) return size_check;

    if (output_size < required_size) {
        return BASE32_ERROR_BUFFER_TOO_SMALL;
    }

    size_t bits = 0;
    uint32_t buffer = 0;
    size_t output_index = 0;

    for (size_t i = 0; i < input_length; ++i) {
        buffer <<= 8;
        buffer += input[i];
        bits += 8;

        while (bits >= 5) {
            output[output_index++] = ctx->alphabet[(buffer >> (bits - 5)) & 0x1f];
            buffer &= ~(0x1f << (bits - 5));
            bits -= 5;
        }
    }

    // Handle padding for the last few bytes
    if (input_length % 5 == 1) {
        buffer <<= 2;
        output[output_index++] = ctx->alphabet[buffer & 0x1f];
        if (ctx->use_padding) {
            output[output_index++] = '='; output[output_index++] = '='; output[output_index++] = '='; output[output_index++] = '='; output[output_index++] = '='; output[output_index++] = '=';
        }
    } else if (input_length % 5 == 2) {
        buffer <<= 4;
        output[output_index++] = ctx->alphabet[buffer & 0x1f];
        if (ctx->use_padding) {
            output[output_index++] = '='; output[output_index++] = '='; output[output_index++] = '='; output[output_index++] = '=';
        }
    } else if (input_length % 5 == 3) {
        buffer <<= 1;
        output[output_index++] = ctx->alphabet[buffer & 0x1f];
        if (ctx->use_padding) {
            output[output_index++] = '='; output[output_index++] = '='; output[output_index++] = '=';
        }
    } else if (input_length % 5 == 4) {
        buffer <<= 3;
        output[output_index++] = ctx->alphabet[buffer & 0x1f];
        if (ctx->use_padding) {
            output[output_index++] = '=';
        }
    }
    output[output_index] = '\0';
    *output_length = output_index;

    return BASE32_SUCCESS;
}

base32_error_t base32_decode(base32_ctx_t *ctx,
                             const char *input,
                             size_t input_length,
                             uint8_t *output,
                             size_t output_size,
                             size_t *output_length) {
    if (ctx == NULL || input == NULL || output == NULL || output_length == NULL) {
        return BASE32_ERROR_NULL_POINTER;
    }

    size_t required_size;
    base32_error_t size_check = base32_get_decode_size(input_length, ctx, &required_size);
    if (size_check != BASE32_SUCCESS) return size_check;

    if (output_size < required_size) {
        return BASE32_ERROR_BUFFER_TOO_SMALL;
    }

    size_t out_idx = 0;
    uint64_t n = 0;
    int group_count = 0;

    for (size_t i = 0; i < input_length; i++) {
        // Skip whitespace and line breaks
        if (input[i] == ' ' || input[i] == '\n' || input[i] == '\r')
            continue;

        // Check for padding
        if (input[i] == '=') {
            if (group_count >= 2 && group_count <= 7) break;
            group_count++;
            continue;
        }

        // Find index in alphabet
        int index = find_alphabet_index(input[i], ctx->alphabet);
        if (index == -1) {
            return BASE32_ERROR_INVALID_INPUT;
        }

        n = (n << 5) | index;
        group_count++;

        // Every 8 characters (groups of 5-bit), output 5 bytes
        if (group_count == 8) {
            for (int j = 4; j >= 0; j--) {
                output[out_idx++] = (n >> (j * 8)) & 0xFF;
            }
            n = 0;
            group_count = 0;
        }
    }

    // Handle remaining bits for the last group
    int padding_bits = group_count == 7 ? 4 : group_count == 5 ? 3 : group_count == 4 ? 1 : group_count == 2 ? 0 : -1;

    if (padding_bits >= 0) {
        n >>= padding_bits;
        switch (group_count) {
            case 7: // 4 bytes
                output[out_idx++] = (n >> 24) & 0xFF;
                output[out_idx++] = (n >> 16) & 0xFF;
                output[out_idx++] = (n >> 8) & 0xFF;
                output[out_idx++] = n & 0xFF;
                break;
            case 5: // 3 bytes
                output[out_idx++] = (n >> 16) & 0xFF;
                output[out_idx++] = (n >> 8) & 0xFF;
                output[out_idx++] = n & 0xFF;
                break;
            case 4: // 2 bytes
                output[out_idx++] = (n >> 8) & 0xFF;
                output[out_idx++] = n & 0xFF;
                break;
            case 2: // 1 byte
                output[out_idx++] = n & 0xFF;
                break;
        }
    }

    *output_length = out_idx;
    return BASE32_SUCCESS;
}

const char *base32_error_string(base32_error_t error) {
    switch (error) {
        case BASE32_SUCCESS: return "Success";
        case BASE32_ERROR_INVALID_INPUT: return "Invalid input";
        case BASE32_ERROR_INVALID_LENGTH: return "Invalid length";
        case BASE32_ERROR_BUFFER_TOO_SMALL: return "Buffer too small";
        case BASE32_ERROR_NULL_POINTER: return "Null pointer";
        case BASE32_ERROR_PADDING: return "Invalid padding";
        case BASE32_ERROR_MEMORY: return "Memory allocation failed";
        default: return "Unknown error";
    }
}

void base32_free(base32_ctx_t *ctx) {
    if (ctx != NULL) {
        free(ctx);
    }
}
