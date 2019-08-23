#ifndef __EXTERNAL_H
#define __EXTERNAL_H

#include <sys/wait.h>
#include <signal.h>

#define MAX_BG_PROCESS 100

typedef struct {
    pid_t pid;
    char p_name[100];
} bg_process;

bg_process processes[MAX_BG_PROCESS];
int num_processes;

void init_bg_process();

void handle_external(char* command, int argc, char** argv);

bool not_kishmish(char** argv, bool bg);

void child_dead(int sig_num);

void store_process(pid_t pid, char* comm);

char* get_process_name(pid_t pid);

#endif
