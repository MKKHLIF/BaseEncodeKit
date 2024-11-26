#include "unity.h"
#include "base64.h"
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128  // Example buffer size, adjust as needed

struct TestVector {
    const char *input;
    const char *encoded;
};

// Test vectors without URL support
const struct TestVector testVectors[] = {
    {"", ""},
    {"f", "Zg=="},
    {"fo", "Zm8="},
    {"foo", "Zm9v"},
    {"foob", "Zm9vYg=="},
    {"fooba", "Zm9vYmE="},
    {"foobar", "Zm9vYmFy"}
};

// Test vectors with URL support
const struct TestVector testVectorsUrl[] = {
    {"", ""},
    {"\xff", "_w"},
    {"f\xff", "Zv8"},
    {"fo\xff", "Zm__"},
    {"foo\xff", "Zm9v_w"},
    {"foob\xff", "Zm9vYv8"},
    {"fooba\xff", "Zm9vYmH_"},
    {"foobar\xff", "Zm9vYmFy_w"},
};

base64_ctx_t *ctx;

// Helper function to compare error codes
void assert_base64_error(base64_error_t error_code, base64_error_t expected_error) {
    TEST_ASSERT_EQUAL(error_code, expected_error);
}

void setUp(void) {

}

void tearDown(void) {
    base64_free(ctx);
}

void test_base64_encode_decode(void) {

    base64_config_t config = {1, 0, 0, ""};  // Default config (with padding, not URL safe, no line breaks)
    base64_init(&ctx, &config);

    size_t output_size, output_length;
    char encoded[BUFFER_SIZE];
    uint8_t decoded[BUFFER_SIZE];

    // Run the tests for each vector
    for (size_t i = 0; i < sizeof(testVectors) / sizeof(testVectors[0]); i++) {
        // Encode
        base64_get_encode_size(strlen(testVectors[i].input), ctx, &output_size);
        TEST_ASSERT_EQUAL(BASE64_SUCCESS, base64_encode(ctx, (const uint8_t *)testVectors[i].input, strlen(testVectors[i].input),
                                                       encoded, output_size, &output_length));
        encoded[output_length] = '\0';  // Null-terminate the encoded string
        TEST_ASSERT_EQUAL_STRING(testVectors[i].encoded, encoded);

        // Decode
        base64_get_decode_size(output_length, ctx, &output_size);
        TEST_ASSERT_EQUAL(BASE64_SUCCESS, base64_decode(ctx, encoded, output_length, decoded, output_size, &output_length));
        decoded[output_length] = '\0';  // Null-terminate the decoded string
        TEST_ASSERT_EQUAL_STRING(testVectors[i].input, (char *)decoded);
    }
}

// Test encoding and decoding with URL support
void test_base64_encode_decode_url_safe(void) {
    size_t output_size, output_length;
    char encoded[BUFFER_SIZE];
    uint8_t decoded[BUFFER_SIZE];

    base64_config_t url_safe_config = {1, 1, 0, ""};  // URL-safe config (with padding, URL safe, no line breaks)
    base64_init(&ctx, &url_safe_config);

    // Run the tests for each vector
    for (size_t i = 0; i < sizeof(testVectorsUrl) / sizeof(testVectorsUrl[0]); i++) {
        // Encode
        base64_get_encode_size(strlen(testVectorsUrl[i].input), ctx, &output_size);
        TEST_ASSERT_EQUAL(BASE64_SUCCESS, base64_encode(ctx, (const uint8_t *)testVectorsUrl[i].input, strlen(testVectorsUrl[i].input),
                                                       encoded, output_size, &output_length));
        encoded[output_length] = '\0';  // Null-terminate the encoded string
        TEST_ASSERT_EQUAL_STRING(testVectorsUrl[i].encoded, encoded);

        // Decode
        base64_get_decode_size(output_length, ctx, &output_size);
        TEST_ASSERT_EQUAL(BASE64_SUCCESS, base64_decode(ctx, encoded, output_length, decoded, output_size, &output_length));
        decoded[output_length] = '\0';  // Null-terminate the decoded string
        TEST_ASSERT_EQUAL_STRING(testVectorsUrl[i].input, (char *)decoded);
    }
}

// Test handling invalid inputs
void test_base64_invalid_inputs(void) {
    // Empty input with null pointer
    base64_error_t result = base64_encode(ctx, NULL, 0, NULL, 0, NULL);
    assert_base64_error(result, BASE64_ERROR_NULL_POINTER);

    // Invalid length
    char encoded[BUFFER_SIZE];
    uint8_t decoded[BUFFER_SIZE];
    size_t output_size, output_length;

    base64_get_encode_size(99999, ctx, &output_size);  // Invalid size
    result = base64_encode(ctx, (const uint8_t *)"test", 4, encoded, output_size, &output_length);
    assert_base64_error(result, BASE64_ERROR_BUFFER_TOO_SMALL);

    base64_get_decode_size(99999, ctx, &output_size);  // Invalid size
    result = base64_decode(ctx, "Zm9v", 4, decoded, output_size, &output_length);
    assert_base64_error(result, BASE64_ERROR_BUFFER_TOO_SMALL);
}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_base64_encode_decode);
    RUN_TEST(test_base64_encode_decode_url_safe);

    return UNITY_END();
}
