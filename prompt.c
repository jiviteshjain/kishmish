#include "shell.h"
#include "utils.h"
#include "prompt.h"

void prompt() {
    char* path = get_relative_pwd(getcwd(NULL, 0));

    // Looks like: <uname@hostname:~/path/to/pwd>

    printf("<" ANSI_GREEN_BOLD "%s@%s" ANSI_DEFAULT ":" ANSI_CYAN "%s" ANSI_DEFAULT "> ", user_name, host_name, path);

    // size_t length = strlen(ANSI_YELLOW_BOLD) + strlen(user_name) + strlen(host_name) + strlen(path) + strlen(ANSI_DEFAULT) + 4;

    // char* prompt = (char*)malloc(sizeof(char) * (length + 1));
    // prompt[0] = '\0';
    // strcat(prompt, ANSI_YELLOW_BOLD);
    // strcat(prompt, user_name);
    // strcat(prompt, "@");
    // strcat(prompt, host_name);
    // strcat(prompt, ":");
    // strcat(prompt, path);
    // strcat(prompt, "> ");
    // strcat(prompt, ANSI_DEFAULT);

    free(path);
    // return prompt;
}