#include "shell.h"
#include "utils.h" 
#include "cronjob.h"
#include "pipe.h"

void handle_cronjob(int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc - 1], "&") == 0) {
        argc--;
    }

    if (argc != 6) {
        fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
        return;
    }

    bool c_next = false;
    bool t_next = false;
    bool p_next = false;

    bool c_found = false;
    bool t_found = false;
    bool p_found = false;

    char* command = NULL;
    int t, p;

    for (int i = 0; i < argc; i++) {
        if (c_next) {
            c_next = false;
            c_found = true;
            command = (char*)malloc(sizeof(char) * (strlen(argv[i]) + 1));
            strcpy(command, argv[i]);
            continue;
        }
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
            continue;
        }
        if (strcmp(argv[i], "-t") == 0) {
            if (t_found) {
                fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
                return;
            }
            t_next = true;
            continue;
        }
        if (strcmp(argv[i], "-p") == 0) {
            if (p_found) {
                fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
                return;
            }
            p_found = true;
            p_next = true;
            continue;
        }
    }

    if (!t_found || !c_found || !p_found) {
        // THIS ALSO INCLUDES THE CASE WHERE SAME ARGUMENT IS GIVEN MULTIPLE TIMES, BECAUSE ONLY 6 ARGUMENTS SO IT MEANS ONE OF THESE WASN'T GIVEN
        fprintf(stderr, "Could not start cronjob: Invalid arguments.\n");
        if (command != NULL) {
            free(command);
        }
        return;
    }

    char* unquoted = trim(command, '"');
    unquoted = trim(unquoted, '\'');
    cronjob(unquoted, t, p);
    free(command);
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
            pied_piper(command);
            sleep(t);
        }
        exit(EXIT_SUCCESS);
    }
}