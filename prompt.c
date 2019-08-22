#include "shell.h"
#include "utils.h"
#include "prompt.h"



char* get_relative_pwd() {
    char* cur_dir = getcwd(NULL, 0);
    size_t cur_len = strlen(cur_dir);
    size_t home_len = strlen(home_dir);

    if (strcmp(cur_dir, home_dir) == 0) {
        free(cur_dir);
        // Allocate dynamically to allow caller to free it
        char* temp = (char*)malloc(sizeof(char) * 2);
        temp[0] = '~';
        temp[1] = '\0';
        return temp;
    }

    char* first_part = substr(cur_dir, 0, home_len);
    if (first_part != NULL && strcmp(first_part, home_dir) == 0) {
        char* second_part = substr(cur_dir, home_len, cur_len - home_len);
        char* relative_pwd = (char*)malloc(sizeof(char) * (strlen(second_part) + 2));
        relative_pwd[0] = '~';
        relative_pwd[1] = '\0';
        strcat(relative_pwd, second_part);

        free(cur_dir);
        free(first_part);
        free(second_part);
        return relative_pwd;
    }
    free(first_part);
    return cur_dir;
}

void prompt() {
    char* path = get_relative_pwd();

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