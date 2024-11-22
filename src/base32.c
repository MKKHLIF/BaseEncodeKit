#include <base32.h>
#include <string.h>

#define BASE32_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"
#define BASE32_PAD '='

static const char BASE32HEX_ENCODE_TABLE[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUV";

static const int8_t BASE32HEX_DECODE_TABLE[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};


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

size_t base32hex_encoded_size(size_t in_sz) {
    if (in_sz > SIZE_MAX / 8 * 5) {
        return 0; // Too large to safely encode
    }

    return ((in_sz * 8 + 4) / 5 + 7) / 8;
}

size_t base32hex_decoded_size(size_t encoded_len) {
    // Worst case: 8 input chars decode to 5 bytes
    return (encoded_len * 5 + 7) / 8;
}


ssize_t base32hex_encode(char *restrict out,
                         size_t out_sz,
                         const uint8_t *restrict in,
                         size_t in_sz) {
    if (!out || !in || (out_sz && out_sz < base32hex_encoded_size(in_sz) + 1)) {
        return -1;
    }

    // Prevent integer overflow
    if (in_sz > SIZE_MAX / 5 - 1) {
        return -2;
    }

    size_t encoded_len = 0;
    size_t bit_buffer = 0;
    int bits_in_buffer = 0;

    for (size_t i = 0; i < in_sz; i++) {
        // Push 8 bits into buffer
        bit_buffer = (bit_buffer << 8) | in[i];
        bits_in_buffer += 8;

        // Extract 5-bit groups
        while (bits_in_buffer >= 5) {
            bits_in_buffer -= 5;

            // Extract 5 most significant bits
            size_t index = (bit_buffer >> bits_in_buffer) & 0x1F;

            // Encode if buffer has space
            if (encoded_len < out_sz - 1) {
                out[encoded_len] = BASE32HEX_ENCODE_TABLE[index];
            }
            encoded_len++;
        }
    }

    // Handle remaining bits
    if (bits_in_buffer > 0) {
        // Shift remaining bits to the right and pad
        size_t index = (bit_buffer << (5 - bits_in_buffer)) & 0x1F;

        if (encoded_len < out_sz - 1) {
            out[encoded_len] = BASE32HEX_ENCODE_TABLE[index];
        }
        encoded_len++;
    }

    // Null-terminate if space allows
    if (out_sz > 0) {
        out[encoded_len < out_sz ? encoded_len : out_sz - 1] = '\0';
    }

    return encoded_len;
}


ssize_t base32hex_decode(uint8_t *restrict out,
                         size_t out_sz,
                         const char *restrict in,
                         size_t in_sz) {
    // If in_sz is 0, use strlen
    if (in_sz == 0) {
        in_sz = strlen(in);
    }

    // Validate inputs
    if (!out || !in ||
        (out_sz && out_sz < base32hex_decoded_size(in_sz))) {
        return -1;
    }

    size_t decoded_len = 0;
    size_t bit_buffer = 0;
    int bits_in_buffer = 0;

    for (size_t i = 0; i < in_sz; i++) {
        // Ignore whitespace
        if (in[i] <= ' ') continue;

        // Decode character
        int8_t val = BASE32HEX_DECODE_TABLE[(unsigned char) in[i]];
        if (val < 0) {
            return -2; // Invalid character
        }

        // Push 5 bits into buffer
        bit_buffer = (bit_buffer << 5) | val;
        bits_in_buffer += 5;

        // Extract full bytes
        while (bits_in_buffer >= 8) {
            bits_in_buffer -= 8;
            uint8_t byte = (bit_buffer >> bits_in_buffer) & 0xFF;

            // Write to output if space allows
            if (decoded_len < out_sz) {
                out[decoded_len] = byte;
            }
            decoded_len++;
        }
    }

    return decoded_len;
}
