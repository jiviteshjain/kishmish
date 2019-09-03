#ifndef __SETENV_H
#define __SETENV_H

void handle_setenv(int argc, char** argv);
bool set_env(char* name, char* val);

void handle_unsetenv(int argc, char** argv);
bool unset_env(char* name);

#endif
