#include <stdio.h>
#include <string.h>

#include "base32.h"


int main(int argc, char *argv[]) {
    base32_ctx_t *ctx;
    base32_config_t config = {
        .use_padding = 1,
        .use_hex = 0,
        .line_length = 76,
        .line_ending = "\n"
    };

    base32_init(&ctx, &config);

    // Prepare buffers
    uint8_t input[] = "Hello, World!";
    char encoded[100];
    uint8_t decoded[100];
    size_t encoded_len, decoded_len;

    // Encode
    base32_encode(ctx, input, strlen((char *) input),
                  encoded, sizeof(encoded), &encoded_len);

    printf("Encoded: %s\n", encoded);

    // Decode
    base32_decode(ctx, encoded, encoded_len,
                  decoded, sizeof(decoded), &decoded_len);

    printf("Decoded: %s\n", decoded);
    // Free context when done
    base32_free(ctx);

    printf("\n");
    return 0;
}
