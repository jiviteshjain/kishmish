#ifndef __SHELL_H
#define __SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <time.h>

#ifndef __USE_POSIX
#define __USE_POSIX
#endif
#include <limits.h>

#define MAX_STATIC_STR_LEN 1024

#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_RED_BOLD "\033[1;31m"
#define ANSI_GREEN_BOLD "\033[1;32m"
#define ANSI_YELLOW_BOLD "\033[1;33m"
#define ANSI_CYAN_BOLD "\033[1;36m"
#define ANSI_DEFAULT "\033[0m"
#define ANSI_CLEAR "\033[2J\033[1;1H"

char* home_dir;
char* user_name;
char* host_name;

pid_t SHELL_PID;
pid_t FG_CHILD_PID;
char FG_CHILD_PNAME[MAX_STATIC_STR_LEN];
#endif
