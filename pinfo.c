#include "shell.h"
#include "utils.h"
#include "pinfo.h"

void handle_pinfo(int argc, char** argv) {
    if (argc > 0 && strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }

    if (argc == 0) {
        pinfo(-1);
        return;
    }

    if (argc > 1 && argc < 0) {
        printf("Could not list process details: Invalid arguments.\n");
        return;
    }

    int pid = strtol(argv[0], NULL, 10);
    if (pid < 0) {
        printf("Could not list process details: Invalid arguments.\n(PS.: Process IDs are non-negative)\n");
        return;
    }
    pinfo(pid);
}

bool pinfo(pid_t pid) {
    if (pid < 0) {
        pid = getpid();
    }
    char* stat_path = (char*)malloc(sizeof(char) * (15 + length_num(pid)));
    // process ids are >= 0, so length_num works fine (it ignores '-' sign)
    sprintf(stat_path, "/proc/%d/status", pid);

    char* p_name = (char*)malloc(sizeof(char) * 17); // MAX_SIZE is 16
    char p_status;
    char* p_mem = (char*)malloc(sizeof(char) * 100); // should be enough
    char* temp = (char*)malloc(sizeof(char) * 1024); // should be enough
    
    FILE* f = fopen(stat_path, "r");
    if (f == NULL) {
        perror("Could not list process details");
        return false;
    }

    int i = 0;
    while (fgets(temp, 1000, f) != NULL) {
        i++;
        if (i == 1) {
            // Process name
            strtok(temp, space_delim);
            char* t = strtok(NULL, space_delim);
            if (t == NULL)
                continue;
            strcpy(p_name, t);
        } else if (i == 3) {
            // Status
            strtok(temp, space_delim);
            char* t = strtok(NULL, space_delim);
            if (t == NULL)
                continue;
            p_status = t[0];
        } else if (i == 18) {
            // Virtual memory size
            strtok(temp, space_delim);
            
            char* t = strtok(NULL, space_delim);
            if (t == NULL)
                break;
            strcpy(p_mem, t);

            t = strtok(NULL, space_delim);
            if (t == NULL)
                break;
            strcat(p_mem, t);

            break;
        }
    }
    fclose(f);
    sprintf(stat_path, "/proc/%d/exe", pid);

    ssize_t exec_len = readlink(stat_path, temp, 1000);
    if (exec_len < 0) {
        perror("Could not list process details");
        return false;
    }
    temp[exec_len] = '\0';

    // All data available

    printf("PROCESS: %s\tID: %d\n", p_name, pid);
    printf("PROCESS STATUS: %c\n", p_status);
    printf("VIRTUAL MEMORY: %s\n", p_mem);
    printf("EXECUTABLE PATH: %s\n", temp);

    free(temp);
    free(p_name);
    free(p_mem);
    free(stat_path);

    return true;
}