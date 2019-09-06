#ifndef __KJOB_H
#define __KJOB_H

#include <signal.h>

void handle_kjob(int argc, char** argv);
bool kjob(int job_num, int sig_num);

#endif
