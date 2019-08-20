#include "shell.h"
#include "utils.h"
#include "cd.h"

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
        perror("Could not change directory.");
        return false;
    }
    return true;
}