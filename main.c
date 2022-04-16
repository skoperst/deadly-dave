#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>

#include "game.h"


void sigseg_handler(int sig) {
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

void usage() {
    printf("-w             start windowed \n");
    printf("-l <level>     start in level \n");
}

int main(int argc, char **argv) {
    int is_windowed = 0;
    int level = 0;

    signal(SIGSEGV, sigseg_handler);

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
