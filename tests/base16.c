#include <unity.h>
#include "base16.h"

#include <string.h>

struct Base16TestVector {
    const char *input;
    const char *encoded_lower;
    const char *encoded_upper;
};

// Define test vectors for base16 encoding (both lowercase and uppercase)
const struct Base16TestVector base16TestVectors[] = {
    {"", "", ""},
    {"f", "66", "66"},
    {"foo", "666F6F", "666F6F"},
    {"hello", "68656C6C6F", "68656C6C6F"},
    {"abc123", "616263313233", "616263313233"},
    {"base16", "626173653136", "626173653136"}
};

// Helper function to assert that the base16 error matches expected error
void assert_base16_error(base16_error_t error_code, base16_error_t expected_error) {
    TEST_ASSERT_EQUAL(error_code, expected_error);
}

// Unit tests for encoding and decoding base16
void test_base16_encode(void) {

    base16_config_t config = {1, 0, 0, 0};  // Default config (lowercase, no line breaks)
    base16_ctx_t *ctx;
    base16_init(&ctx, &config);

    for (int i = 0; i < sizeof(base16TestVectors) / sizeof(base16TestVectors[0]); i++) {
        const struct Base16TestVector *tv = &base16TestVectors[i];
        char encoded_output[128];
        size_t output_length = 0;

        // Encode input with lowercase
        base16_error_t err = base16_encode(ctx, (const uint8_t*)tv->input, strlen(tv->input), encoded_output, sizeof(encoded_output), &output_length);
        assert_base16_error(err, BASE16_SUCCESS);
        encoded_output[output_length] = '\0';  // Null-terminate the output
        TEST_ASSERT_EQUAL_STRING(tv->encoded_lower, encoded_output);

        // Encode input with uppercase
        base16_config_t config_upper = {1, 0, 0, 0};  // Config for uppercase
        base16_init(&ctx, &config_upper);
        err = base16_encode(ctx, (const uint8_t*)tv->input, strlen(tv->input), encoded_output, sizeof(encoded_output), &output_length);
        assert_base16_error(err, BASE16_SUCCESS);
        encoded_output[output_length] = '\0';  // Null-terminate the output
        TEST_ASSERT_EQUAL_STRING(tv->encoded_upper, encoded_output);
    }
    base16_free(ctx);
}

void test_base16_decode(void) {

    base16_config_t config = {1, 0, 0, 0};  // Default config (lowercase, no line breaks)
    base16_ctx_t *ctx;
    base16_init(&ctx, &config);

    for (int i = 0; i < sizeof(base16TestVectors) / sizeof(base16TestVectors[0]); i++) {
        const struct Base16TestVector *tv = &base16TestVectors[i];
        uint8_t decoded_output[128];
        size_t output_length = 0;

        // Decode the lowercase encoded string
        base16_error_t err = base16_decode(ctx, tv->encoded_lower, strlen(tv->encoded_lower), decoded_output, sizeof(decoded_output), &output_length);
        assert_base16_error(err, BASE16_SUCCESS);
        decoded_output[output_length] = '\0';  // Null-terminate the output
        TEST_ASSERT_EQUAL_STRING(tv->input, (char*)decoded_output);

        // Decode the uppercase encoded string
        base16_config_t config_upper = {1, 0, 0, 0};  // Config for uppercase
        base16_init(&ctx, &config_upper);
        err = base16_decode(ctx, tv->encoded_upper, strlen(tv->encoded_upper), decoded_output, sizeof(decoded_output), &output_length);
        assert_base16_error(err, BASE16_SUCCESS);
        decoded_output[output_length] = '\0';  // Null-terminate the output
        TEST_ASSERT_EQUAL_STRING(tv->input, (char*)decoded_output);
    }
    base16_free(ctx);
}

