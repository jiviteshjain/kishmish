#include "shell.h"
#include "utils.h"
#include "nightswatch.h"


void handle_nightswatch(int argc, char** argv) {
    if (argc < 1 || argc > 4) {
        printf("Could not watch: Invalid arguments.\n");
        return;
    }
    if (strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }
    if (argc == 1) {
        if (strcmp(argv[0], "interrupt") == 0) {
            nightswatch(0, 5);
        } else if (strcmp(argv[0], "dirty") == 0) {
            nightswatch(1, 5);
        } else {
            printf("Could not watch: Invalid arguments.\n");
            return;
        }
    } else if (argc == 3) {
        int interval;
        if (
            strcmp(argv[0], "-n") == 0 &&
            (interval = strtol(argv[1], NULL, 10)) > 0
        ) {
            if (strcmp(argv[2], "interrupt") == 0) {
                nightswatch(0, interval);
            } else if (strcmp(argv[2], "dirty") == 0) {
                nightswatch(1, interval);
            } else {
                printf("Could not watch: Invalid arguments.\n");
                return;
            }
        } else if (
            strcmp(argv[1], "-n") == 0 &&
            (interval = strtol(argv[2], NULL, 10)) > 0
        ) {
            if (strcmp(argv[0], "interrupt") == 0) {
                nightswatch(0, interval);
            } else if (strcmp(argv[0], "dirty") == 0) {
                nightswatch(1, interval);
            } else {
                printf("Could not watch: Invalid arguments.\n");
                return;
            }
        } else {
            printf("Could not watch: Invalid arguments.\n");
            return;
        }
    } else {
        printf("Could not watch: Invalid arguments.\n");
        return;
    }
}

bool night_interrupt() {
    FILE* f = fopen("/proc/interrupts", "r");
    if (f == NULL) {
        perror("Could not display interrupt info");
        return false;
    }
    char* temp = (char*)malloc(sizeof(char) * 100);
    int i = 0;
    while (fgets(temp, 1000, f) != NULL) {
        i++;
        if (i == 3) {
            temp[49] = '\0';
            printf("%s\n", temp + 5);
            break;
        }
    }
    free(temp);
    fclose(f);
    return true;
}

bool night_dirty() {
    FILE* f = fopen("/proc/meminfo", "r");
    if (f == NULL) {
        perror("Could not display memory info");
        return false;
    }
    char* temp = (char*)malloc(sizeof(char) * 100);
    int i = 0;
    while (fgets(temp, 1000, f) != NULL) {
        i++;
        if (i == 17) {
            strtok(temp, space_delim);
            char* t = strtok(NULL, space_delim);
            if (t == NULL){
                printf("Could not display memory info: Unable to read data.\n");
                free(temp);
                fclose(f);
                return false;
            }
            printf("%s ", t);
            t = strtok(NULL, space_delim);
            if (t == NULL) {
                printf("Could not display memory info: Unable to read data.\n");
                free(temp);
                fclose(f);
                return false;
            }
            printf("%s\n", t);
            break;
        }
    }
    free(temp);
    fclose(f);
    return true;
}

bool nightswatch(int comm, int interval) {
    if (comm == 0) {
        // INTERRUPTS
        printf(ANSI_GREEN "Watching keyboard interrupts...\n");
        printf("       CPU0       CPU1       CPU2       CPU3\n" ANSI_DEFAULT);
        while (true) {
            if (!night_interrupt())
                return false;
            if (get_me_out())
                return true;
            sleep(interval);
            if (get_me_out())
                return true;
        }
    } else {
        // DIRTY
        printf(ANSI_GREEN "Watching dirty memory...\n" ANSI_DEFAULT);
        while (true) {
            if (!night_dirty())
                return false;
            if (get_me_out())
                return true;
            sleep(interval);
            if (get_me_out())
                return true;
        }
    }
}

bool get_me_out() {
    bool can_read = false;

    struct timeval tv;
    tv.tv_usec = 0;
    tv.tv_sec = 0;

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    int ret = select(1, &rfds, NULL, NULL, &tv);
    if (ret < 0) {
        perror("Could not watch");
        return true; // asking to exit
    } else if (ret > 0) {
        can_read =  FD_ISSET(0, &rfds);
    } else {
        return false; // no data - don't exit
    }

    if (!can_read)
        return false; // no data - don't exit

    char c = fgetc(stdin);
    

    if (c == EOF) {
        return false;
    }
    fgetc(stdin);
    if (c == 'q') {
        return true;
    }
    return false;
}