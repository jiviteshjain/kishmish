#include "shell.h"
#include "utils.h"
#include "history.h"

void init() {
    home_dir = getcwd(NULL, 0);
    user_name = getlogin();  // Statically declared. Don't free() this
    host_name = (char*)malloc(sizeof(char) * (HOST_NAME_MAX + 1));
    gethostname(host_name, (HOST_NAME_MAX + 1));
    init_history();
    printf(ANSI_YELLOW_BOLD "Welcome to Kishmish - a sweet little shell.\n" ANSI_DEFAULT);
}

void goodbye() {
    // Cleanup function
    // Save history, print msg, free pointers, exit
    printf(ANSI_YELLOW_BOLD "\nThank you for using Kishmish!\nThis shell was made with a tinge of love, lots of sweetness, and of course, a ton of kishmish :)\n" ANSI_DEFAULT);
    preserve_history();
    free(home_dir);
    free(host_name);
    exit(EXIT_SUCCESS);
}

char* trim(char* str, char c) {
    // trims occurences of c from front and back
    // does not modify original pointer, returns new pointer, but modifies original string. Use original pointer to free
    char* t = str;
    while (*t == c) {
        t++;
    }

    size_t i = strlen(t) - 1;
    while (t[i] == c) {
        t[i] = '\0';
        i--;
    }
    return t;
}

char* substr (const char* str, size_t start, size_t length) {
    long long total_len = strlen(str);
    if (start + length > total_len) {
        return NULL;
    }
    char* sub_str = (char*)malloc(sizeof(char) * (length + 1));
    for (long long i = 0; i < length; i++) {
        sub_str[i] = str[i + start];
    }
    sub_str[length] = '\0';
    return sub_str;
}

size_t count_chars (const char* str, char c) {
    size_t len = strlen(str);
    size_t count = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == c) {
            count++;
        }
    }
    return count;
}

long long length_num(long long n) {
    // Returns number of digits in abs(n)
    if (n == 0) {
        return 1;
    }

    if (n < 0) {
        n = -n;
    }

    long long len = 0;

    while (n > 0) {
        n = n / 10;
        len++;
    }
    return len;
}

char* num_to_str(long long n) {
    long long len = length_num(n);

    char* str = (char*)malloc(sizeof(char) * (len + (long long)(n < 0) + 1));

    long long m = (n < 0 ? -n : n);
    long long i = 0;
    for (; i < len; i++) {
        str[i] = (m % 10) + '0';
        m = m / 10;
    }

    if (n < 0) {
        str[i] = '-';
        str[i + 1] = '\0';
    } else {
        str[i] = '\0';
        i--;
    }

    long long j = 0;
    while (i > j) {
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
        i--;
        j++;
    }
    return str;
}

char* tilda_expand(char* str) {
    if (str[0] != '~')
        return str;

    ssize_t len = strlen(str) + strlen(home_dir); // +1 for '\0' -1 for '~'
    char* final = (char*)malloc(sizeof(char) * len);
    strcpy(final, home_dir);
    strcat(final, str + 1);
    return final;
}