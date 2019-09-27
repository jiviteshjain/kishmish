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

char** break_pipes(char* str) {
    size_t num_pipes = count_pipes(str);
    size_t num_commands = num_pipes + 1;

    char** commands = (char**)malloc(sizeof(char*) * (num_commands + 1));
    commands[num_commands] = NULL;

    size_t i = 0;
    char* next = strtok(str, "|");
    while (next != NULL) {
        commands[i] = (char*)malloc(sizeof(char) * (strlen(next) + 1));
        strcpy(commands[i], next);
        i++;
        next = strtok(NULL, "|");
    }

    return commands;
}

void pied_piper(char* str) {
    if (str == NULL) {
        return;
    }

    size_t num_pipes = count_pipes(str);
    size_t num_commands = num_pipes + 1;
    if (num_commands == 1) {
        parse_command(str);
        return;
    }

    char** commands = break_pipes(str); // DONOT USE STR AFTER THIS

    int pipe_a[2];
    int pipe_b[2];


    for (int i = 0; i < num_commands; i++) {
        if (i & 1) {
            if (i < num_commands - 1) {
                pipe(pipe_b);
            }
            pid_t pid = fork();

            if (pid < 0) {
                // IN PARENT, FORK FAILED
                perror("Could not create pipeline");
                for (int j = 0; j < num_commands; j++) {
                    free(commands[j]);
                }
                free(commands);
                return;
            } else if (pid > 0) {
                // IN PARENT
                if (i > 0) {
                    close(pipe_a[0]);
                }
                if (i < num_commands - 1) {
                    close(pipe_b[1]);
                }
                waitpid(pid, NULL, 0);
            } else {
                // IN CHILD
                if (i > 0) {
                    // close(pipe_a[1]);
                    dup2(pipe_a[0], STDIN_FILENO);
                }

                if (i < num_commands - 1) {
                    // close(pipe_b[0]);
                    dup2(pipe_b[1], STDOUT_FILENO);
                }

                parse_command(commands[i]);
                exit(0);
            }
        } else {
            if (i < num_commands - 1) {
                pipe(pipe_a);
            }
            pid_t pid = fork();

            if (pid < 0) {
                // IN PARENT, FORK FAILED
                perror("Could not create pipeline");
                for (int j = 0; j < num_commands; j++) {
                    free(commands[j]);
                }
                free(commands);
                return;
            } else if (pid > 0) {
                // IN PARENT
                if (i > 0) {
                    close(pipe_b[0]);
                }
                if (i < num_commands - 1) {
                    close(pipe_a[1]);
                }
                waitpid(pid, 0, WUNTRACED);
            } else {
                // IN CHILD
                if (i > 0) {
                    // close(pipe_b[1]);
                    dup2(pipe_b[0], STDIN_FILENO);
                }

                if (i < num_commands - 1) {
                    // close(pipe_a[0]);
                    dup2(pipe_a[1], STDOUT_FILENO);
                }

                parse_command(commands[i]);
                exit(0);
            }
        }
    }

    for (int j = 0; j < num_commands; j++) {
        free(commands[j]);
    }
    free(commands);
    return;
}

void pied_piper_no_fork(char* str) {
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
    if (stdout_copy < 0) {
        perror("Could not create pipeline");
        free(command);
        for (size_t j = 0; j < num_pipes; j++) {
            free(pipes[j]);
        }
        free(pipes);
        return;
    }

    if (pipe(pipes[0]) < 0) {
        perror("Could not create pipeline");
        free(command);
        for (size_t j = 0; j < num_pipes; j++) {
            free(pipes[j]);
        }
        free(pipes);
        close(stdout_copy);
        return;
    }

    if (dup2(pipes[0][1], STDOUT_FILENO) < 0) {
        perror("Could not create pipeline");
        free(command);
        close(pipes[0][0]);
        close(pipes[0][1]);
        for (size_t j = 0; j < num_pipes; j++) {
            free(pipes[j]);
        }
        free(pipes);
        close(stdout_copy);
        return;
    }
    parse_command(command);
    close(pipes[0][1]);

    int stdin_copy = dup(STDIN_FILENO);
    if (stdin_copy < 0) {
        perror("Could not create pipeline");
        fprintf(stderr, "Fatal I/O error. Exiting...");
        goodbye();
    }

    size_t i = 0;
    while ((next = strtok_r(NULL, "|", &temp)) != NULL) {
        command = (char*)realloc(command, sizeof(char) * (strlen(next) + 1));
        strcpy(command, next);
    
        i++; // i refers to 0 indexed command number
        if (i == num_commands - 1) {
            break; // command already copied here
        }
        if (pipe(pipes[i]) < 0) {
            perror("Could not create pipeline");
            fprintf(stderr, "Fatal I/O error. Exiting...");
            goodbye();
        }
        if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0) {
            perror("Could not create pipeline");
            fprintf(stderr, "Fatal I/O error. Exiting...");
            goodbye();
        }
        if (dup2(pipes[i][1], STDOUT_FILENO) < 0) {
            perror("Could not create pipeline");
            fprintf(stderr, "Fatal I/O error. Exiting...");
            goodbye();
        }
        parse_command(command);
        close(pipes[i - 1][0]);
        close(pipes[i][1]);
    }

    if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0) {
        perror("Could not create pipeline");
        fprintf(stderr, "Fatal I/O error. Exiting...");
        goodbye();
    }
    if (dup2(stdout_copy, STDOUT_FILENO) < 0) {
        perror("Could not create pipeline");
        fprintf(stderr, "Fatal I/O error. Exiting...");
        goodbye();
    }

    parse_command(command);

    if (dup2(stdin_copy, STDIN_FILENO) < 0) {
        perror("Could not create pipeline");
        fprintf(stderr, "Fatal I/O error. Exiting...");
        goodbye();
    }
    
    close(pipes[i - 1][0]);
    close(stdin_copy);
    close(stdout_copy);
    
    free(command);

    for (size_t j = 0; j < num_pipes; j++) {
        free(pipes[j]);
    }
    free(pipes);
}