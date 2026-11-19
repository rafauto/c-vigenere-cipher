/*
 * cipher.h - Polyalphabetic (Vigenere-style) byte cipher with a random key.
 *
 * Each byte of the message is shifted by the key byte at the same position
 * (the key repeats when the message is longer than the key), modulo 256.
 */
#ifndef CIPHER_H
#define CIPHER_H

#include <stddef.h>

#define KEY_MIN_LEN 3
#define KEY_MAX_LEN 7

/* Fills `key` with `len` random bytes. Call srand() once before using it. */
void generate_key(unsigned char *key, size_t len);

/* Encrypts `len` bytes of `in` into `out` (same size). */
void encrypt_bytes(const unsigned char *in, unsigned char *out, size_t len,
                   const unsigned char *key, size_t key_len);

/* Decrypts `len` bytes of `in` into `out` (same size). */
void decrypt_bytes(const unsigned char *in, unsigned char *out, size_t len,
                   const unsigned char *key, size_t key_len);

/* Writes `len` bytes as lowercase hex into `hex` (needs 2 * len + 1 bytes). */
void to_hex(const unsigned char *bytes, size_t len, char *hex);

/*
 * Parses a hex string into `bytes` (needs strlen(hex) / 2 bytes).
 * Returns the number of bytes written, or -1 if the string is not valid hex.
 */
long from_hex(const char *hex, unsigned char *bytes);

#endif /* CIPHER_H */
