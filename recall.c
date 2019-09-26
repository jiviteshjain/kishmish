#include "shell.h"
#include "utils.h"
#include "history.h"
#include "prompt.h"
#include "pipe.h"
#include "recall.h"

bool is_recall(char* command) {
    size_t len = strlen(command);
    
    if (len < 3) {
        return false;
    }

    return (
        command[0] == UP_0 &&
        command[1] == UP_1 &&
        command[2] == UP_2
    );
}

void handle_recall(char* command, int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc != 0) {
        fprintf(stderr, "Could not recall command history: Invalid syntax.\n");
        return;
    }

    size_t len = strlen(command);
    if (len % 3 != 0) {
        fprintf(stderr, "Could not recall command history: Invalid syntax.\n");
        return;
    }

    for (size_t i = 0; i < len; i++) {
        if (i % 3 == 0) {
            if (command[i] != UP_0) {
                fprintf(stderr, "Could not recall command history: Invalid syntax.\n");
                return;
            }
        } else if (i % 3 == 1) {
            if (command[i] != UP_1) {
                fprintf(stderr, "Could not recall command history: Invalid syntax.\n");
                return;
            }
        } else {
            if (command[i] != UP_2) {
                fprintf(stderr, "Could not recall command history: Invalid syntax.\n");
                return;
            }
        }
    }
    int up = len / 3;
    if (up > 20) {
        fprintf(stderr, "Could not recall command history: Recall limit of 20 exceeded.\n");
        return;
    }

    recall(up);
}

bool recall(int up) {

    if (up < 0) {
        fprintf(stderr, "Could not recall command history: Invalid arguments.\n");
        return false;
    }

    if (up > history.ind_h + 1) {
        fprintf(stderr, "Could not recall command history: Invalid arguments.\n");
        return false;
    }

    if (up > 20) {
        fprintf(stderr, "Could not recall command history: Recall limit of 20 exceeded.\n");
        return false;
    }

    int index = 0;

    if (history.ind_h < 20) {
        index = (history.ind_h - up + 1) % 20;
    } else if (up <= history.ind_h - 20) {
        index = (history.ind_h - up - 19) % 20;
    } else {
        index = (history.ind_h - up + 1) % 20;
    }

    char* command = (char*)malloc(sizeof(char) * (strlen(history.data[index]) + 1));
    strcpy(command, history.data[index]);
    
    prompt();
    printf("%s\n", command);
    if (!is_recall(command)) {
        store_history(command);
    }
    pied_piper(command);
    
    free(command);
    return true;
}