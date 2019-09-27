#ifndef __CRONJOB_H
#define __CRONJOB_H

#include <errno.h>

void handle_cronjob(int argc, char** argv);
bool cronjob(char* command, int t, int p);

#endif
