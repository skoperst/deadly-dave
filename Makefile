CC = gcc
BIN = ddave

H_FILES := include/bullet.h
H_FILES += include/dave.h
H_FILES += include/game.h
H_FILES += include/input.h
H_FILES += include/invfreq.h
H_FILES += include/monster.h
H_FILES += include/plasma.h
H_FILES += include/soundfx.h
H_FILES += include/tile.h

C_FILES := main.c
C_FILES += game.c
C_FILES += tile.c
C_FILES += dave.c
C_FILES += bullet.c
C_FILES += monster.c
C_FILES += plasma.c
C_FILES += invfreq.c
C_FILES += soundfx.c



CFLAGS = -rdynamic -std=c99 -Wall
CFLAGS += $(shell pkg-config --cflags sdl2 SDL2_image)
LIBS := $(shell pkg-config --libs sdl2 SDL2_image)

all: $(C_FILES) $(H_FILES)
	$(CC) $(C_FILES) $(CFLAGS) -Iinclude $(LIBS) -o $(BIN)

clean:
	rm -f $(BIN)
