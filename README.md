# BaseEncodeKit - RFC4648

This C library provides an implementation of the Base Encoding schemes defined in [RFC 4648](https://tools.ietf.org/html/rfc4648), which includes the Base64, Base32, and Base16 (Hex) encoding and decoding algorithms.

## Features
- **Base64 Encoding & Decoding**: Encode and decode data using the Base64 alphabet (standard and URL-safe variants).
- **Base32 Encoding & Decoding**: Encode and decode data using the Base32 alphabet (standard and Crockford variants).
- **Base16 (Hex) Encoding & Decoding**: Encode and decode data as hexadecimal strings.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/MKKHLIF/BaseEncodeKit.git
   cd BaseEncodeKit
    ```

2. Create a build directory and run CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```
3. Build the library:
   ```bash
   make
   ```
## Usage

### Base64 Encoding/Decoding Example:

```c
   #include "base64.h"

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

```

## License
Licensed under the [MIT license](LICENSE.md).
