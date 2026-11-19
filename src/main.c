/*
 * main.c - Interactive menu: encrypt a message or decrypt the last one.
 *
 * Revised version of the original PjBL-2 project (see original/).
 * Files written to the current directory:
 *   cipher.txt - encrypted message, as hex (safe for any byte value)
 *   key.txt    - key length on the first line, then one key byte per line
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cipher.h"

#define MAX_MESSAGE 1000
#define CIPHER_FILE "cipher.txt"
#define KEY_FILE "key.txt"

/* Reads one line from stdin without the trailing newline. Returns 0 on EOF. */
static int read_line(char *buffer, size_t size)
{
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }
    buffer[strcspn(buffer, "\r\n")] = '\0';
    return 1;
}

/* Reads a menu option. Returns -1 on EOF, 0 on invalid input. */
static int read_option(void)
{
    char line[32];

    if (!read_line(line, sizeof line)) {
        return -1;
    }
    return atoi(line);
}

static int save_key(const unsigned char *key, size_t key_len)
{
    FILE *file = fopen(KEY_FILE, "w");

    if (file == NULL) {
        perror("Could not create " KEY_FILE);
        return 0;
    }
    fprintf(file, "%zu\n", key_len);
    for (size_t i = 0; i < key_len; i++) {
        fprintf(file, "%d\n", key[i]);
    }
    fclose(file);
    return 1;
}

/* Loads the key. Returns its length, or 0 if the file is missing or invalid. */
static size_t load_key(unsigned char *key)
{
    FILE *file = fopen(KEY_FILE, "r");
    size_t key_len = 0;

    if (file == NULL) {
        return 0;
    }
    if (fscanf(file, "%zu", &key_len) != 1 || key_len < KEY_MIN_LEN || key_len > KEY_MAX_LEN) {
        fclose(file);
        return 0;
    }
    for (size_t i = 0; i < key_len; i++) {
        int value;

        if (fscanf(file, "%d", &value) != 1 || value < 0 || value > 255) {
            fclose(file);
            return 0;
        }
        key[i] = (unsigned char)value;
    }
    fclose(file);
    return key_len;
}

static void encrypt_message(void)
{
    char message[MAX_MESSAGE + 2];
    unsigned char encrypted[MAX_MESSAGE];
    char hex[2 * MAX_MESSAGE + 1];
    unsigned char key[KEY_MAX_LEN];
    size_t key_len = (size_t)(rand() % (KEY_MAX_LEN - KEY_MIN_LEN + 1)) + KEY_MIN_LEN;
    size_t len;
    FILE *file;

    printf("\n=*=*=*=*=*=*=* ENCRYPT *=*=*=*=*=*=*=");
    printf("\nType the message to encrypt (up to %d characters): ", MAX_MESSAGE);
    if (!read_line(message, sizeof message)) {
        return;
    }
    len = strlen(message);
    if (len == 0 || len > MAX_MESSAGE) {
        printf("The message must have between 1 and %d characters.\n", MAX_MESSAGE);
        return;
    }

    generate_key(key, key_len);
    encrypt_bytes((const unsigned char *)message, encrypted, len, key, key_len);
    to_hex(encrypted, len, hex);

    file = fopen(CIPHER_FILE, "w");
    if (file == NULL) {
        perror("Could not create " CIPHER_FILE);
        return;
    }
    fprintf(file, "%s\n", hex);
    fclose(file);

    if (save_key(key, key_len)) {
        printf("Encrypted: %s\n", hex);
        printf("Saved to " CIPHER_FILE " and " KEY_FILE ".\n");
    }
}

static void decrypt_message(void)
{
    char hex[2 * MAX_MESSAGE + 2];
    unsigned char encrypted[MAX_MESSAGE];
    char message[MAX_MESSAGE + 1];
    unsigned char key[KEY_MAX_LEN];
    size_t key_len = load_key(key);
    long len;
    FILE *file;

    printf("\n=*=*=*=*=*=*=* DECRYPT *=*=*=*=*=*=*=");
    if (key_len == 0) {
        printf("\nNo valid key found. Encrypt a message first.\n");
        return;
    }

    file = fopen(CIPHER_FILE, "r");
    if (file == NULL || fgets(hex, sizeof hex, file) == NULL) {
        printf("\nNo encrypted message found. Encrypt a message first.\n");
        if (file != NULL) {
            fclose(file);
        }
        return;
    }
    fclose(file);
    hex[strcspn(hex, "\r\n")] = '\0';

    len = from_hex(hex, encrypted);
    if (len < 0 || len > MAX_MESSAGE) {
        printf("\n" CIPHER_FILE " is corrupted.\n");
        return;
    }
    decrypt_bytes(encrypted, (unsigned char *)message, (size_t)len, key, key_len);
    message[len] = '\0';
    printf("\nDecrypted: %s\n", message);
}

int main(void)
{
    srand((unsigned int)time(NULL));

    for (;;) {
        int option;

        printf("\n=*=*=*=*=*=*=* MENU *=*=*=*=*=*=*=");
        printf("\n<1> Encrypt a message\n<2> Decrypt the last message\n<3> Quit");
        printf("\nChoose an option: ");

        option = read_option();
        switch (option) {
        case 1:
            encrypt_message();
            break;
        case 2:
            decrypt_message();
            break;
        case 3:
        case -1:
            printf("\nGoodbye!\n");
            return 0;
        default:
            printf("Invalid option. Type 1, 2 or 3.\n");
        }
    }
}
