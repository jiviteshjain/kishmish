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

    char* temp = get_process_name(pid); // Don't free temp

    if (WIFEXITED(w_st) && WEXITSTATUS(w_st) == EXIT_SUCCESS) {
        if (temp == NULL)
            fprintf(stderr, ANSI_RED_BOLD "\nALERT: Process with ID %d exited normally.\n" ANSI_DEFAULT, pid);
        else
            fprintf(stderr, ANSI_RED_BOLD "\nALERT: %s with ID %d exited normally.\n" ANSI_DEFAULT, temp, pid);
    } else {
        if (temp == NULL)
            fprintf(stderr, ANSI_RED_BOLD "\nALERT: Process with ID %d exited abnormally.\n" ANSI_DEFAULT, pid);
        else
            fprintf(stderr, ANSI_RED_BOLD "\nALERT: %s with ID %d exited abnormally.\n" ANSI_DEFAULT, temp, pid);
    }
    free(temp);
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
            store_process(pid, argv[0]);
            printf("[%d] %d\n", num_processes, pid);
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

void init_bg_process() {
    for (int i = 0; i < MAX_BG_PROCESS; i++) {
        processes[i].pid = -1;
    }
    num_processes = 0;
}

void store_process(pid_t pid, char* comm) {
    num_processes++;
    int i = 0;
    for (int i = 0; i < MAX_BG_PROCESS; i++) {
        if (processes[i].pid == -1) {
            break;
        }
    }
    if (processes[i].pid != -1)
        return; // no space to store

    // Now we have space to store

    strcpy(processes[i].p_name, comm);
    processes[i].pid = pid;
}

char* get_process_name(pid_t pid) {
    num_processes--;
    for (int i = 0; i < MAX_BG_PROCESS; i++) {
        if (processes[i].pid == pid) {
            char* temp = (char*)malloc(sizeof(char) * (strlen(processes[i].p_name) + 1));
            strcpy(temp, processes[i].p_name);
            processes[i].pid = -1;
            return temp;
        }
    }
    return NULL;
}
