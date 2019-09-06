#ifndef __FG_H
#define __FG_H

#include <sys/wait.h>
#include <signal.h>

void handle_fg(int argc, char** argv);
bool fg(int job_num);

#endif
