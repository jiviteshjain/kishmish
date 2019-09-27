#ifndef __PIPE_H
#define __PIPE_H
#include <sys/wait.h>

size_t count_pipes(const char* command);
void pied_piper_fork(char* str);
void pied_piper(char* str);

#endif
