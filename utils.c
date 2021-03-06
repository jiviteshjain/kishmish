#include "shell.h"
#include "utils.h"
#include "history.h"
#include "external.h"
#include "process.h"
#include "signals.h"


void init() {
    // initialise everything here

    home_dir = get_home_dir();
    user_name = getlogin();  // Statically declared. Don't free() this
    host_name = (char*)malloc(sizeof(char) * (HOST_NAME_MAX + 1));
    gethostname(host_name, (HOST_NAME_MAX + 1));

    SHELL_PID = getpid();
    FG_CHILD_PID = -1;
    FG_CHILD_PNAME[0] = '\0';

    init_signals();
    init_history();
    init_processes();

    printf(ANSI_CLEAR);
    printf(ANSI_YELLOW_BOLD "\n\t***\tWelcome to Kishmish shell\t***\t\n");
    printf(ANSI_YELLOW "\t  Because everything happens for a raisin\t    \t\n\n" ANSI_DEFAULT);
}

void goodbye() {
    // cleanup and exit
    // Save history, print msg, free pointers, exit
    
    printf(ANSI_YELLOW_BOLD "\nGoodbye!\n" ANSI_YELLOW "This shell was made with a tinge of love, lots of sweetness, and of course, a ton of kishmish :)\n" ANSI_DEFAULT);
    preserve_history();
    free(home_dir);
    free(host_name);
    exit(EXIT_SUCCESS);
}

char* trim(char* str, char c) {
    // trims occurences of c from front and back
    // does not modify original pointer, returns new pointer,
    // but modifies original string. use original pointer to free

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
    // returns a substring of str starting at start and of length characters
    // dynamically allocates and returns a new copy, caller must free
    // does not modify old copy or pointers

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
    // counts the number of occurences of c in str

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
    // converts an integer to its base 10 string representation
    // dynamically allocates and returns a new string, caller must free

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
    // expands the tilda in front of a path into home_dir
    // if path contains a tilda, allocates and returns a new string, caller must free
    // else, returns the same pointer, unmodified
    // gauranteed to not modify original string or pointer

    if (str[0] != '~')
        return str;

    ssize_t len = strlen(str) + strlen(home_dir); // +1 for '\0' -1 for '~'
    char* final = (char*)malloc(sizeof(char) * len);
    strcpy(final, home_dir);
    strcat(final, str + 1);
    return final;
}

char* get_home_dir() {
    // gets the current home directory
    // dynamically allocates and returns a pointer to a new string, caller must free

    pid_t pid = getpid();
    char* read_path = (char*)malloc(sizeof(char) * MAX_STATIC_STR_LEN);
    sprintf(read_path, "/proc/%d/exe", pid);
    
    char* temp = (char*)malloc(sizeof(char) * MAX_STATIC_STR_LEN);
    ssize_t exec_len = readlink(read_path, temp, MAX_STATIC_STR_LEN-1);

    if (exec_len < 0) {
        free(read_path);
        free(temp);
        return getcwd(NULL, 0);
    }
    temp[exec_len] = '\0';
    for (int i = exec_len - 1; i >= 0; i--) {
        if (temp[i] == '/') {
            temp[i] = '\0';
            break;
        }
    }
    free(read_path);
    return temp;
}

char* get_relative_pwd(char* dir) {
    // convert dir (assumed absolute) to path relative to home_dir
    // returns dir if dir is not a sub-directory of home_dir
    // in either case dynamically allocates and returns pointer to a new string, caller must free

    char* cur_dir = (char*)malloc(sizeof(char) * (strlen(dir) + 1));
    strcpy(cur_dir, dir);
    size_t cur_len = strlen(cur_dir);
    size_t home_len = strlen(home_dir);

    if (strcmp(cur_dir, home_dir) == 0) {
        free(cur_dir);
        // Allocate dynamically to allow caller to free it
        char* temp = (char*)malloc(sizeof(char) * 2);
        temp[0] = '~';
        temp[1] = '\0';
        return temp;
    }

    char* first_part = substr(cur_dir, 0, home_len);
    if (first_part != NULL && strcmp(first_part, home_dir) == 0) {
        size_t first_len = strlen(first_part);
        if (first_len < cur_len && (cur_dir[first_len] == '/' || cur_dir[first_len] == '\0')) {
            char* second_part = substr(cur_dir, home_len, cur_len - home_len);
            char* relative_pwd = (char*)malloc(sizeof(char) * (strlen(second_part) + 2));
            relative_pwd[0] = '~';
            relative_pwd[1] = '\0';
            strcat(relative_pwd, second_part);

            free(cur_dir);
            free(first_part);
            free(second_part);
            return relative_pwd;
        }
    }
    free(first_part);
    return cur_dir;
}

char* get_full_command(char* command, int argc, char** argv) {
    // combines command and its arguments into a single space-separated string
    // dynamically allocates and returns pointer to a new string, must be freed by caller
    
    char* temp = (char*)malloc(sizeof(char) * MAX_STATIC_STR_LEN);
    strcpy(temp, command);
    size_t len = strlen(temp);

    for (int i = 0; i < argc; i++) {
        len = len + strlen(argv[i]) + 1;  // 1 for space
        if (len >= MAX_STATIC_STR_LEN - 1) { // 1 for null character
            break;
        }
        strcat(temp, argv[i]);
        strcat(temp, " ");
    }

    len = strlen(temp);  // get the exact length
    if (temp[len-1] == ' ')
        temp[len - 1] = '\0';
    return temp;
}