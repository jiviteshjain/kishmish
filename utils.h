#ifndef __UTILS_H
#define __UTILS_H

#define space_delim " \t\r\n\v\f"

void init();
void goodbye();
char* trim(char* str, char c);
char* substr(const char* str, size_t start, size_t length);
size_t count_chars(const char* str, char c);
long long length_num(long long n);
char* num_to_str(long long n);
char* tilda_expand(char* str);
char* get_relative_pwd(char* dir);
char* get_home_dir();

#endif