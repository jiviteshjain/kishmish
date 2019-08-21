#include "shell.h"
#include "utils.h"
#include "echo.h"

void handle_echo (int argc, char** argv) {
    if (argc == 0) {
        return;
    }

    if (strcmp(argv[argc-1], "&") == 0) {
        argc--;
    }
    char** trimmed_argv = (char**)malloc(sizeof(char*) * argc);

    for (int i = 0; i < argc; i++) {
        trimmed_argv[i] = trim(argv[i], '\"');
        trimmed_argv[i] = trim(trimmed_argv[i], '\'');
    }

    echo(trimmed_argv, argc);
    free(trimmed_argv); // these are not new strings, just new pointers because trim moves them.
    // original strings and argv freed by caller
}

bool echo(char** strs, long long n) {
    // All sanitization, such as removing \n etc. is done by parsers
    for (long long i = 0; i < n - 1; i++) {
        printf("%s ", strs[i]);
    }
    printf("%s\n", strs[n - 1]);
    return true;
}