CC = gcc
CFLAGS = -I./include -Wall
LDFLAGS = -L./bin
LDFLAGS_SHARED = -L./bin
SRC = $(wildcard src/*.c)
SRC_HEADERS = $(wildcard src/*.h)
OUT = main
OUT_GAME = libgamelib

OUT_DIR = out

GAME_SRC = $(wildcard src/game/*.c)
GAME_HEADERS = $(wildcard src/game/*.h)

ifeq ($(OS),Windows_NT)
	OUT := $(OUT).exe
	OUT_GAME := $(OUT_GAME).dll
	LDFLAGS += -lraylib -lopengl32 -lgdi32 -lwinmm
	LDFLAGS_SHARED += -shared -lraylib -lopengl32 -lgdi32 -lwinmm
else
	OUT_GAME := $(OUT_GAME).dylib
	LDFLAGS += -lraylib.550 -Wl,-rpath,@loader_path/../bin -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo -framework OpenGL
	LDFLAGS_SHARED += -dynamiclib -lraylib.550 -Wl,-rpath,@loader_path/../bin -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo -framework OpenGL
endif

.PHONY: game

all: game main

make_dirs:
	mkdir -p out

out/libdialog.a: macos/dialog.m macos/dialog.h
	$(CC) -c macos/dialog.m -o out/dialog.o -ObjC -framework Cocoa
	ar rcs out/libdialog.a out/dialog.o

$(OUT_DIR)/$(OUT): $(SRC) $(SRC_HEADERS)
	$(CC) $(SRC) -o $(OUT_DIR)/$(OUT) $(CFLAGS) $(LDFLAGS)

ifeq ($(OS),Windows_NT)
$(OUT_DIR)/$(OUT_GAME): $(GAME_SRC) $(GAME_HEADERS) $(OUT_DIR)/raylib.dll
else
$(OUT_DIR)/$(OUT_GAME): $(GAME_SRC) $(GAME_HEADERS)
endif
	$(CC) $(GAME_SRC) -o $(OUT_DIR)/$(OUT_GAME) $(CFLAGS) $(LDFLAGS_SHARED) -ldialog -L./out

main: make_dirs
	make $(OUT_DIR)/$(OUT)

game: make_dirs out/libdialog.a
	make $(OUT_DIR)/$(OUT_GAME)

ifeq ($(OS),Windows_NT)

$(OUT_DIR)/raylib.dll:
	cmd /c copy .\bin\raylib.dll .\out\

endif

clean:
	rm -rf $(OUT_DIR)
