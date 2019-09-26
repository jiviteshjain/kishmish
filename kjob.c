#include "shell.h"
#include "utils.h"
#include "process.h"
#include "kjob.h"

void handle_kjob(int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc != 2) {
        fprintf(stderr, "Could not send signal: Invalid arguments.\n");
        return;
    }

    int job_num = strtol(argv[0], NULL, 10);
    if (job_num <= 0) {
        fprintf(stderr, "Could not send signal: Invalid job number.\n");
        return;
    }

    int sig_num = strtol(argv[1], NULL, 10);
    if (sig_num <= 0) {
        fprintf(stderr, "Could not send signal: Invalid signal number.\n");
        return;
    }

    kjob(job_num, sig_num);
}

bool kjob(int job_num, int sig_num) {
    proc* p = get_data_by_id(processes, job_num);
    if (p == NULL) {
        fprintf(stderr, "Could not send signal: Process not found.\n");
        return false;
    }

    if (kill(p->pid, sig_num) < 0) {
        perror("Could not send signal");
        return false;
    }
    return true;
}
