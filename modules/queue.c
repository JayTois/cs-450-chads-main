#include "mpx_supt.h"
#include <string.h>

#include "queue.h"
#include "pcb.h"

// ——————————————————————————————————————————————————

/**
 * Constructs a node
 * @param pcb Process control block that the node will reference
 * @return Node
 */
struct Node *new_node(struct ProcessControlBlock *pcb) {
    struct Node *n = (struct Node *) sys_alloc_mem(sizeof(struct Node));

    n->pcb = pcb;
    n->next = NULL;

    return n;
}

// ——————————————————————————————————————————————————

/**
 * Constructs a queue
 * @return Queue
 */
struct Queue *new_queue() {
    struct Queue *q = sys_alloc_mem(sizeof(struct Queue));
    q->front = q->rear = NULL;

    return q;
}

// ——————————————————————————————————————————————————

// WARNING: Confusing
/**
 * Enqueues the process control block into the queue using its priority
 * @param q Queue
 * @param pcb Process control block
 */
void enqueue_priority(struct Queue *q, struct ProcessControlBlock *pcb) {
    struct Node *n_node = new_node(pcb);

    //suspended queues are last in the pirority
    //for example, 
    /* pcb_1 pcb_2 pcb_3
    * all with a priority of 1
    * pcb_1 is suspended
    * it must be arranged as...
    * pcb_2 pcb_3 pcb_1
    */

    // If the queue is empty
    // Then the new node is both the front and the rear
    if (q->front == NULL) {
        q->front = q->rear = n_node;
        return;
    }

    // Check if inserting the new node in front of the first node is appropriate
    // This needs to be a special case for two reasons
    // 1. The pointer to the front of the queue needs changed
    // 2. The while loop must start at the second node since there is no prev pointer
    // Instead n->next is acting as n and n is acting as n->prev
    if (pcb->priority < q->front->pcb->priority) {
        n_node->next = q->front;
        q->front = n_node;
        return;
    }

    // Check if inserting in front of the rest of the nodes is appropriate
    struct Node *n = q->front;
    while (n->next != NULL) {
        if (pcb->priority < n->next->pcb->priority) {
            n_node->next = n->next;
            n->next = n_node;
            return;
        }

        n = n->next;
    }

    // If it was not appropriate to insert the new node in front of any nodes
    // Then the new node is inserted at the rear
    q->rear->next = n_node;
    q->rear = n_node;
    return;
}

// ——————————————————————————————————————————————————

/**
 * Enqueues the process control block using the traditional first in, first out method
 * @param q Queue
 * @param pcb Process control block
 */
void enqueue_fifo(struct Queue *q, struct ProcessControlBlock *pcb) {
    struct Node *n_node = new_node(pcb);

    if (q->front == NULL) {
        q->front = q->rear = n_node;
        return;
    }

    q->rear->next = n_node;
    q->rear = n_node;
}
// ——————————————————————————————————————————————————

/**
 * Dequeues a process control block from the queue
 * @param q Queue
 */
struct ProcessControlBlock *dequeue(struct Queue *q) {
    if (q->front == NULL) {
        return NULL;
    }

    struct Node *n = q->front;
    q->front = q->front->next;
    sys_free_mem(n);

    if (q->front == NULL) {
        q->rear = NULL;
    }

    return n->pcb;
}

// ——————————————————————————————————————————————————

/**
 * Finds the process with the given name in a queue
 * @param q Queue
 * @param name Process name
 * @return Process control block
 */
struct ProcessControlBlock *find(struct Queue *q, char *name) {
    struct Node *n = q->front;
    while (n != NULL) {
        if (strcmp(n->pcb->name, name) == 0) {
            return n->pcb;
        }

        n = n->next;
    }

    return NULL;
}

// ——————————————————————————————————————————————————

/**
 * Removes the process control block from the queue
 * @param q Queue
 * @param pcb Process control block
 * @return Error code
 */
int remove(struct Queue *q, struct ProcessControlBlock *pcb) {
   // Node *n_node;

    // If the queue is empty then there is nothing to remove
    if (q->front == NULL) {
        return -1;
    }

    // Special case for the front node
    if (q->front->pcb == pcb) {
        struct Node *n = q->front;
        q->front = q->front->next;
        sys_free_mem(n);
        return 0;
    }

    // The while loop must start at the second node since there is no prev pointer
    // prev: n, n: n->next
    struct Node *n = q->front;
    struct Node *p;
    p = NULL;
    while (n != NULL) {
        if (n->pcb == pcb) {
            // Special case for the rear node
            // rear node case might have to be changed up
            if (n == q->rear) {
                p->next = NULL;
                q->rear = p;
                sys_free_mem(n);
                return 0;
            }
            else {
                p->next = n->next;   
                n->next =NULL;
            //n->next = n->next->next;
                sys_free_mem(n);
                return 0;
            }
        }
        p = n;
        n = n->next;
    }

    return -1;
}
