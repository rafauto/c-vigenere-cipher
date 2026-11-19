/*
 * cipher.c - Pure cipher functions (no I/O), so they can be unit tested.
 */
#include "cipher.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void generate_key(unsigned char *key, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        key[i] = (unsigned char)(rand() % 256);
    }
}

void encrypt_bytes(const unsigned char *in, unsigned char *out, size_t len,
                   const unsigned char *key, size_t key_len)
{
    for (size_t i = 0; i < len; i++) {
        out[i] = (unsigned char)(in[i] + key[i % key_len]);
    }
}

void decrypt_bytes(const unsigned char *in, unsigned char *out, size_t len,
                   const unsigned char *key, size_t key_len)
{
    for (size_t i = 0; i < len; i++) {
        out[i] = (unsigned char)(in[i] - key[i % key_len]);
    }
}

void to_hex(const unsigned char *bytes, size_t len, char *hex)
{
    static const char digits[] = "0123456789abcdef";

    for (size_t i = 0; i < len; i++) {
        hex[2 * i] = digits[bytes[i] >> 4];
        hex[2 * i + 1] = digits[bytes[i] & 0x0F];
    }
    hex[2 * len] = '\0';
}

static int hex_value(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    c = (char)tolower((unsigned char)c);
    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return -1;
}

long from_hex(const char *hex, unsigned char *bytes)
{
    size_t len = strlen(hex);

    if (len % 2 != 0) {
        return -1;
    }
    for (size_t i = 0; i < len / 2; i++) {
        int high = hex_value(hex[2 * i]);
        int low = hex_value(hex[2 * i + 1]);

        if (high < 0 || low < 0) {
            return -1;
        }
        bytes[i] = (unsigned char)((high << 4) | low);
    }
    return (long)(len / 2);
}
