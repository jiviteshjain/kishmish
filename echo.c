#include "shell.h"
#include "utils.h"
#include "echo.h"

bool echo(char** strs, long long n) {
    // All sanitization, such as removing \n etc. is done by parsers
    for (long long i = 0; i < n - 1; i++) {
        printf("%s ", strs[i]);
    }
    printf("%s\n", strs[n - 1]);
    return true;
}