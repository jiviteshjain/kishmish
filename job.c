#include "shell.h"
#include "utils.h"
#include "process.h"
#include "job.h"

char get_proc_state(pid_t pid) {
    char* stat_path = (char*)malloc(sizeof(char) * (15 + length_num(pid)));
    sprintf(stat_path, "/proc/%d/status", pid);

    FILE* f = fopen(stat_path, "r");
    if (f == NULL) {
        perror("Could not read process state");
        free(stat_path);
        return '\0';
    }

    char* temp = (char*)malloc(sizeof(char) * MAX_STATIC_STR_LEN);

    char p_status;

    int i = 0;
    while (fgets(temp, MAX_STATIC_STR_LEN - 1, f) != NULL) {
        i++;
        if (i == 3) {
            // Status
            strtok(temp, space_delim);
            char* t = strtok(NULL, space_delim);
            if (t == NULL) {
                printf("Could not read process state: Data unavailable.\n");
                p_status = '\0';
            } else {
                p_status = t[0];
            }
            break;
        }
    }

    free(stat_path);
    free(temp);
    fclose(f);
    return p_status;
}

void handle_jobs(int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc != 0) {
        printf("Could not list jobs: Invalid arguments.\n");
        return;
    }

    jobs();
}

bool jobs() {
    node* cur = processes;
    int i = 0;
    while (cur != NULL) {
        i++;
        
        printf("[%d] ", i);
        char p_state = get_proc_state(cur->data.pid);
        if (p_state == '\0') {
            return false; // error printed by get_proc_state
        } else if (p_state == 'T') {
            printf("Stopped ");
        } else {
            printf("Running ");
        }
        printf("%s %d\n", cur->data.pname, cur->data.pid);

        cur = cur->next;
    }
    return true;
}