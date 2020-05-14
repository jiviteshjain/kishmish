#include "shell.h"
#include "utils.h"
#include "process.h"
#include "signals.h"
#include "prompt.h"

void init_signals() {
    signal(SIGCHLD, child_dead);
    signal(SIGTSTP, sigtstp_handler);
    signal(SIGINT, sigint_handler);
}

void child_dead(int sig_num) {
    // a child process terminated
    // update process data structures and print an alert

    int w_st;
    pid_t pid = waitpid(-1, &w_st, WNOHANG);

    if (pid <= 0) {
        return;
    }

    // get the name of the dead process and remove it from the list
    proc* p = get_data_by_pid(processes, pid);
    char* pname;
    if (p != NULL) {
        char* temp = p->pname;  // Don't free temp
        pname = (char*)malloc(sizeof(char) * (strlen(temp) + 1));
        strcpy(pname, temp);
        processes = delete_node_by_pid(processes, pid);
    } else {
        pname = (char*)malloc(sizeof(char) * MAX_STATIC_STR_LEN);
        strcpy(pname, "Process");
    }

    // print an alert
    if (WIFEXITED(w_st) && WEXITSTATUS(w_st) == EXIT_SUCCESS) {
        fprintf(stderr, ANSI_RED_BOLD "\nALERT: %s with ID %d exited normally.\n" ANSI_DEFAULT, pname, pid);
    } else {
        fprintf(stderr, ANSI_RED_BOLD "\nALERT: %s with ID %d exited abnormally.\n" ANSI_DEFAULT, pname, pid);
    }
    prompt();
    fflush(stdout);
    free(pname);
    return;
}

void sigtstp_handler(int signum) {
    pid_t pid = getpid();

    if (pid != SHELL_PID) {
        // CHILD PROCESS
        return;
    }

    if (FG_CHILD_PID == -1) {
        return;
    }

    raise(SIGTSTP);
}

void sigint_handler(int signum) {
    pid_t pid = getpid();

    if (pid != SHELL_PID){
        // child process
        return;
    }

    if (FG_CHILD_PID == -1) {
        return;
    }

    raise(SIGINT);
}