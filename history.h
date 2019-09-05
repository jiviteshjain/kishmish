#ifndef __HISTORY_H
#define __HISTORY_H

#define HISTORY_FILE_NAME ".kishmish_history.dat"

struct history_struct {
    char data[20][MAX_STATIC_STR_LEN];
    int ind_h;
};

struct history_struct history;
char* history_path;

void handle_history(int argc, char** argv);
void init_history();
void preserve_history();
void store_history(char* str);
void print_history(int n);

#endif
