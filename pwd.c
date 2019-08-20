#include "shell.h"
#include "pwd.h"
#include "utils.h"

bool pwd() {
    char* dir = getcwd(NULL, 0);
    
    if (dir == NULL) {
        perror("Could not display the current working directory:");
        return false;
    }
    
    printf("%s\n", dir);
    free(dir);
    return true;
}
