#ifndef __HISTORY_H
#define __HISTORY_H

char history[20][1024];
int ind_h;

void handle_history(int argc, char** argv);
void init_history();
void store_history(char* str);
void print_history(int n);

#endif
