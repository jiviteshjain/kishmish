#include "shell.h"
#include "utils.h"
#include "history.h"

void init_history() {
    ind_h = -1;
    // ind_h is index filled, not number of commands
}

void handle_history(int argc, char** argv) {
    if (argc > 2) {
        printf("Could not print history: Invalid arguments.\n");
        return;
    }

    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc > 1) {
        printf("Could not print history: Invalid arguments.\n");
        return;
    }
    if (argc == 0) {
        print_history(10);
    } else {
        int num = strtol(argv[0], NULL, 10);  // returns 0 if not a valid number, which is fine
        print_history(num);
    }
    
    
}

void store_history(char* str) {
    if (strlen(str) > 1023) {
        return;
    }

    if (ind_h < 19) {
        ind_h++;
        strcpy(history[ind_h], str);
        return;
    }

    ind_h++;
    if (ind_h >= 40) {
        ind_h = ind_h - 20;
    }

    // ind_h >= 20 here
    strcpy(history[ind_h - 20], str);
}

void print_history(int n) {
    if (n <= 0) {
        return;
    }
    if (n > ind_h + 1) {
        n = ind_h + 1;
    }
    if (n > 10) {
        n = 10;
    }

    if (ind_h < 20) {
        for (int i = ind_h - n + 1; i <= ind_h; i++) {
            printf("%s\n", history[i]);
        }
        return;
    }

    // ind_h >= 20 here
    // ind_h - 20 gives index

    if (n <= ind_h - 20) {
        for (int i = ind_h - n -19; i <= ind_h - 20; i++) {
            printf("%s\n", history[i]);
        }
        return;
    }

    // n > in_h - 20 here
    // in_h >= 20 here
    int s = ind_h - n + 1;
    for (int i = 0; i < n; i++) {
        printf("%s\n", history[s]);
        s = (s + 1) % 20;
    }
}