#include "unity.h"
#include "base64.h"

#include <stdlib.h>

void setUp(void) {
}

void tearDown(void) {
}


void test_base64_encoding(void) {
    char *encoded = base64_encode("Hello, World!", 13);
    TEST_ASSERT_EQUAL_STRING("SGVsbG8sIFdvcmxkIQ==", encoded);
    free(encoded);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_base64_encoding);

    return UNITY_END();
}
