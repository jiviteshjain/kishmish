#ifndef __JOB_H
#define __JOB_H

char get_proc_state(pid_t pid);
void handle_jobs(int argc, char** argv);
bool jobs();

#endif
