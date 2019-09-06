#include "shell.h"
#include "utils.h"
#include "process.h"
#include "fg.h"

void handle_fg(int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc != 1) {
        printf("Could not bring process to foreground: Invalid arguments.\n");
        return;
    }

    int job_num = strtol(argv[0], NULL, 0);
    if (job_num <= 0) {
        printf("Could not bring process to foreground: Invalid job number.\n");
        return;
    }

    fg(job_num);
}

bool fg(int job_num) {
    proc* p = get_data_by_id(processes, job_num);

    if (p == NULL) {
        printf("Could not bring process to foreground: Process not found.\n");
        return false;
    }

    pid_t child_pid = p->pid;

    processes = delete_node_by_pid(processes, child_pid);

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    tcsetpgrp(STDIN_FILENO, child_pid);
    kill(child_pid, SIGCONT);

    int w_st;
    waitpid(child_pid, &w_st, 0);

    tcsetpgrp(STDIN_FILENO, getpgrp());

    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    return (WIFEXITED(w_st) && WEXITSTATUS(w_st) == EXIT_SUCCESS);
}