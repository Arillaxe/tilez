CC = gcc
CFLAGS = -I./include
LDFLAGS = -L./bin -lraylib.550 -Wl,-rpath,@loader_path/../bin -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo -framework OpenGL
SRC = src/main.c
OUT = main
OUT_GAME = libgamelib.dylib

OUT_DIR = out

GAME_SRC = $(wildcard src/game/*.c)

.PHONY: game

all: game main

make_dirs:
	mkdir -p out

$(OUT_DIR)/$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT_DIR)/$(OUT) $(CFLAGS) $(LDFLAGS)

$(OUT_DIR)/$(OUT_GAME): $(GAME_SRC)
	$(CC) -dynamiclib $(GAME_SRC) -o $(OUT_DIR)/$(OUT_GAME) $(CFLAGS) $(LDFLAGS)

main: make_dirs
	make $(OUT_DIR)/$(OUT)

game: make_dirs
	make $(OUT_DIR)/$(OUT_GAME)

clean:
	rm -rf $(OUT_DIR)
