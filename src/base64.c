#include "base64.h"
#include <stdint.h>
#include <stdlib.h>

#define BASE64_ERR_NULL_PTR       (-1)  // NULL pointer error
#define BASE64_ERR_SMALL_BUFFER   (-2)  // Output buffer is too small
#define BASE64_ERR_INVALID_INPUT  (-3)  // Input string contains invalid characters or improper padding
#define BASE64_ERR_EMPTY_INPUT    (-4)  // Input is empty or invalid

size_t base64_encoded_size(const size_t input_length) {
    // Check for potential overflow in calculation
    if (input_length > (SIZE_MAX - 3) / 4) {
        return 0;
    }
    return 4 * ((input_length + 2) / 3) + 1;
}

size_t base64_decoded_size(const size_t in_sz) {
    return (in_sz / 4) * 3;
}


ssize_t base64_encode(char *out, const size_t out_sz, const uint8_t *in, const size_t in_sz) {
    if (!out || out_sz == 0) {
        return -1;
    }

    out[0] = '\0';

    if ((!in && in_sz > 0)) {
        return -1;
    }

    const size_t required_sz = base64_encoded_size(in_sz);
    if (required_sz == 0) {
        return -2; // Input too large
    }

    if (out_sz < required_sz) {
        return -1;
    }

    static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    size_t i, j = 0;
    uint32_t buffer = 0;
    int bits = 0;

    for (i = 0; i < in_sz; i++) {
        buffer = (buffer << 8) | in[i];
        bits += 8;

        while (bits >= 6) {
            bits -= 6;
            out[j++] = base64_chars[(buffer >> bits) & 0x3F];
        }
    }

    if (bits > 0) {
        buffer <<= (6 - bits);
        out[j++] = base64_chars[buffer & 0x3F];
    }

    while (j % 4 != 0) {
        out[j++] = '=';
    }

    out[j] = '\0';
    return j;
}

ssize_t base64_decode(uint8_t *restrict out,
                      size_t out_sz,
                      const char *restrict in,
                      size_t in_sz) {
    static const int base64_decoding_table[256] = {
        ['A'] = 0, ['B'] = 1, ['C'] = 2, ['D'] = 3, ['E'] = 4, ['F'] = 5, ['G'] = 6, ['H'] = 7,
        ['I'] = 8, ['J'] = 9, ['K'] = 10, ['L'] = 11, ['M'] = 12, ['N'] = 13, ['O'] = 14, ['P'] = 15,
        ['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19, ['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23,
        ['Y'] = 24, ['Z'] = 25, ['a'] = 26, ['b'] = 27, ['c'] = 28, ['d'] = 29, ['e'] = 30, ['f'] = 31,
        ['g'] = 32, ['h'] = 33, ['i'] = 34, ['j'] = 35, ['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39,
        ['o'] = 40, ['p'] = 41, ['q'] = 42, ['r'] = 43, ['s'] = 44, ['t'] = 45, ['u'] = 46, ['v'] = 47,
        ['w'] = 48, ['x'] = 49, ['y'] = 50, ['z'] = 51, ['0'] = 52, ['1'] = 53, ['2'] = 54, ['3'] = 55,
        ['4'] = 56, ['5'] = 57, ['6'] = 58, ['7'] = 59, ['8'] = 60, ['9'] = 61, ['+'] = 62, ['/'] = 63,
        ['='] = -1 // Padding character
    };


    if (!in || !out) return BASE64_ERR_NULL_PTR;
    if (in_sz == 0) return BASE64_ERR_EMPTY_INPUT;

    // Handle input padding
    size_t padded_in_sz = in_sz;
    while (padded_in_sz > 0 && in[padded_in_sz - 1] == '=') {
        padded_in_sz--;
    }

    // Calculate required buffer size for decoding
    size_t required_out_sz = base64_decoded_size(padded_in_sz);
    if (out_sz < required_out_sz) {
        return required_out_sz; // Return required size for larger buffer
    }

    size_t idx = 0, out_idx = 0;
    while (idx < padded_in_sz) {
        // Get 4 Base64 characters
        int a = (in[idx] >= 0 && in[idx] <= 255) ? base64_decoding_table[(unsigned char) in[idx]] : -1;
        int b = (in[idx + 1] >= 0 && in[idx + 1] <= 255) ? base64_decoding_table[(unsigned char) in[idx + 1]] : -1;
        int c = (in[idx + 2] >= 0 && in[idx + 2] <= 255) ? base64_decoding_table[(unsigned char) in[idx + 2]] : -1;
        int d = (in[idx + 3] >= 0 && in[idx + 3] <= 255) ? base64_decoding_table[(unsigned char) in[idx + 3]] : -1;

        // Check for invalid characters
        if (a == -1 || b == -1 || (c == -1 && in[idx + 2] != '=') || (d == -1 && in[idx + 3] != '=')) {
            return BASE64_ERR_INVALID_INPUT;
        }

        // Decode 4 Base64 characters into 3 bytes
        out[out_idx++] = (a << 2) | (b >> 4);
        if (in[idx + 2] != '=') out[out_idx++] = ((b & 0x0F) << 4) | (c >> 2);
        if (in[idx + 3] != '=') out[out_idx++] = ((c & 0x03) << 6) | d;

        idx += 4;
    }

    return out_idx; // Return the number of bytes written to the output buffer
}

ssize_t base64_url_decode(uint8_t *out, size_t out_sz, const char *in, size_t in_sz) {
    // Base64 URL decoding alphabet (URL-safe variant)
    static const int8_t base64_decode_table[256] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 63,
        -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
        // Rest of the table initialized to -1
    };

    // Validate input
    if (!out || !in || in_sz == 0) {
        return -1;
    }

    // Track decoded bytes and input parsing
    size_t out_idx = 0;
    size_t in_idx = 0;
    uint32_t bits = 0;
    int bits_collected = 0;

    while (in_idx < in_sz) {
        // Skip whitespace and ignored characters
        char c = in[in_idx];
        if (c == '\r' || c == '\n' || c == ' ' || c == '\t') {
            in_idx++;
            continue;
        }

        // Decode character
        int8_t decoded = base64_decode_table[(unsigned char) c];
        if (decoded == -1) {
            // Invalid character
            return -1;
        }

        // Accumulate bits
        bits = (bits << 6) | decoded;
        bits_collected += 6;

        // When we have 8 or more bits, output a byte
        if (bits_collected >= 8) {
            bits_collected -= 8;
            uint8_t byte = (bits >> bits_collected) & 0xFF;

            // Check output buffer
            if (out_idx >= out_sz) {
                return -1; // Output buffer too small
            }

            out[out_idx++] = byte;
        }

        in_idx++;
    }

    // Check if we have a valid Base64 sequence
    if (bits_collected != 0) {
        return -1; // Incomplete Base64 sequence
    }

    return out_idx;
}
