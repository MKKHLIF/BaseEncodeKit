#include <stdlib.h>
#include <string.h>
#include <base64.h>

// Internal base64 alphabet and constants
static const char BASE64_STANDARD_ALPHABET[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char BASE64_URL_ALPHABET[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

// Internal context structure
struct base64_ctx_t {
    char alphabet[64];
    int use_padding;
    int url_safe;
    int line_length;
    char line_ending[3];
    int current_line_length;
};

// Default configuration
static const base64_config_t DEFAULT_CONFIG = {
    .use_padding = 1,
    .url_safe = 0,
    .line_length = 76,
    .line_ending = "\n"
};

base64_error_t base64_init(base64_ctx_t **ctx, const base64_config_t *config) {
    if (ctx == NULL) {
        return BASE64_ERROR_NULL_POINTER;
    }

    // Allocate context
    *ctx = malloc(sizeof(base64_ctx_t));
    if (*ctx == NULL) {
        return BASE64_ERROR_MEMORY;
    }

    // Use default config if not provided
    const base64_config_t *effective_config = config ? config : &DEFAULT_CONFIG;

    // Set alphabet based on config
    memcpy((*ctx)->alphabet,
           effective_config->url_safe ? BASE64_URL_ALPHABET : BASE64_STANDARD_ALPHABET,
           64);

    // Copy configuration
    (*ctx)->use_padding = effective_config->use_padding;
    (*ctx)->url_safe = effective_config->url_safe;
    (*ctx)->line_length = effective_config->line_length;
    (*ctx)->current_line_length = 0;
    strncpy((*ctx)->line_ending, effective_config->line_ending, sizeof((*ctx)->line_ending) - 1);

    return BASE64_SUCCESS;
}

base64_error_t base64_get_encode_size(const size_t input_length,
                                      const base64_ctx_t *ctx,
                                      size_t *output_size) {
    if (ctx == NULL || output_size == NULL) {
        return BASE64_ERROR_NULL_POINTER;
    }

    // Calculate base64 encoded size
    size_t base_size = 4 * ((input_length + 2) / 3);

    // Add line breaks if needed
    if (ctx->line_length > 0) {
        const size_t line_breaks = (base_size / ctx->line_length) * strlen(ctx->line_ending);
        base_size += line_breaks;
    }

    *output_size = base_size + 1; // +1 for null terminator
    return BASE64_SUCCESS;
}

base64_error_t base64_get_decode_size(const size_t input_length,
                                      const base64_ctx_t *ctx,
                                      size_t *output_size) {
    if (ctx == NULL || output_size == NULL) {
        return BASE64_ERROR_NULL_POINTER;
    }

    // Estimate max decoded size (may be slightly larger than exact)
    *output_size = (input_length * 3) / 4 + 2;
    return BASE64_SUCCESS;
}

const char *base64_error_string(const base64_error_t error) {
    switch (error) {
        case BASE64_SUCCESS: return "Success";
        case BASE64_ERROR_INVALID_INPUT: return "Invalid input";
        case BASE64_ERROR_INVALID_LENGTH: return "Invalid length";
        case BASE64_ERROR_BUFFER_TOO_SMALL: return "Buffer too small";
        case BASE64_ERROR_NULL_POINTER: return "Null pointer";
        case BASE64_ERROR_PADDING: return "Invalid padding";
        case BASE64_ERROR_MEMORY: return "Memory allocation failed";
        default: return "Unknown error";
    }
}

void base64_free(base64_ctx_t *ctx) {
    if (ctx != NULL) {
        free(ctx);
    }
}

// Helper function to find index in alphabet
static int find_alphabet_index(const char c, const char *alphabet) {
    for (int i = 0; i < 64; i++) {
        if (alphabet[i] == c) return i;
    }
    return -1;
}

base64_error_t base64_encode(const base64_ctx_t *ctx,
                             const uint8_t *input,
                             const size_t input_length,
                             char *output,
                             const size_t output_size,
                             size_t *output_length) {
    if (ctx == NULL || input == NULL || output == NULL || output_length == NULL) {
        return BASE64_ERROR_NULL_POINTER;
    }

    size_t required_size;
    base64_error_t size_check = base64_get_encode_size(input_length, ctx, &required_size);
    if (size_check != BASE64_SUCCESS) return size_check;

    if (output_size < required_size) {
        return BASE64_ERROR_BUFFER_TOO_SMALL;
    }

    size_t bits = 0;
    uint32_t buffer = 0;
    size_t output_index = 0;

    for (size_t i = 0; i < input_length; ++i) {
        buffer <<= 8;
        buffer += input[i];
        bits += 8;

        while (bits >= 6) {
            output[output_index++] = ctx->alphabet[(buffer >> (bits - 6)) & 0x3f];
            buffer &= ~(0x3f << (bits - 6));
            bits -= 6;
        }
    }

    if (input_length % 3 == 1) {
        buffer <<= 4;
        output[output_index++] = ctx->alphabet[buffer & 0x3f];
        if (ctx->use_padding) {
            output[output_index++] = '=';
            output[output_index++] = '=';
        }
    } else if (input_length % 3 == 2) {
        buffer <<= 2;
        output[output_index++] = ctx->alphabet[buffer & 0x3f];
        if (ctx->use_padding) {
            output[output_index++] = '=';
        }
    }
    output[output_index] = '\0';
    *output_length = output_index;

    return BASE64_SUCCESS;
}

base64_error_t base64_decode(base64_ctx_t *ctx,
                             const char *input,
                             size_t input_length,
                             uint8_t *output,
                             size_t output_size,
                             size_t *output_length) {
    if (ctx == NULL || input == NULL || output == NULL || output_length == NULL) {
        return BASE64_ERROR_NULL_POINTER;
    }

    size_t required_size;
    base64_error_t size_check = base64_get_decode_size(input_length, ctx, &required_size);
    if (size_check != BASE64_SUCCESS) return size_check;

    if (output_size < required_size) {
        return BASE64_ERROR_BUFFER_TOO_SMALL;
    }

    size_t out_idx = 0;
    uint32_t n = 0;
    int group_count = 0;

    for (size_t i = 0; i < input_length; i++) {
        // Skip whitespace and line breaks
        if (input[i] == ' ' || input[i] == '\n' || input[i] == '\r')
            continue;

        // Check for padding
        if (input[i] == '=') {
            if (group_count >= 2) break;
            n <<= 6;
            group_count++;
            continue;
        }

        // Find index in alphabet
        int index = find_alphabet_index(input[i], ctx->alphabet);
        if (index == -1) {
            return BASE64_ERROR_INVALID_INPUT;
        }

        n = (n << 6) | index;
        group_count++;

        // Every 4 characters (groups of 6-bit), output 3 bytes
        if (group_count == 4) {
            output[out_idx++] = (n >> 16) & 0xFF;
            output[out_idx++] = (n >> 8) & 0xFF;
            output[out_idx++] = n & 0xFF;
            n = 0;
            group_count = 0;
        }
    }

    // Handle remaining bits for the last group
    switch (group_count) {
        case 3:
            output[out_idx++] = (n >> 10) & 0xFF;
            output[out_idx++] = (n >> 2) & 0xFF;
            break;
        case 2:
            output[out_idx++] = (n >> 4) & 0xFF;
            break;
    }

    *output_length = out_idx;
    return BASE64_SUCCESS;
}
