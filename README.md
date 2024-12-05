# BaseEncodeKit - RFC4648

This C library provides an implementation of the Base Encoding schemes defined in [RFC 4648](https://tools.ietf.org/html/rfc4648), which includes the Base64, Base32, and Base16 (Hex) encoding and decoding algorithms.

## Key Features

### Encoding & Decoding Capabilities

- **Base64 Encoding & Decoding**
   - **Standard Variant**: Encode and decode data using the standard Base64 alphabet.
   - **URL-Safe Variant**: Encode and decode data using a URL-safe Base64 alphabet to ensure safe data transmission in URLs.

- **Base32 Encoding & Decoding**
   - **Standard Variant**: Encode and decode data using the standard Base32 alphabet.
   - **Crockford Variant**: Utilize the Crockford Base32 alphabet for encoding and decoding, which minimizes confusion between similar-looking characters.

- **Base16 (Hex) Encoding & Decoding**
   - **Hexadecimal Strings**: Seamlessly convert data to and from hexadecimal representations.


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
