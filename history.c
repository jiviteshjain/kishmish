#include "shell.h"
#include "utils.h"
#include "history.h"

// we store the last 20 commands in a circular-queue-like data structure
// we do not store consecutive repeated commands
// semicolon-separated commands are stored as separate entries
// a pipeline of commands is stored as a single entry
// no whitespace-cleaning etc is done. command is stored is verbatim

void init_history() {
    // try to read history from binary file, if it exists
    // otherwise initialise history data structures

    size_t len = strlen(home_dir) + strlen(HISTORY_FILE_NAME) + 1;
    history_path = (char*)malloc(sizeof(char) * (len + 1));
    strcpy(history_path, home_dir);
    strcat(history_path, "/");
    strcat(history_path, HISTORY_FILE_NAME);
    
    FILE* f = fopen(history_path, "rb");
    if (f == NULL) {
        // initialise the struct yourself, assume it never existed before
        history.ind_h = -1;
        return;
    }

    if (fread(&history, sizeof(history), 1, f) != 1) {
        // initialise the struct yourself, assume it never existed before
        history.ind_h = -1;
    }
    fclose(f);
}

void preserve_history() {
    // create or overwrite history file
    // storing history datastructures as binary

    FILE* f = fopen(history_path, "wb");
    if (f == NULL) {
        fprintf(stderr, "Could not save history.\n");
        return;
    }
    if (fwrite(&history, sizeof(history), 1, f) != 1) {
        fprintf(stderr, "Could not save history.\n");
        return;
    }
}

void handle_history(int argc, char** argv) {
    if (argc > 2) {
        fprintf(stderr, "Could not print history: Invalid arguments.\n");
        return;
    }

    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc > 1) {
        fprintf(stderr, "Could not print history: Invalid arguments.\n");
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
    // store this command in history
    
    if (strlen(str) > MAX_STATIC_STR_LEN-1) {
        return;
    }

    if (history.ind_h >= 0 && strcmp(history.data[history.ind_h % 20], str) == 0) {
        return; //avoid consecutive duplicates
    }

    if (history.ind_h < 19) {
        history.ind_h++;
        strcpy(history.data[history.ind_h], str);
        return;
    }

    history.ind_h++;
    if (history.ind_h >= 40) {
        history.ind_h = history.ind_h - 20;
    }

    // history.ind_h >= 20 here
    strcpy(history.data[history.ind_h - 20], str);
}

void print_history(int n) {
    // print these many commands
    
    if (n <= 0) {
        return;
    }
    if (n > history.ind_h + 1) {
        n = history.ind_h + 1;
    }
    if (n > 20) {
        n = 20;
    }

    if (history.ind_h < 20) {
        for (int i = history.ind_h - n + 1; i <= history.ind_h; i++) {
            printf("%s\n", history.data[i]);
        }
        return;
    }

    // history.ind_h >= 20 here
    // history.ind_h - 20 gives index

    if (n <= history.ind_h - 20) {
        for (int i = history.ind_h - n -19; i <= history.ind_h - 20; i++) {
            printf("%s\n", history.data[i]);
        }
        return;
    }

    // n > in_h - 20 here
    // in_h >= 20 here
    int s = history.ind_h - n + 1;
    for (int i = 0; i < n; i++) {
        printf("%s\n", history.data[s]);
        s = (s + 1) % 20;
    }
}