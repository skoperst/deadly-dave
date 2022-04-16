#include <stdio.h>

#include "game.h"

void usage() {
    printf("-w             start windowed \n");
}

int main(int argc, char **argv) {
    int is_windowed = 0;

    argc--;
    argv++;
    while (argc > 0) {
        if (strcmp(argv[0], "-w") == 0) {
            argc--;
            argv++;
            is_windowed = 1;
        } else {
            usage();
            return 0;
        }
    }

    return game_main(is_windowed);
}
