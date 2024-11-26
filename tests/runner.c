#include "unity.h"

extern void test_base64_encode_decode(void);
extern void test_base64_encode_decode_url_safe(void);
extern void test_base64_invalid_inputs(void);

extern void test_base32_encode_decode(void);
extern void test_base32_encode_decode_hex(void);
extern void test_base32_invalid_inputs(void);

extern void test_base16_encode(void);
extern void test_base16_decode(void);

void setUp(void) {
}

void tearDown(void) {
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_base64_encode_decode);
    // RUN_TEST(test_base64_encode_decode_url_safe);
    // RUN_TEST(test_base64_invalid_inputs);

    // RUN_TEST(test_base32_encode_decode);
    // RUN_TEST(test_base32_encode_decode_hex);

    RUN_TEST(test_base16_encode);
    RUN_TEST(test_base16_decode);

    return UNITY_END();
}
