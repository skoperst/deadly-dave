#include <stdio.h>

#include "game.h"

void usage() {
    printf("-w             start windowed \n");
    printf("-l <level>     start in level \n");
}

int main(int argc, char **argv) {
    int is_windowed = 0;
    int level = 0;

    argc--;
    argv++;
    while (argc > 0) {
        if (strcmp(argv[0], "-w") == 0) {
            argc--;
            argv++;
            is_windowed = 1;

        } else if (strcmp(argv[0], "-l") == 0) {
            argc--;
            argv++;
            if (argc < 1) {
                usage();
                exit(-1);
            }
            level = atoi(argv[0]);
            argc--;
            argv++;

        } else {
            usage();
            return 0;
        }
    }

    return game_main(is_windowed, level);
}
