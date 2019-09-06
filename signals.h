#ifndef __SIGNALS_H
#define __SIGNALS_H

#include <signal.h>
#include <sys/wait.h>

void init_signals();
void child_dead(int sig_num);
void send_me_back(int sig_num);

#endif
