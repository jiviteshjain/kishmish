#ifndef __RECALL_H
#define __RECALL_H

#define UP_0 (char)27
#define UP_1 (char)91
#define UP_2 (char)65

bool is_recall(char* command);
void handle_recall(char* command, int argc, char** argv);
bool recall(int up);

#endif
