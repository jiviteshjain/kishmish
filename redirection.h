#ifndef __REDIRECTION_H
#define __REDIRECTION_H

typedef struct {
    bool input;
    char input_file[MAX_STATIC_STR_LEN];
    bool output;
    char output_file[MAX_STATIC_STR_LEN];
    bool append;
} redirect;

typedef struct {
    int stdin_copy;
    int in_file;
    int stdout_copy;
    int out_file;
} redirect_meta;

redirect_meta redirection_begin(const redirect r);

void redirection_end(const redirect r, const redirect_meta m);

#endif
