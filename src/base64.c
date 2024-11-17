//
// Created by mk on 15/11/2024.
//

#include "base64.h"
#include <stdlib.h>


void add_padding(char *output, size_t *j, const int padding) {
    if (padding == 2) {
        output[*j] = '=';
        (*j)++;
        output[*j] = '=';
        (*j)++;
    } else if (padding == 1) {
        output[*j] = '=';
        (*j)++;
    }
}

char *base64_encode(const uint8_t *in, const size_t in_sz) {
    size_t output_len = 4 * ((in_sz + 2) / 3); // Base64 output length

    output_len += 2; // Padding can be 1 or 2 characters

    // Allocate memory for the encoded string
    char *output = (char *) malloc(output_len + 1); // +1 for null terminator
    if (!output) {
        return NULL; // Return NULL if memory allocation fails
    }

    size_t i, j = 0;
    uint32_t buffer = 0;
    size_t bits = 0;

    // Process each byte in the input in
    for (i = 0; i < in_sz; i++) {
        // Accumulate in into the buffer
        buffer <<= 8;
        buffer |= in[i];
        bits += 8;

        // Extract 6 bits at a time and encode
        while (bits >= 6) {
            bits -= 6;
            output[j++] = GET_BASE64_CHAR(buffer >> bits); // Use macro to get 6 bits
        }
    }

    // Handle any remaining bits and add them as the last base64 character
    if (bits > 0) {
        buffer <<= (6 - bits); // Left shift to fill remaining bits
        output[j++] = GET_BASE64_CHAR(buffer & 0x3F);
    }

    // Add padding if necessary
    const int padding = (3 - (in_sz % 3)) % 3;
    add_padding(output, &j, padding); // Add padding using the non-macro function


    // Null-terminate the encoded string
    output[j] = '\0';

    return output;
}
