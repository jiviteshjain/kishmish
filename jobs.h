#ifndef __JOBS_H
#define __JOBS_H

char get_proc_state(pid_t pid);
void handle_jobs(int argc, char** argv);
bool jobs();

#endif
