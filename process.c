#include "shell.h"
#include "utils.h"
#include "process.h"

// processes are stored as a linked list of proc data structures
// internal ids assigned to the processes are in ascending order of
// birth times, only including the processes that are still alive
// thus, they can change if other processes die
// process id is used throughout for unique identification of a process

// LINKED LIST FUNCTIONS

node* insert_head(node* head, proc data) {
    node* new = (node*)malloc(sizeof(node));
    new->data = data;

    if (head == NULL) {
        new->next = NULL;
        return new;
    }

    new->next = head;
    return new;
}

node* insert_tail(node* head, proc data) {
    node* new = (node*)malloc(sizeof(node));
    new->data = data;
    new->next = NULL;

    if (head == NULL) {
        return new;
    }

    node* cur = head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = new;
    return head;
}

node* delete_node_by_pid(node* head, pid_t pid) {
    if (head == NULL) {
        return NULL;
    }

    if (head->data.pid == pid) {
        node* temp = head->next;
        free(head);
        return temp;
    }

    node* cur = head->next;
    node* prev = head;

    while (cur != NULL) {
        if (cur->data.pid == pid) {
            prev->next = cur->next;
            free(cur);
            return head;
        }
        prev = cur;
        cur = cur->next;
    }
    return head;
}

proc* get_data_by_pid(node* head, pid_t pid) {
    int i = 0;
    while (head != NULL) {
        i++;
        if (head->data.pid == pid) {
            head->data.id = i;
            return &(head->data);
        }
        head = head->next;
    }
    return NULL;
}

proc* get_data_by_id(node* head, int id) {
    int i = 0;
    while (head != NULL) {
        i++;
        if (i == id) {
            head->data.id = i;
            return &(head->data);
        }
        head = head->next;
    }
    return NULL;
}

void print_all(node* head) {
    int i = 0;
    while (head != NULL) {
        i++;
        printf("[%d] %s %d\n", i, head->data.pname, head->data.pid);
        head = head->next;
    }
}

// PROCESS FUNCTIONS
void init_processes() {
    processes = NULL;
}

proc make_proc (pid_t pid, char* pname) {
    proc p;
    p.id = -1;
    p.pid = pid;
    strcpy(p.pname, pname);
    return p;
}

int store_process(pid_t pid, char* temp) {

    processes = insert_tail(processes, make_proc(pid, temp));
    
    proc* val = get_data_by_pid(processes, pid);
    if (val != NULL)
        return val->id;
    else
        return -1;
}
