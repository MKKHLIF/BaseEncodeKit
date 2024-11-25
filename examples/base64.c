#include <stdio.h>
#include <string.h>

#include "base64.h"


int main(int argc, char *argv[]) {
    base64_ctx_t *ctx;
    base64_config_t config = {
        .use_padding = 1,
        .url_safe = 0,
        .line_length = 76,
        .line_ending = "\n"
    };

    // Initialize context
    base64_init(&ctx, &config);

    // Prepare buffers
    uint8_t input[] = "Hello, World!";
    char encoded[100];
    uint8_t decoded[100];
    size_t encoded_len, decoded_len;

    // Encode
    base64_encode(ctx, input, strlen((char *) input),
                  encoded, sizeof(encoded), &encoded_len);

    printf("Encoded: %s\n", encoded);

    // Decode
    base64_decode(ctx, encoded, encoded_len,
                  decoded, sizeof(decoded), &decoded_len);

    printf("Decoded: %s\n", decoded);
    // Free context when done
    base64_free(ctx);

    printf("\n");
    return 0;
}
