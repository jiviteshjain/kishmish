#ifndef __NIGHTSWATCH_H
#define __NIGHTSWATCH_H

#include <sys/time.h>
#include <sys/select.h>

void handle_nightswatch(int argc, char** argv);
bool night_interrupt();
bool night_dirty();
bool nightswatch(int comm, int interval);
bool get_me_out();

#endif
