CC = clang
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g
SRC = src
OUT = out

.PHONY: all clean

all: $(OUT)/main

$(OUT)/main: $(OUT)/main.o $(OUT)/connect4.o | $(OUT)
	$(CC) $(CFLAGS) $^ -o $@

$(OUT)/main.o: $(SRC)/main.c | $(OUT)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/connect4.o: $(SRC)/connect4.c $(SRC)/ds.h | $(OUT)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT):
	mkdir -p $@

clean:
	rm -rf $(OUT)
