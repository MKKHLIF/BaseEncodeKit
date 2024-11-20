#include "unity.h"
#include "base64.h"

#include <stdlib.h>
#include <string.h>

void setUp(void) {
}

void tearDown(void) {
}

void assert_string_equal(const char *expected, const char *actual) {
    TEST_ASSERT_EQUAL_STRING(expected, actual);
}

void test_base64_encode_decode(void) {
    // Test vectors for Base64 encoding and decoding
    struct TestVector {
        const char *decoding;
        const char *encoding;
    };

    const struct TestVector testVectors[] = {
        {"", ""},
        {"f", "Zg=="},
        {"fo", "Zm8="},
        {"foo", "Zm9v"},
        {"foob", "Zm9vYg=="},
        {"fooba", "Zm9vYmE="},
        {"foobar", "Zm9vYmFy"}
    };

    for (size_t i = 0; i < sizeof(testVectors) / sizeof(testVectors[0]); ++i) {
        char encoded[64];
        ssize_t enc_len = base64_encode(encoded, sizeof(encoded), (const uint8_t *) testVectors[i].decoding,
                                        strlen(testVectors[i].decoding));
        encoded[enc_len] = '\0'; // Null-terminate the encoded string
        assert_string_equal(testVectors[i].encoding, encoded);

        // Assuming you have a decoding function, you can test it similarly:
        // For example, if you had base64_decode function, you could write:
        char decoded[64];
        ssize_t dec_len = base64_decode(decoded, sizeof(decoded), encoded, enc_len);
        decoded[dec_len] = '\0';
        assert_string_equal(testVectors[i].decoding, decoded);
    }
}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_base64_encode_decode);

    return UNITY_END();
}
