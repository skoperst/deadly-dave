CC = gcc
INCS = -I /usr/local/Cellar/sdl2/2.0.12_1/include/SDL2
INCS += -I /usr/include/SDL2
INCS += -I include/
LIBS = -L /usr/local/Cellar/sdl2/2.0.12_1/lib

CFLAGS = -rdynamic -std=gnu89 -Wall
LFLAGS = -lSDL2main -lSDL2

SRC = main.c game.c tile.c dave.c bullet.c monster.c plasma.c invfreq.c soundfx.c
OBJ = ddave

all: $(SRC)
	$(CC) $(SRC) $(INCS) $(LIBS) $(CFLAGS) $(LFLAGS) -o $(OBJ)
