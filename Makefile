CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2

BIN := cipher
TEST_BIN := test_cipher

.PHONY: all test smoke original clean

all: $(BIN)

$(BIN): src/main.c src/cipher.c src/cipher.h
	$(CC) $(CFLAGS) -o $@ src/main.c src/cipher.c

$(TEST_BIN): tests/test_cipher.c src/cipher.c src/cipher.h
	$(CC) $(CFLAGS) -o $@ tests/test_cipher.c src/cipher.c

test: $(TEST_BIN)
	./$(TEST_BIN)

# End-to-end run of the menu: encrypt, decrypt, quit, then check the output.
smoke: $(BIN)
	printf '1\nSmoke test: 123 !@# with spaces\n2\n3\n' | ./$(BIN) | tee smoke.log
	grep -q 'Decrypted: Smoke test: 123 !@# with spaces' smoke.log
	rm -f smoke.log cipher.txt key.txt

# The original course version needs a lenient, older C standard to compile.
original:
	$(CC) -std=gnu89 -w -o cipher_original original/cripto_descrito.c

clean:
	rm -f $(BIN) $(TEST_BIN) cipher_original smoke.log cipher.txt key.txt \
		arquivo-cripto.txt arquivo-chave.txt
