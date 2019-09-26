#include "shell.h"
#include "utils.h"
#include "env.h"

void handle_setenv(int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc < 1 || argc > 2) {
        fprintf(stderr, "Could not set environment variable: Invalid arguments.\n");
        return;
    }

    if (argc == 1) {
        set_env(argv[0], "");
    } else {
        set_env(argv[0], argv[1]);
    }
}

bool set_env(char* name, char* val) {
    if (setenv(name, val, 1) < 0) {
        // setenv copied those strings
        perror("Could not set environment variable");
        return false;
    }
    return true;
}

void handle_unsetenv(int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc != 1) {
        fprintf(stderr, "Could not unset environment variable: Invalid arguments.\n");
        return;
    }

    unset_env(argv[0]);
}

bool unset_env(char* name) {
    if (unsetenv(name) < 0) {
        perror("Could not unset environment variable");
        return false;
    }
    return true;
}
