#include <stdio.h>
#include <string.h>

#include "base16.h"


int main(int argc, char *argv[]) {
    base16_ctx_t *ctx;
    base16_config_t config = {
        .uppercase = 1,
        .line_length = 76,
        .line_ending = "\n"
    };

    base16_init(&ctx, &config);

    // Prepare buffers
    uint8_t input[] = "Hello, World!";
    char encoded[100];
    uint8_t decoded[100];
    size_t encoded_len, decoded_len;

    // Encode
    base16_encode(ctx, input, strlen((char *) input),
                  encoded, sizeof(encoded), &encoded_len);

    printf("Encoded: %s\n", encoded);

    // Decode
    base16_decode(ctx, encoded, encoded_len,
                  decoded, sizeof(decoded), &decoded_len);

    printf("Decoded: %s\n", decoded);
    // Free context when done
    base16_free(ctx);

    printf("\n");
    return 0;
}
