#include "shell.h"
#include "utils.h"
#include "parse.h"
#include "redirection.h"
#include "pipe.h"

size_t count_pipes(const char* command) {
    size_t len = strlen(command);
    size_t count = 0;
    for (size_t i = 0; i < len; i++) {
        if (command[i] == '|') {
            count++;
        }
    }
    return count;
}

void pied_piper(char* str) {
    // The pipe does not stop if a command in between fails.
    if (str == NULL) {
        return;
    }

    size_t num_pipes = count_pipes(str);
    size_t num_commands = num_pipes + 1;
    if (num_commands == 1) {
        parse_command(str);
        return;
    }

    char* temp;
    char* next = strtok_r(str, "|", &temp);
    if (next == NULL)
        return;
    char* command = (char*)malloc(sizeof(char) * (strlen(next) + 1));
    strcpy(command, next);

    int** pipes = (int**)malloc(sizeof(int*) * num_pipes);
    for (size_t j = 0; j < num_pipes; j++) {
        pipes[j] = (int*)malloc(sizeof(int) * 2);
    }

    int stdout_copy = dup(STDOUT_FILENO);

    pipe(pipes[0]);
    dup2(pipes[0][1], STDOUT_FILENO);
    parse_command(command);
    close(pipes[0][1]);

    int stdin_copy = dup(STDIN_FILENO);

    size_t i = 0;
    while ((next = strtok_r(NULL, "|", &temp)) != NULL) {
        command = (char*)realloc(command, sizeof(char) * (strlen(next) + 1));
        strcpy(command, next);
    
        i++; // i refers to 0 indexed command number
        if (i == num_commands - 1) {
            break; // command already copied here
        }
        pipe(pipes[i]);
        dup2(pipes[i - 1][0], STDIN_FILENO);
        dup2(pipes[i][1], STDOUT_FILENO);
        parse_command(command);
        close(pipes[i - 1][0]);
        close(pipes[i][1]);
    }

    dup2(pipes[i - 1][0], STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);

    parse_command(command);

    dup2(stdin_copy, STDIN_FILENO);
    
    close(pipes[i - 1][0]);
    close(stdin_copy);
    close(stdout_copy);
    
    free(command);
}