#ifndef __LS_H
#define __LS_H

#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

typedef struct {
    // does not contain file name because that is already available
    char* perms;  // malloc-ed by get_ls_data()
    int num_h_link;
    char* user;   // malloc-ed by getpwuid()
    char* group;  // malloc-ed by getgrgid()
    size_t size;
    char* time;  // malloc-ed by get_ls_data()
    char* name;  // malloc-ed by scandir()
    bool valid;
} ls_data;

void handle_ls(int argc, char** argv);

void print_ls_data(const ls_data f);

ls_data get_ls_data(const char* path);

int filter(const struct dirent* s);

bool ls(char* path, bool flag_a, bool flag_l);

#endif