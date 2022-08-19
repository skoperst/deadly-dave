CC = gcc

CFLAGS = -g -std=gnu89 -Wall `sdl2-config --cflags` -DHAVE_SDL2
LDFLAGS += `sdl2-config --libs`
INCS = -I ../include/

all: test_invfreq test_monster

test_invfreq: test_invfreq.c ../invfreq.c
	$(CC) test_invfreq.c ../invfreq.c $(INCS) $(CFLAGS) -o test_invfreq $(LDFLAGS)

test_monster: test_monster.c ../tile.c ../monster.c
	$(CC) test_monster.c ../tile.c ../monster.c $(INCS) $(CFLAGS) -o test_monster $(LDFLAGS)
