#include "shell.h"
#include "utils.h"
#include "process.h"
#include "signals.h"
#include "prompt.h"

void init_signals() {
    signal(SIGCHLD, child_dead);
    signal(SIGTSTP, send_me_back);
}

void child_dead(int sig_num) {
    int w_st;
    pid_t pid = waitpid(-1, &w_st, WNOHANG);

    if (pid <= 0) {
        return;
    }
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

void send_me_back(int sig_num) {
    
    pid_t pid = getpid();

    if (pid != SHELL_PID) {
        // CHILD PROCESS
        return;
    }
    // PARENT PROCESS
    if (FG_CHILD_PID == -1) {
        // NO FG PROCESS RUNNING
        return;
    }

    // SEND FG PROCESS TO BACKGROUND
    setpgid(FG_CHILD_PID, 0);
    kill(FG_CHILD_PID, SIGTSTP);
    tcsetpgrp(STDIN_FILENO, getpgrp()); // should pull resources out of that processes

    
    int child_id = store_process(FG_CHILD_PID, FG_CHILD_PNAME);
    printf("[%d] %s %d suspended\n", child_id, FG_CHILD_PNAME, FG_CHILD_PID);
    fflush(stdout);
}