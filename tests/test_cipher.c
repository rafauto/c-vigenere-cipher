/*
 * test_cipher.c - Unit tests for the pure cipher functions (no framework).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/cipher.h"

static int failures = 0;

#define CHECK(condition, name)                        \
    do {                                              \
        if (condition) {                              \
            printf("PASS  %s\n", name);               \
        } else {                                      \
            printf("FAIL  %s\n", name);               \
            failures++;                               \
        }                                             \
    } while (0)

static void test_round_trip_text(void)
{
    const char *text = "Hello, world! Messages longer than the key wrap around it.";
    const unsigned char key[] = {10, 200, 33, 255, 0};
    size_t len = strlen(text);
    unsigned char encrypted[128];
    unsigned char decrypted[128];

    encrypt_bytes((const unsigned char *)text, encrypted, len, key, sizeof key);
    decrypt_bytes(encrypted, decrypted, len, key, sizeof key);
    CHECK(memcmp(text, decrypted, len) == 0, "round trip restores the text");
    CHECK(memcmp(text, encrypted, len) != 0, "ciphertext differs from plaintext");
}

static void test_known_vector(void)
{
    const unsigned char in[] = {'A', 'B', 'C', 'D'};
    const unsigned char key[] = {1, 2, 3};
    const unsigned char expected[] = {'B', 'D', 'F', 'E'};
    unsigned char out[4];

    encrypt_bytes(in, out, sizeof in, key, sizeof key);
    CHECK(memcmp(out, expected, sizeof expected) == 0, "key repeats over the message");
}

static void test_wraps_modulo_256(void)
{
    const unsigned char in[] = {250};
    const unsigned char key[] = {10};
    unsigned char out[1];
    unsigned char back[1];

    encrypt_bytes(in, out, 1, key, 1);
    decrypt_bytes(out, back, 1, key, 1);
    CHECK(out[0] == 4 && back[0] == 250, "byte arithmetic wraps modulo 256");
}

static void test_all_bytes_round_trip(void)
{
    unsigned char in[256];
    unsigned char enc[256];
    unsigned char dec[256];
    char hex[513];
    unsigned char parsed[256];
    unsigned char key[7];

    for (int i = 0; i < 256; i++) {
        in[i] = (unsigned char)i;
    }
    srand(42);
    generate_key(key, sizeof key);
    encrypt_bytes(in, enc, 256, key, sizeof key);
    to_hex(enc, 256, hex);
    CHECK(from_hex(hex, parsed) == 256, "hex decodes every byte");
    decrypt_bytes(parsed, dec, 256, key, sizeof key);
    CHECK(memcmp(in, dec, 256) == 0, "all 256 byte values survive encrypt + hex");
}

static void test_hex(void)
{
    const unsigned char bytes[] = {0x00, 0x0a, 0xff, 0x20};
    char hex[9];
    unsigned char back[4];

    to_hex(bytes, sizeof bytes, hex);
    CHECK(strcmp(hex, "000aff20") == 0, "to_hex writes lowercase hex");
    CHECK(from_hex("000AFF20", back) == 4 && memcmp(back, bytes, 4) == 0,
          "from_hex accepts uppercase");
    CHECK(from_hex("abc", back) == -1, "from_hex rejects odd length");
    CHECK(from_hex("zz", back) == -1, "from_hex rejects non-hex characters");
    CHECK(from_hex("", back) == 0, "from_hex accepts empty string");
}

int main(void)
{
    test_round_trip_text();
    test_known_vector();
    test_wraps_modulo_256();
    test_all_bytes_round_trip();
    test_hex();

    if (failures > 0) {
        printf("\n%d test(s) failed.\n", failures);
        return EXIT_FAILURE;
    }
    printf("\nAll tests passed.\n");
    return EXIT_SUCCESS;
}
