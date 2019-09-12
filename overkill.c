#include "shell.h"
#include "utils.h"
#include "process.h"
#include "overkill.h"

void handle_overkill(int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc != 0) {
        printf("Could not kill background processes: Invalid arguments.\n");
        return;
    }

    overkill();
}

bool overkill() {
    node* cur = processes;
    while (cur != NULL) {
        kill(cur->data.pid, SIGKILL);
        usleep(1000);
        cur = cur->next;
    }
    return true;
}