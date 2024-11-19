#include <stdio.h>
#include <string.h>

#include "base64.h"


int main(int argc, char *argv[]) {
    const char *test_data = "Hello, World!";
    const size_t test_len = strlen(test_data);

    const size_t buf_sz = base64_buff_sz(test_len);
    char buffer[buf_sz];
    const ssize_t written = base64_encode(buffer, buf_sz,
                                          (const unsigned char *) test_data, test_len);
    if (written >= 0) {
        printf("Encoded (buffer): %s\n", buffer);
    } else {
        printf("Encoding failed\n");
    }


    printf("\n");
    return 0;
}
