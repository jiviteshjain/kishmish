#include "shell.h"
#include "utils.h"
#include "cd.h"


void handle_cd (int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }
    if (argc == 0) {
        cd(".");
    } else if (argc > 1) {
        printf("Could not change directory: Invalid arguments\n");
        return;
    } else {
        char* temp = trim(argv[0], '\"');
        temp = trim(temp, '\'');
        cd(temp);
    }
}

bool cd(char* path) {
    char* final_path;
    // TODO: Let the parser handle ~
    if (strcmp(path, "~") == 0) {
        final_path = home_dir;
    } else if (strcmp(path, "") == 0) {
        final_path = home_dir;
    } else {
        final_path = path;
    }

    int ret_val = chdir(final_path);
    if (ret_val < 0) {
        perror("Could not change directory");
        return false;
    }
    return true;
}