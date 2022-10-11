#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

struct Node {
    struct ProcessControlBlock *pcb;
    struct Node *next;
};

struct Queue {
    struct Node *front;
    struct Node *rear;
};

struct Node *new_node(struct ProcessControlBlock *pcb);
struct Queue *new_queue();
struct ProcessControlBlock *find(struct Queue *q, char *name);
void enqueue_priority(struct Queue *q, struct ProcessControlBlock *pcb);
void enqueue_fifo(struct Queue *q, struct ProcessControlBlock *pcb);
struct ProcessControlBlock *dequeue(struct Queue *q);
int remove(struct Queue *q, struct ProcessControlBlock *pcb);

#endif
