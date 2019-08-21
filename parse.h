#ifndef __PARSE_H
#define __PARSE_H

const char* whitespace_delim = " \t\r\n\v\f";

void parse_command(char* str);
void parse(char* input);

#endif
