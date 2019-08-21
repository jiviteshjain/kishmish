#include "shell.h"
#include "utils.h"
#include "external.h"

void handle_external(char* command, int argc, char** argv) {
    char** new_argv = (char**)malloc(sizeof(char*) * (argc + 2));
    new_argv[0] = command; // Not copying strings, just the pointers
    bool bg = false;
    if (argc > 0 && strcmp(argv[argc - 1], "&") == 0) {
        bg = true;
        argc--;
    }

    int new_argc = 1;
    for (int i = 0; i < argc; i++) {
        new_argv[new_argc] = argv[i];
        new_argc++;
    }
    new_argv[new_argc] = NULL;
    not_kishmish(new_argv, bg);
    free(new_argv); // no need to free individual strings.
}

bool not_kishmish(char** argv, bool bg) {
    if (!bg) {
        // FOREGROUND: PARENT WAITS
        pid_t pid = fork();
        if (pid < 0) {
            // IN PARENT, CHILD DIDN'T GET CREATED
            perror("Could not execute command");
            return false;
        } else if (pid > 0) {
            // IN PARENT, WAIT FOR CHILD TO COME BACK
            int w_st;
            waitpid(pid, &w_st, WUNTRACED);
            return (WIFEXITED(w_st) && WEXITSTATUS(w_st) == EXIT_SUCCESS);
        } else {
            // IN CHILD
            if (execvp(argv[0], argv) < 0) {
                perror("Could not execute command");
                exit(EXIT_FAILURE); // kill child if can't execute command
            }
        }
    } else {
        // BACKGROUND: PARENT KEEPS GOING
        pid_t pid = fork();
        if (pid < 0) {
            // IN PARENT, CHILD DIDN'T GET CREATED
            perror("Could not execute command");
            return false;
        } else if (pid > 0) {
            // IN PARENT, DON'T WAIT
            return true;
        } else {
            // IN CHILD
            if (execvp(argv[0], argv) < 0) {
                perror("Could not execute command");
                exit(EXIT_FAILURE);  // kill child if can't execute command
            }
        }
    }
}