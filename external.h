#ifndef __EXTERNAL_H
#define __EXTERNAL_H

#include <sys/wait.h>

void handle_external(char* command, int argc, char** argv);

bool not_kishmish(char** argv, bool bg);

#endif
