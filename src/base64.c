#include "base64.h"
#include <stdint.h>
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

char *base64_encode(const char *in, const size_t in_sz) {
    size_t output_len = 4 * ((in_sz + 2) / 3);

    output_len += 2;

    char *output = (char *) malloc(output_len + 1);
    if (!output) {
        return NULL;
    }

    size_t i, j = 0;
    uint32_t buffer = 0;
    size_t bits = 0;

    for (i = 0; i < in_sz; i++) {
        buffer <<= 8;
        buffer |= in[i];
        bits += 8;

        while (bits >= 6) {
            bits -= 6;
            output[j++] = GET_BASE64_CHAR(buffer >> bits);
        }
    }

    if (bits > 0) {
        buffer <<= (6 - bits);
        output[j++] = GET_BASE64_CHAR(buffer & 0x3F);
    }

    const int padding = (3 - (in_sz % 3)) % 3;
    add_padding(output, &j, padding);


    output[j] = '\0';

    return output;
}
