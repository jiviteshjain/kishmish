#include "shell.h"
#include "utils.h"

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
