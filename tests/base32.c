#include <unity.h>
#include <stdlib.h>
#include <string.h>

#include "base32.h"

#define BUFFER_SIZE 128  // Example buffer size, adjust as needed

struct Base32TestVector {
    const char *input;          // Input string to be encoded
    const char *encoded;        // Expected encoded base32 string using default alphabet
    const char *encoded_hex;    // Expected encoded base32 string using hex-based alphabet (if needed)
};

// Test vectors
const struct Base32TestVector base32TestVectors[] = {
    {"", "", ""},
    {"f", "MY======", "MY"},
    {"fo", "MZXQ====", "MZXQ"},
    {"foo", "MZXW6===", "MZXW6"},
    {"foob", "MZXW6YQ=", "MZXW6YQ"},
    {"fooba", "MZXW6YTB", "MZXW6YTB"},
    {"foobar", "MZXW6YTBOI======", "MZXW6YTBOI"},
};


// Helper function to compare error codes
void assert_base32_error(base32_error_t error_code, base32_error_t expected_error) {
    TEST_ASSERT_EQUAL(error_code, expected_error);
}

// Test encoding and decoding with default config (base32 alphabet, with padding)
void test_base32_encode_decode(void) {

    size_t output_size, output_length;
    char encoded[BUFFER_SIZE];
    uint8_t decoded[BUFFER_SIZE];

    base32_config_t config = {1, 0, 0, ""};  // Default config (with padding, no hex, no line breaks)
    base32_ctx_t *ctx;

    base32_init(&ctx, &config);

    // Run the tests for each vector
    for (size_t i = 0; i < sizeof(base32TestVectors) / sizeof(base32TestVectors[0]); i++) {
        // Encode
        base32_get_encode_size(strlen(base32TestVectors[i].input), ctx, &output_size);
        TEST_ASSERT_EQUAL(BASE32_SUCCESS, base32_encode(ctx, (const uint8_t *)base32TestVectors[i].input, strlen(base32TestVectors[i].input),
                                                       encoded, output_size, &output_length));
        encoded[output_length] = '\0';  // Null-terminate the encoded string
        TEST_ASSERT_EQUAL_STRING(base32TestVectors[i].encoded, encoded);

        // Decode
        base32_get_decode_size(output_length, ctx, &output_size);
        TEST_ASSERT_EQUAL(BASE32_SUCCESS, base32_decode(ctx, encoded, output_length, decoded, output_size, &output_length));
        decoded[output_length] = '\0';  // Null-terminate the decoded string
        TEST_ASSERT_EQUAL_STRING(base32TestVectors[i].input, (char *)decoded);
    }

    base32_free(ctx);

}

// Test encoding and decoding with hex-based config (base32hex alphabet)
void test_base32_encode_decode_hex(void) {
    size_t output_size, output_length;
    char encoded[BUFFER_SIZE];
    uint8_t decoded[BUFFER_SIZE];

    // Set config for base32hex
    base32_config_t hex_config = {1, 1, 0, ""};  // Use hex alphabet (with padding, hex-based, no line breaks)
    base32_ctx_t *ctx;
    base32_init(&ctx, &hex_config);

    // Run the tests for each vector
    for (size_t i = 0; i < sizeof(base32TestVectors) / sizeof(base32TestVectors[0]); i++) {
        // Encode
        base32_get_encode_size(strlen(base32TestVectors[i].input), ctx, &output_size);
        TEST_ASSERT_EQUAL(BASE32_SUCCESS, base32_encode(ctx, (const uint8_t *)base32TestVectors[i].input, strlen(base32TestVectors[i].input),
                                                       encoded, output_size, &output_length));
        encoded[output_length] = '\0';  // Null-terminate the encoded string
        TEST_ASSERT_EQUAL_STRING(base32TestVectors[i].encoded_hex, encoded);

        // Decode
        base32_get_decode_size(output_length, ctx, &output_size);
        TEST_ASSERT_EQUAL(BASE32_SUCCESS, base32_decode(ctx, encoded, output_length, decoded, output_size, &output_length));
        decoded[output_length] = '\0';  // Null-terminate the decoded string
        TEST_ASSERT_EQUAL_STRING(base32TestVectors[i].input, (char *)decoded);
    }
    base32_free(ctx);

}

// Test handling invalid inputs
void test_base32_invalid_inputs(void) {
    base32_config_t hex_config = {1, 1, 0, ""};  // Use hex alphabet (with padding, hex-based, no line breaks)
    base32_ctx_t *ctx;
    base32_init(&ctx, &hex_config);

    // Empty input with null pointer
    base32_error_t result = base32_encode(ctx, NULL, 0, NULL, 0, NULL);
    assert_base32_error(result, BASE32_ERROR_NULL_POINTER);

    // Invalid length
    char encoded[BUFFER_SIZE];
    uint8_t decoded[BUFFER_SIZE];
    size_t output_size, output_length;

    base32_get_encode_size(99999, ctx, &output_size);  // Invalid size
    result = base32_encode(ctx, (const uint8_t *)"test", 4, encoded, output_size, &output_length);
    assert_base32_error(result, BASE32_ERROR_BUFFER_TOO_SMALL);

    base32_get_decode_size(99999, ctx, &output_size);  // Invalid size
    result = base32_decode(ctx, "MZXW6===", 8, decoded, output_size, &output_length);
    assert_base32_error(result, BASE32_ERROR_BUFFER_TOO_SMALL);

    base32_free(ctx);

}