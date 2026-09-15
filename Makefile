CC = gcc
CFLAGS = -O0 -g
SRC = src
BIN = bin

TARGETS = $(BIN)/crackme01_strings $(BIN)/crackme02_xor $(BIN)/crackme03_antidebug $(BIN)/crackme04_packed $(BIN)/crackme05_crypto

all: $(BIN) $(TARGETS)

$(BIN):
	mkdir -p $(BIN)

$(BIN)/%: $(SRC)/%.c | $(BIN)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(BIN)

.PHONY: all clean
