#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "base16.h"

// Internal context structure
struct base16_ctx_t {
    int uppercase;
    int line_length;
    char line_ending[3];
    int current_line_length;
};

// Default configuration
static const base16_config_t DEFAULT_CONFIG = {
    .uppercase = 1,
    .line_length = 76,
    .line_ending = "\n"
};

base16_error_t base16_init(base16_ctx_t **ctx, const base16_config_t *config) {
    if (ctx == NULL) {
        return BASE16_ERROR_NULL_POINTER;
    }

    // Allocate context
    *ctx = malloc(sizeof(base16_ctx_t));
    if (*ctx == NULL) {
        return BASE16_ERROR_MEMORY;
    }

    // Use default config if not provided
    const base16_config_t *effective_config = config ? config : &DEFAULT_CONFIG;

    // Copy configuration
    (*ctx)->uppercase = effective_config->uppercase;
    (*ctx)->line_length = effective_config->line_length;
    (*ctx)->current_line_length = 0;
    strncpy((*ctx)->line_ending, effective_config->line_ending, sizeof((*ctx)->line_ending) - 1);

    return BASE16_SUCCESS;
}

base16_error_t base16_get_encode_size(size_t input_length,
                                      const base16_ctx_t *ctx,
                                      size_t *output_size) {
    if (ctx == NULL || output_size == NULL) {
        return BASE16_ERROR_NULL_POINTER;
    }

    // Calculate base16 encoded size (2 chars per byte)
    size_t base_size = input_length * 2;

    // Add line breaks if needed
    if (ctx->line_length > 0) {
        size_t line_breaks = (base_size / ctx->line_length) * strlen(ctx->line_ending);
        base_size += line_breaks;
    }

    *output_size = base_size + 1; // +1 for null terminator
    return BASE16_SUCCESS;
}

base16_error_t base16_get_decode_size(size_t input_length,
                                      const base16_ctx_t *ctx,
                                      size_t *output_size) {
    if (ctx == NULL || output_size == NULL) {
        return BASE16_ERROR_NULL_POINTER;
    }

    // Each 2 chars decode to 1 byte
    *output_size = input_length / 2 + 1;
    return BASE16_SUCCESS;
}

base16_error_t base16_encode(base16_ctx_t *ctx,
                             const uint8_t *input,
                             size_t input_length,
                             char *output,
                             size_t output_size,
                             size_t *output_length) {
    if (ctx == NULL || input == NULL || output == NULL || output_length == NULL) {
        return BASE16_ERROR_NULL_POINTER;
    }

    size_t required_size;
    base16_error_t size_check = base16_get_encode_size(input_length, ctx, &required_size);
    if (size_check != BASE16_SUCCESS) return size_check;

    if (output_size < required_size) {
        return BASE16_ERROR_BUFFER_TOO_SMALL;
    }

    size_t out_idx = 0;
    size_t line_count = 0;

    for (size_t i = 0; i < input_length; i++) {
        // Hex encoding for each byte
        int high = (input[i] >> 4) & 0x0F;
        int low = input[i] & 0x0F;

        // Convert to hex character
        output[out_idx++] = high + (high > 9 ? (ctx->uppercase ? 'A' - 10 : 'a' - 10) : '0');
        output[out_idx++] = low + (low > 9 ? (ctx->uppercase ? 'A' - 10 : 'a' - 10) : '0');

        // Add line breaks if configured
        if (ctx->line_length > 0) {
            line_count += 2;
            if (line_count >= ctx->line_length) {
                strcpy(output + out_idx, ctx->line_ending);
                out_idx += strlen(ctx->line_ending);
                line_count = 0;
            }
        }
    }

    output[out_idx] = '\0';
    *output_length = out_idx;

    return BASE16_SUCCESS;
}

base16_error_t base16_decode(base16_ctx_t *ctx,
                             const char *input,
                             size_t input_length,
                             uint8_t *output,
                             size_t output_size,
                             size_t *output_length) {
    if (ctx == NULL || input == NULL || output == NULL || output_length == NULL) {
        return BASE16_ERROR_NULL_POINTER;
    }

    size_t required_size;
    base16_error_t size_check = base16_get_decode_size(input_length, ctx, &required_size);
    if (size_check != BASE16_SUCCESS) return size_check;

    if (output_size < required_size) {
        return BASE16_ERROR_BUFFER_TOO_SMALL;
    }

    size_t out_idx = 0;

    for (size_t i = 0; i < input_length; i += 2) {
        // Skip whitespace
        while (i < input_length && isspace(input[i])) i++;
        if (i + 1 >= input_length) break;

        // Convert hex characters to byte
        int high = isdigit(input[i]) ? input[i] - '0' : (tolower(input[i]) - 'a' + 10);
        int low = isdigit(input[i + 1]) ? input[i + 1] - '0' : (tolower(input[i + 1]) - 'a' + 10);

        // Validate hex characters
        if (high > 15 || low > 15) {
            return BASE16_ERROR_INVALID_INPUT;
        }

        output[out_idx++] = (high << 4) | low;
    }

    *output_length = out_idx;
    return BASE16_SUCCESS;
}

const char *base16_error_string(base16_error_t error) {
    switch (error) {
        case BASE16_SUCCESS: return "Success";
        case BASE16_ERROR_INVALID_INPUT: return "Invalid input";
        case BASE16_ERROR_INVALID_LENGTH: return "Invalid length";
        case BASE16_ERROR_BUFFER_TOO_SMALL: return "Buffer too small";
        case BASE16_ERROR_NULL_POINTER: return "Null pointer";
        case BASE16_ERROR_MEMORY: return "Memory allocation failed";
        default: return "Unknown error";
    }
}

void base16_free(base16_ctx_t *ctx) {
    if (ctx != NULL) {
        free(ctx);
    }
}
