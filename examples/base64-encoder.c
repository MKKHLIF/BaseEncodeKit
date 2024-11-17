#include <stdio.h>
#include "base64.h"


int main(int argc, char *argv[]) {
    const char *input = "f";

    char *encoded = base64_encode(input, 1);
    printf("Encoded: %s\n", encoded);

    printf("\n");
    return 0;
}
