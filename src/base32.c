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

ssize_t base32_decode(uint8_t *restrict out,
                      size_t out_sz,
                      const char *restrict in,
                      size_t in_sz) {
    if (out == NULL || in == NULL || out_sz == 0 || in_sz == 0) {
        return -1; // Invalid pointers or sizes
    }

    // Base32 decoding uses a 8-to-5 ratio, so the output size will be roughly 5/8 of the input size
    size_t required_size = (in_sz * 5) / 8;
    if (out_sz < required_size) {
        return -1; // Insufficient output buffer size
    }

    // Create a lookup table for the Base32 alphabet
    uint8_t base32_table[256];
    for (size_t i = 0; i < 256; ++i) {
        base32_table[i] = 0xFF; // Invalid character by default
    }

    for (size_t i = 0; BASE32_ALPHABET[i] != '\0'; ++i) {
        base32_table[(unsigned char) BASE32_ALPHABET[i]] = (uint8_t) i;
    }

    base32_table[(unsigned char) BASE32_PAD] = 0xFF; // Padding character

    size_t i = 0, j = 0;
    uint32_t buffer = 0;
    size_t buffer_bits = 0;

    while (i < in_sz) {
        unsigned char c = in[i++];

        // Skip padding characters
        if (c == BASE32_PAD) {
            break;
        }

        // Get the Base32 value for the current character
        uint8_t value = base32_table[(unsigned char) c];

        // Check if it's a valid Base32 character
        if (value == 0xFF) {
            return -2; // Invalid character in input
        }

        // Accumulate bits into the buffer
        buffer = (buffer << 5) | value;
        buffer_bits += 5;

        // While we have at least 8 bits, write a byte to the output
        while (buffer_bits >= 8) {
            out[j++] = (buffer >> (buffer_bits - 8)) & 0xFF;
            buffer_bits -= 8;
        }
    }

    // Check for incomplete padding
    if (buffer_bits != 0) {
        return -3; // Invalid padding or incomplete data
    }

    return j; // Return the number of bytes written
}
