CC = gcc
INCS = -I /usr/local/Cellar/sdl2/2.0.12_1/include/SDL2
INCS += -I /usr/include/SDL2
LIBS = -L /usr/local/Cellar/sdl2/2.0.12_1/lib

CFLAGS = -g -std=gnu89 -Wall
LFLAGS = -lSDL2main -lSDL2

SRC = game.c tile.c font.c dave.c
OBJ = ddave

all: $(SRC)
	$(CC) $(SRC) $(INCS) $(LIBS) $(CFLAGS) $(LFLAGS) -o $(OBJ)
