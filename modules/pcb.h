#ifndef PCB_H
#define PCB_H

#include "queue.h"

#define SYSTEM 0
#define USER 1

#define RUNNING 0
#define READY 1
#define BLOCKED 2

#define NOT_SUSPENDED 0
#define SUSPENDED 1

struct ProcessControlBlock {
    // This was changed to a char array because a char pointer can cause major problems, according to the TA
    char name[10];
    int class, priority, execution_state, dispatching_state;
    char stack[1024];

    uint32_t *top;

};

void init_queues(void);
struct ProcessControlBlock *allocate_pcb();
int free_pcb(struct ProcessControlBlock *pcb);
struct ProcessControlBlock *setup_pcb(char *name, int class, int priority);
struct ProcessControlBlock *find_pcb(char *name);
void insert_pcb(struct ProcessControlBlock *pcb);
int remove_pcb(struct ProcessControlBlock *pcb);
struct Queue *get_ready_queue();
struct Queue *get_blocked_queue();
char *get_name(struct ProcessControlBlock *pcb);
char *get_class(struct ProcessControlBlock *pcb);
char *get_priority(struct ProcessControlBlock *pcb);
char *get_execution_state(struct ProcessControlBlock *pcb);
char *get_dispatching_state(struct ProcessControlBlock *pcb);
void print_pcb(struct ProcessControlBlock *pcb);

#endif
