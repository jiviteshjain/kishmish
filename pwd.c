#include "shell.h"
#include "pwd.h"
#include "utils.h"

void handle_pwd(int argc, char** argv) {
    if (argc == 0) {
        pwd();
    } else if (argc == 1) {
        if (strcmp(argv[0], "&") == 0) {
            pwd();
        } else {
            printf("Could not display current working directory: Invalid arguments.\n");
        }
    } else {
        printf("Could not display current working directory: Invalid arguments.\n");
    }
}

bool pwd() {
    char* dir = getcwd(NULL, 0);
    
    if (dir == NULL) {
        perror("Could not display the current working directory");
        return false;
    }
    
    printf("%s\n", dir);
    free(dir);
    return true;
}
