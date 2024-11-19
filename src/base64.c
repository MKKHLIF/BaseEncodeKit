#include "base64.h"
#include <stdint.h>
#include <stdlib.h>


// void add_padding(char *output, size_t *j, const int padding) {
//     if (padding == 2) {
//         output[*j] = '=';
//         (*j)++;
//         output[*j] = '=';
//         (*j)++;
//     } else if (padding == 1) {
//         output[*j] = '=';
//         (*j)++;
//     }
// }
//
// char *base64_encode(const char *in, const size_t in_sz) {
//     size_t output_len = 4 * ((in_sz + 2) / 3);
//
//     output_len += 2;
//
//     char *output = (char *) malloc(output_len + 1);
//     if (!output) {
//         return NULL;
//     }
//
//     size_t i, j = 0;
//     uint32_t buffer = 0;
//     size_t bits = 0;
//
//     for (i = 0; i < in_sz; i++) {
//         buffer <<= 8;
//         buffer |= in[i];
//         bits += 8;
//
//         while (bits >= 6) {
//             bits -= 6;
//             output[j++] = GET_BASE64_CHAR(buffer >> bits);
//         }
//     }
//
//     if (bits > 0) {
//         buffer <<= (6 - bits);
//         output[j++] = GET_BASE64_CHAR(buffer & 0x3F);
//     }
//
//     const int padding = (3 - (in_sz % 3)) % 3;
//     add_padding(output, &j, padding);
//
//
//     output[j] = '\0';
//
//     return output;
// }


size_t base64_buff_sz(const size_t input_length) {
    // Check for potential overflow in calculation
    if (input_length > (SIZE_MAX - 3) / 4) {
        return 0;
    }
    return 4 * ((input_length + 2) / 3) + 1;
}


ssize_t base64_encode(char *out, const size_t out_sz, const uint8_t *in, const size_t in_sz) {
    if (!out || out_sz == 0) {
        return -1;
    }

    out[0] = '\0';

    if ((!in && in_sz > 0)) {
        return -1;
    }

    const size_t required_sz = base64_buf_sz(in_sz);
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
