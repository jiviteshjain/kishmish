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
    
    printf("[%d] %s %d resumed\n", job_num, p->pname, p->pid);

    FG_CHILD_PID = child_pid;
    strcpy(FG_CHILD_PNAME, p->pname);

    processes = delete_node_by_pid(processes, child_pid);

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    // TODO: Error handling here
    tcsetpgrp(STDIN_FILENO, child_pid); // It is already a separate group
    kill(child_pid, SIGCONT);

    int w_st;
    waitpid(child_pid, &w_st, WUNTRACED);

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

    // setpgid(child_pid, getpgrp());
    // tcsetpgrp(STDIN_FILENO, getpgrp());
    // kill(child_pid, SIGCONT);

    // int w_st;
    // waitpid(child_pid, &w_st, 0);

    return (WIFEXITED(w_st) && WEXITSTATUS(w_st) == EXIT_SUCCESS);
}