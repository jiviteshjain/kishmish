#ifndef __PROCESS_H
#define __PROCESS_H

typedef struct {
    pid_t pid;
    char pname[MAX_STATIC_STR_LEN];
    int id; //UNSAFE TO READ
} proc;

typedef struct node {
    proc data;
    struct node* next;
} node;

node* processes;

node* insert_head(node* head, proc data);
node* insert_tail(node* head, proc data);
node* delete_node_by_pid(node* head, pid_t pid);
proc* get_data_by_pid(node* head, pid_t pid);
proc* get_data_by_id(node* head, int id);
void print_all(node* head);
void init_processes();
proc make_proc(pid_t pid, char* pname);
int store_process(pid_t pid, int argc, char** argv);

#endif
