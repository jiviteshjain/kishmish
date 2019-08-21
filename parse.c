#include "shell.h"
#include "utils.h"
#include "parse.h"

void parse_command(char* str) {
    
    size_t len = strlen(str);
    char* str_backup = (char*)malloc(sizeof(char) * (len + 1));
    strcpy(str_backup, str);

    char* next = strtok(str, whitespace_delim);
    char* command = (char*)malloc(sizeof(char) * (strlen(next) + 1));
    strcpy(command, next);

    int argc = 0;
    char** argv = (char**)malloc(sizeof(char*) * len); // lots of space

    while((next = strtok(NULL, whitespace_delim)) != NULL) {
        argv[argc] = (char*)malloc(sizeof(char) * (strlen(next) + 1));
        strcpy(argv[argc], next);
        argc++;
    }
    
    if (strcmp(command, "echo")) {
        // handle_echo(argc, argv);
    } else if (strcmp(command, "ls")) {
        // handle_ls(argc, argv);
    } else if (strcmp(command, "pwd")) {
        // handle_pwd(argc, argv);
    } else if (strcmp(command, "cd")) {
        // handle_cd(argc, argv);
    } else {
        // handle_builtins(str_backup);
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