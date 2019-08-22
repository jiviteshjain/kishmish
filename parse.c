#include "shell.h"
#include "utils.h"
#include "parse.h"
#include "cd.h"
#include "ls.h"
#include "pwd.h"
#include "echo.h"
#include "pinfo.h"
#include "external.h"

void handle_tilda(int argc, char** argv) {
    for (int i = 0; i < argc; i++) {
        argv[i] = tilda_expand(argv[i]);
    }
}

int handle_amp(char* command, int argc, char** argv) {
    // argv has enough space for 1 more argument

    size_t c_len = strlen(command);

    if (command[c_len-1] == '&') {
        command[c_len - 1] = '\0';
        argv[0] = (char*)malloc(sizeof(char) * 2);
        strcpy(argv[0], "&");
        return 1;
    }

    for (int i = 0; i < argc; i++) {
        size_t len = strlen(argv[i]);
        if (len == 0) {
            continue; // safety check, although this can't happen
        }
        if (argv[i][len-1] != '&') {
            continue;
        }
        // Now surely an ampersand

        if (len == 1) {
            // only ampersand
            return i + 1;
        } else {
            // more than ampersand
            argv[i][len - 1] = '\0';
            argv[i + 1] = (char*)malloc(sizeof(char) * 2);
            strcpy(argv[i + 1], "&");
            return i + 2;
        }
    }
    return argc;
}

void parse_command(char* str) {
    
    if (str == NULL)
        return;

    size_t len = strlen(str);
    // char* str_backup = (char*)malloc(sizeof(char) * (len + 1));
    // strcpy(str_backup, str);

    char* next = strtok(str, space_delim);
    if (next == NULL)
        return;
    char* command = (char*)malloc(sizeof(char) * (strlen(next) + 1));
    strcpy(command, next);

    int argc = 0;
    char** argv = (char**)malloc(sizeof(char*) * (len+1)); // lots of space

    while((next = strtok(NULL, space_delim)) != NULL) {
        argv[argc] = (char*)malloc(sizeof(char) * (strlen(next) + 1));
        strcpy(argv[argc], next);
        argc++;
    }
    argc = handle_amp(command, argc, argv);
    handle_tilda(argc, argv);
    
    if (strcmp(command, "echo") == 0) {
        handle_echo(argc, argv);
    } else if (strcmp(command, "ls") == 0) {
        handle_ls(argc, argv);
    } else if (strcmp(command, "pwd") == 0) {
        handle_pwd(argc, argv);
    } else if (strcmp(command, "cd") == 0) {
        handle_cd(argc, argv);
    } else if (strcmp(command, "pinfo") == 0) {
        handle_pinfo(argc, argv);
    } else {
        handle_external(command, argc, argv);
    }

    free(command);
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);
}

void parse(char* input) {
    
    size_t len_array = count_chars(input, ';') + 1;
    //last one may not end with ';', so +1 for safety
    char** commands = (char**)malloc(sizeof(char*) * len_array);

    int i = 0;
    char* next = strtok(input, ";");
    while (next != NULL) {
        commands[i] = (char*)malloc(sizeof(char) * (strlen(next) + 1));
        strcpy(commands[i], next);
        i++;
        next = strtok(NULL, ";");
    }
    int num_commands = i; // actual number of commands
    
    // strtok does not malloc any memory. It returns parts of input string.
    // so I'm malloc-ing only as many times as the actual number of commands
    // so free only those many times
    
    for (i = 0; i < num_commands; i++) {
        parse_command(commands[i]);
        free(commands[i]);
    }
    free(commands);
}