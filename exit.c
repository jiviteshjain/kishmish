#include "shell.h"
#include "utils.h"
#include "exit.h"

void handle_exit(int argc, char** argv) {
    if (argc == 0) {
        exit_kishmish();
    } else if (argc == 1 && strcmp(argv[0], "&") == 0) {
        exit_kishmish();
    } else {
        fprintf(stderr, "Could not exit: Invalid argumemnts.\n");
        return;
    }
}

void exit_kishmish() {
    goodbye();
}