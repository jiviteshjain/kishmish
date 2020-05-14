#include "shell.h"
#include "utils.h"
#include "process.h"
#include "fg.h"

void handle_fg(int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc != 1) {
        fprintf(stderr, "Could not bring process to foreground: Invalid arguments.\n");
        return;
    }

    int job_num = strtol(argv[0], NULL, 0);
    if (job_num <= 0) {
        fprintf(stderr, "Could not bring process to foreground: Invalid job number.\n");
        return;
    }

    fg(job_num);
}

bool fg(int job_num) {
    proc* p = get_data_by_id(processes, job_num);

    if (p == NULL) {
        fprintf(stderr, "Could not bring process to foreground: Process not found.\n");
        return false;
    }

    pid_t child_pid = p->pid;
    
    printf("[%d] %s %d resumed\n", job_num, p->pname, p->pid);

    FG_CHILD_PID = child_pid;
    strcpy(FG_CHILD_PNAME, p->pname);

    // process list only stores background processes
    processes = delete_node_by_pid(processes, child_pid);

    // protect shell against signals for illegal use of stdin and stdout
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    // It is already a separate group
    // make the child's process group the foreground process group
    tcsetpgrp(STDIN_FILENO, child_pid);
    // TODO: Error handling here

    // ask child to continue
    kill(child_pid, SIGCONT);

    // wait for child
    int w_st;
    waitpid(child_pid, &w_st, WUNTRACED);

    // child is completed, now make shell the foreground process group again
    tcsetpgrp(STDIN_FILENO, getpgrp());

    // safe to end protection from signals
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    
    // if the child was just stopped, store it in the process list again
    if (WIFSTOPPED(w_st)) {
        // IT WAS STOPPED, NOT TERMINATED
        int child_id = store_process(FG_CHILD_PID, FG_CHILD_PNAME);
        printf("[%d] %s %d suspended\n", child_id, FG_CHILD_PNAME, FG_CHILD_PID);
    }

    // no foreground process
    FG_CHILD_PID = -1;
    FG_CHILD_PNAME[0] = '\0';

    return (WIFEXITED(w_st) && WEXITSTATUS(w_st) == EXIT_SUCCESS);
}