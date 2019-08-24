#include "shell.h"
#include "utils.h"
#include "prompt.h"

void prompt() {
    char* abs_path = getcwd(NULL, 0);
    char* path = get_relative_pwd(abs_path);

    // Looks like: <uname@hostname:~/path/to/pwd>

    printf("<" ANSI_GREEN_BOLD "%s@%s" ANSI_DEFAULT ":" ANSI_CYAN "%s" ANSI_DEFAULT "> ", user_name, host_name, path);
    free(path);
    free(abs_path);
}