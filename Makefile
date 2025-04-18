CC = clang
CFLAGS = -Wall -Wextra -pedantic -std=c11
SRC = src
OUT = out

.PHONY: all clean

all: $(OUT)/main

$(OUT)/main: $(OUT)/main.o | $(OUT)
	$(CC) $(CFLAGS) $< -o $@

$(OUT)/main.o: $(SRC)/main.c | $(OUT)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT):
	mkdir -p $@

clean:
	rm -rf $(OUT)
