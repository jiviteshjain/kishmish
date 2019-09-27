#include "shell.h"
#include "utils.h" 
#include "cronjob.h"
#include "parse.h"

void handle_cronjob(int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc - 1], "&") == 0) {
        argc--;
    }

    if (argc < 6) {
        fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
        return;
    }

    bool c_next = false;
    bool t_next = false;
    bool p_next = false;

    bool c_found = false;
    bool t_found = false;
    bool p_found = false;

    char* command = (char*)malloc(sizeof(char) * MAX_STATIC_STR_LEN);
    command[0] = '\0';
    int t, p;

    for (int i = 0; i < argc; i++) {
        if (t_next) {
            t_next = false;
            t_found = true;
            errno = 0;
            t = strtol(argv[i], NULL, 10);
            if (errno > 0) {
                fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
                return;
            }
            continue;
        }
        if (p_next) {
            p_next = false;
            p_found = true;
            errno = 0;
            p = strtol(argv[i], NULL, 10);
            if (errno > 0) {
                fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
                return;
            }
            continue;
        }

        if (strcmp(argv[i], "-c") == 0) {
            if (c_found) {
                fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
                return;
            }
            c_next = true;
            c_found = true;
            continue;
        }
        if (strcmp(argv[i], "-t") == 0) {
            if (t_found) {
                fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
                return;
            }
            t_next = true;
            c_next = false;
            continue;
        }
        if (strcmp(argv[i], "-p") == 0) {
            if (p_found) {
                fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
                return;
            }
            c_next = false;
            p_next = true;
            continue;
        }

        if (c_next) {
            c_found = true;
            strcat(command, argv[i]);
            strcat(command, " ");
            continue;
        } else {
            fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
            return;
        }
    }

    if (!t_found || !c_found || !p_found) {
        fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
        if (command != NULL) {
            free(command);
        }
        return;
    }

    
    cronjob(command, t, p);
}

bool cronjob(char* command, int t, int p) {
    if (p == 0) {
        return true;
    }
    if (t < 0 || p < 0) {
        fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
        return false;
    }

    int num;
    if (t > p) {
        num = 1;
    } else {
        num = p / t;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Could not start cronjob");
        return false;
    } else if (pid == 0) {
        for (int i = 0; i < num; i++) {
            char* temp = (char*)malloc(sizeof(char) * (strlen(command) + 1));
            strcpy(temp, command);
            parse_command(temp); // pipes not needed, because split by pipe already
            sleep(t);
        }
        exit(EXIT_SUCCESS);
    }
}