CC = gcc
CFLAGS = -I./include -Wall
LDFLAGS = -L./bin
LDFLAGS_SHARED = -L./bin
SRC = $(wildcard src/*.c)
SRC_HEADERS = $(wildcard src/*.h)
OUT = main
OUT_GAME = game

OUT_DIR = out

GAME_SRC = $(wildcard src/game/*.c)
GAME_HEADERS = $(wildcard src/game/*.h)

ifeq ($(OS),Windows_NT)
	OUT := $(OUT).exe
	OUT_GAME := $(OUT_GAME).dll
	LDFLAGS += -lraylib -lopengl32 -lgdi32 -lwinmm -lshlwapi -L./out -lplatform -lshlwapi -lcomdlg32
	LDFLAGS_SHARED += -shared -lraylib -lopengl32 -lgdi32 -lwinmm -lplatform -L./out -lcomdlg32 -lshlwapi
else
	OUT_GAME := $(OUT_GAME).dylib
	LDFLAGS += -lraylib.550 -Wl,-rpath,@loader_path/../bin -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo -framework OpenGL
	LDFLAGS_SHARED += -dynamiclib -lraylib.550 -Wl,-rpath,@loader_path/../bin -framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo -framework OpenGL
endif

.PHONY: game

all: game main

make_dirs:
	mkdir -p out

ifeq ($(OS),Windows_NT)
out/libplatform.a: src/platform/platform_win.c src/platform/platform.h
	$(CC) -c src/platform/platform_win.c -o out/libplatform.o
	ar rcs out/libplatform.a out/libplatform.o
else
out/libplatform.a: src/platform/platform_mac.c src/platform/platform.h
	$(CC) -c macos/dialog.m -o out/dialog.o -ObjC -framework Cocoa
	ar rcs out/libdialog.a out/dialog.o
endif

$(OUT_DIR)/$(OUT): $(SRC) $(SRC_HEADERS)
	$(CC) $(SRC) -o $(OUT_DIR)/$(OUT) $(CFLAGS) $(LDFLAGS)

ifeq ($(OS),Windows_NT)
$(OUT_DIR)/$(OUT_GAME): $(GAME_SRC) $(GAME_HEADERS) $(OUT_DIR)/raylib.dll $(OUT_DIR)/libplatform.a
else
$(OUT_DIR)/$(OUT_GAME): $(GAME_SRC) $(GAME_HEADERS)
endif
	$(CC) $(GAME_SRC) -o $(OUT_DIR)/$(OUT_GAME) $(CFLAGS) $(LDFLAGS_SHARED)

main: make_dirs
	make $(OUT_DIR)/$(OUT)

game: make_dirs out/libplatform.a
	make $(OUT_DIR)/$(OUT_GAME)

ifeq ($(OS),Windows_NT)

$(OUT_DIR)/raylib.dll:
	cmd /c copy .\bin\raylib.dll .\out\

endif

clean:
	rm -rf $(OUT_DIR)
