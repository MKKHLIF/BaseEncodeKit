# BaseEncodeKit - RFC4648

This C library provides an implementation of the Base Encoding schemes defined in [RFC 4648](https://tools.ietf.org/html/rfc4648), which includes the Base64, Base32, and Base16 (Hex) encoding and decoding algorithms.

## Features
- **Base64 Encoding & Decoding**: Encode and decode data using the Base64 alphabet (standard and URL-safe variants).
- **Base32 Encoding & Decoding**: Encode and decode data using the Base32 alphabet (standard and Crockford variants).
- **Base16 (Hex) Encoding & Decoding**: Encode and decode data as hexadecimal strings.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/rfc4648-c-library.git
   cd rfc4648-c-library
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

### Base64 Encoding Example:

```c
#include "rfc4648.h"

const char *data = "Hello, World!";
char *encoded = base64_encode((unsigned char*)data, strlen(data));
printf("Encoded: %s\n", encoded);
free(encoded);

```
### Base64 Decoding Example:

```c
#include "rfc4648.h"

const char *encoded = "SGVsbG8sIFdvcmxkIQ==";
unsigned char *decoded = base64_decode(encoded, strlen(encoded));
printf("Decoded: %s\n", decoded);
free(decoded);
```

## License
Licensed under the [MIT license](LICENSE.md).
