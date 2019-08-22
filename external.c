#include "shell.h"
#include "utils.h"
#include "external.h"
#include "prompt.h"

void child_dead (int sig_num) {
    int w_st;
    pid_t pid = waitpid(-1, &w_st, WNOHANG);

    if (pid <= 0) {
        return;
    }

    // char* file_path = (char*)malloc(sizeof(char) * (12 + length_num(pid)));
    // sprintf(file_path, "/proc/%d/comm", pid);
    // printf("%s\n", file_path);
    // char* p_name = (char*)malloc(sizeof(char) * 100);
    // FILE* f = fopen(file_path, "r");
    // if (f == NULL) {
    //     strcpy(p_name, "-undefined-");
    // } else {
    //     fgets(p_name, 1000, f);
    // }
    // free(file_path);
    // if (f != NULL)
    //     fclose(f);

    if (WIFEXITED(w_st) && WEXITSTATUS(w_st) == EXIT_SUCCESS) {
        fprintf(stderr, ANSI_RED_BOLD "\nALERT: Process with ID %d exited normally.\n" ANSI_DEFAULT, pid);
    } else {
        fprintf(stderr, ANSI_RED_BOLD "\nALERT: Process with ID %d exited abnormally.\n" ANSI_DEFAULT, pid);
    }
    prompt();
    fflush(stdout);
    return;
    // free(p_name);
}

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
            signal(SIGCHLD, child_dead);
            // usleep(500 * 1000 * 1000);
            return true;
        } else {
            // IN CHILD
            // close(STDOUT_FILENO);
            // close(STDERR_FILENO);
            // setpgid(0, 0);
            if (execvp(argv[0], argv) < 0) {
                perror("Could not execute command");
                exit(EXIT_FAILURE);  // kill child if can't execute command
            }
        }
    }
}