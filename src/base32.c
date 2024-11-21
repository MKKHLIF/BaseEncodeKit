#include <base32.h>

#define BASE32_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"
#define BASE32_PAD '='

ssize_t base32_encode(char *restrict out,
                      size_t out_sz,
                      const uint8_t *restrict in,
                      size_t in_sz) {
    if (out == NULL || in == NULL || out_sz == 0 || in_sz == 0) {
        return -1; // Invalid pointers or sizes
    }

    // Base32 encoding uses a 5-to-8 ratio, so the output size will be roughly 8/5 of the input size
    size_t required_size = ((in_sz + 4) / 5) * 8; // This accounts for padding

    if (out_sz < required_size + 1) {
        // +1 for null terminator
        return -1; // Insufficient output buffer size
    }

    size_t i = 0, j = 0;
    uint32_t buffer = 0;
    size_t buffer_bits = 0;

    while (i < in_sz) {
        buffer = (buffer << 8) | in[i++];
        buffer_bits += 8;

        // While we have at least 5 bits, encode a Base32 character
        while (buffer_bits >= 5) {
            out[j++] = BASE32_ALPHABET[(buffer >> (buffer_bits - 5)) & 0x1F];
            buffer_bits -= 5;
        }
    }

    // Handle any remaining bits (padding)
    if (buffer_bits > 0) {
        out[j++] = BASE32_ALPHABET[(buffer << (5 - buffer_bits)) & 0x1F];
    }

    // Add padding if needed
    while (j < required_size) {
        out[j++] = BASE32_PAD;
    }

    out[j] = '\0'; // Null-terminate the output string
    return j; // Return the number of characters written
}
