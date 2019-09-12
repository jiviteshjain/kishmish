#include "shell.h"
#include "utils.h"
#include "external.h"
#include "prompt.h"
#include "process.h"
#include "signals.h"


// void child_stop(int sig_num) {
//     raise(SIGSTOP);
// }

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
    not_kishmish(new_argc, new_argv, bg);
    free(new_argv); // no need to free individual strings.
}

bool not_kishmish(int argc, char** argv, bool bg) {
    if (!bg) {
        // FOREGROUND: PARENT WAITS
        pid_t pid = fork();
        if (pid < 0) {
            // IN PARENT, CHILD DIDN'T GET CREATED
            perror("Could not execute command");
            return false;
        } else if (pid > 0) {
            // IN PARENT, WAIT FOR CHILD TO COME BACK
            FG_CHILD_PID = pid;
            char* temp = get_full_command("", argc, argv);
            strcpy(FG_CHILD_PNAME, temp);
            free(temp);

            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            // TODO: Error handling here
            setpgid(pid, 0);
            tcsetpgrp(STDIN_FILENO, pid);

            int w_st;
            waitpid(pid, &w_st, WUNTRACED);

            tcsetpgrp(STDIN_FILENO, getpgrp());

            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);
            
            if (WIFSTOPPED(w_st)) {
                // IT WAS STOPPED, NOT TERMINATED
                int child_id = store_process(FG_CHILD_PID, FG_CHILD_PNAME);
                printf("[%d] %s %d suspended\n", child_id, FG_CHILD_PNAME, FG_CHILD_PID);
            }

            FG_CHILD_PID = -1;
            FG_CHILD_PNAME[0] = '\0';

            return (WIFEXITED(w_st) && WEXITSTATUS(w_st) == EXIT_SUCCESS);
        } else {
            // IN CHILD
            setpgid(0, 0);
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
            char* temp = get_full_command("", argc, argv);
            int id = store_process(pid, temp);
            free(temp);

            printf("[%d] %d\n", id, pid);
            
            setpgid(pid, 0);
            tcsetpgrp(STDIN_FILENO, getpgrp());
            return true;
        } else {
            // IN CHILD
            setpgid(0, 0);
            // signal(SIGTTIN, child_stop);
            if (execvp(argv[0], argv) < 0) {
                perror("Could not execute command");
                exit(EXIT_FAILURE);  // kill child if can't execute command
            }
        }
    }
}
