CC = clang
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = src
OUT = dist

.PHONY: all clean

all: $(OUT)/main $(OUT)/librandom.so $(OUT)/libplayer.so $(OUT)/libminmax.so

$(OUT)/main: $(OUT)/main.o $(OUT)/connect4.o | $(OUT)
	$(CC) $(CFLAGS) $^ -o $@

$(OUT)/main.o: $(SRC)/main.c | $(OUT)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/connect4.o: $(SRC)/connect4.c $(SRC)/ds.h | $(OUT)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT)/librandom.so: $(OUT)/random.o $(OUT)/connect4.o | $(OUT)
	$(CC) -shared -fPIC $^ -o $@

$(OUT)/random.o: $(SRC)/random.c | $(OUT)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(OUT)/libplayer.so: $(OUT)/player.o $(OUT)/connect4.o | $(OUT)
	$(CC) -shared -fPIC $^ -o $@

$(OUT)/player.o: $(SRC)/player.c | $(OUT)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(OUT)/libminmax.so: $(OUT)/minmax.o $(OUT)/connect4.o | $(OUT)
	$(CC) -shared -fPIC $^ -o $@

$(OUT)/minmax.o: $(SRC)/minmax.c | $(OUT)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(OUT):
	mkdir -p $@

clean:
	rm -rf $(OUT)
