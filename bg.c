#include "shell.h"
#include "utils.h"
#include "process.h"
#include "bg.h"

void handle_bg(int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc - 1], "&") == 0) {
        argc--;
    }

    if (argc != 1) {
        printf("Could not run background process: Invalid arguments.\n");
        return;
    }

    int job_num = strtol(argv[0], NULL, 0);
    if (job_num <= 0) {
        printf("Could not run background process: Invalid job number.\n");
        return;
    }

    bg(job_num);
}

bool bg(int job_num) {
    proc* p = get_data_by_id(processes, job_num);

    if (p == NULL) {
        printf("Could not run background process: Process not found.\n");
        return false;
    }

    pid_t child_pid = p->pid;
    // Don't delete from processes list, as it is still in background

    if (kill(child_pid, SIGCONT) < 0) {
        perror("Could not run background process");
        return false;
    }

    return true;
}