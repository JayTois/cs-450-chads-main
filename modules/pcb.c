#include "mpx_supt.h"
#include <string.h>

#include "pcb.h"
#include "queue.h"
#include "helpers.h"

// ——————————————————————————————————————————————————

struct Queue *ready_queue;
struct Queue *blocked_queue;

/**
 * Initializes the ready and blocked queues
 */
void init_queues(){
    ready_queue = new_queue();
    blocked_queue = new_queue();
}

// ——————————————————————————————————————————————————

/**
 * Allocates memory for a process control block and initializes execution state and dispatching state
 * @return Process control block
 */
struct ProcessControlBlock *allocate_pcb() {
    struct ProcessControlBlock *pcb = sys_alloc_mem(sizeof(*pcb));

    pcb->execution_state = BLOCKED;
    pcb->dispatching_state = SUSPENDED;
    memset(pcb->stack, 0, sizeof(pcb->stack));

    return pcb;
}

// ——————————————————————————————————————————————————

/**
 * Frees memory associated with a process control block
 * @param pcb Process control block
 * @return Error code
 */
int free_pcb(struct ProcessControlBlock *pcb) {
    return sys_free_mem(pcb);
}

// ——————————————————————————————————————————————————

/**
 * Constructs a process control block, validating input in the process
 * @param name Process name
 * @param class Process class
 * @param priority Process priority
 * @return Process control block or NULL if input is invalid
 */
struct ProcessControlBlock *setup_pcb(char *name, int class, int priority) {
    if (find_pcb(name) != NULL
        || class < 0 || class > 1
        || priority < 0 || priority > 9) {
        return NULL;
    }

    struct ProcessControlBlock *pcb = allocate_pcb();

    strcpy(pcb->name, name);
    pcb->class = class;
    pcb->priority = priority;
    pcb->execution_state = READY;
    pcb->dispatching_state = NOT_SUSPENDED;

    return pcb;
}

// ——————————————————————————————————————————————————

/**
 * Finds the process with the given name
 * @param name Process name
 * @return Process control block or NULL if the process not found
 */
struct ProcessControlBlock *find_pcb(char *name) {
    struct ProcessControlBlock *pcb;

    if ((pcb = find(get_ready_queue(), name)) != NULL) {
        return pcb;
    } else if ((pcb = find(get_blocked_queue(), name)) != NULL) {
        return pcb;
    } else {
        return NULL;
    }
}

// ——————————————————————————————————————————————————

/**
 * Inserts a process control block into the appropriate queue
 * @param pcb Process control block
 */
void insert_pcb(struct ProcessControlBlock *pcb) {
    if (pcb->execution_state == READY) {
        enqueue_priority(ready_queue, pcb);
    } else if (pcb->execution_state == BLOCKED) {
        enqueue_fifo(blocked_queue, pcb);
    }
}

// ——————————————————————————————————————————————————

/**
 * Removes a process control block from the appropriate queue
 * @param pcb Process control block
 * @return Error code
 */
int remove_pcb(struct ProcessControlBlock *pcb) {
    if (pcb->execution_state == READY) {
        return remove(ready_queue, pcb);
    } else if (pcb->execution_state == BLOCKED) {
        return remove(blocked_queue, pcb);
    } else {
        return -1;
    }
}

// ——————————————————————————————————————————————————

/**
 * Gets the ready queue
 * @return Ready queue
 */
struct Queue *get_ready_queue() {
    return ready_queue;
}

// ——————————————————————————————————————————————————

/**
 * Gets the blocked queue
 * @return Blocked queue
 */
struct Queue *get_blocked_queue() {
    return blocked_queue;
}

// ——————————————————————————————————————————————————

/**
 * Gets the name of a process
 * @param pcb Process control block
 * @return Process name
 */
char *get_name(struct ProcessControlBlock *pcb) {
    return pcb->name;
}

// ——————————————————————————————————————————————————

/**
 * Gets the class of a process
 * @param pcb Process control block
 * @return Process class
 */
char *get_class(struct ProcessControlBlock *pcb) {
    if (pcb->class == SYSTEM) {
        return "SYSTEM";
    } else if (pcb->class == USER) {
        return "USER";
    } else {
        return NULL;
    }
}

// ——————————————————————————————————————————————————

/**
 * Gets the priority of a process
 * @param pcb Process control block
 * @return Process priority
 */
char *get_priority(struct ProcessControlBlock *pcb) {
    char *priority = " ";
    itoa(priority, pcb->priority);
    return priority;
}

// ——————————————————————————————————————————————————

/**
 * Gets the execution state of a process
 * @param pcb Process control block
 * @return Process execution state
 */
char *get_execution_state(struct ProcessControlBlock *pcb) {
    if (pcb->execution_state == RUNNING) {
        return "RUNNING";
    } else if (pcb->execution_state == READY) {
        return "READY";
    } else if (pcb->execution_state == BLOCKED) {
        return "BLOCKED";
    } else {
        return NULL;
    }
}

// ——————————————————————————————————————————————————

/**
 * Gets the dispatching state of a process
 * @param pcb Process control block
 * @return Process dispatching state
 */
char *get_dispatching_state(struct ProcessControlBlock *pcb) {
    if (pcb->dispatching_state == NOT_SUSPENDED) {
        return "NOT SUSPENDED";
    } else if (pcb->dispatching_state == SUSPENDED) {
        return "SUSPENDED";
    } else {
        return NULL;
    }
}

// ——————————————————————————————————————————————————

/**
 * Prints the name, class, priority, execution state, and dispatching state of a process
 * @param pcb Process control block
 */
void print_pcb(struct ProcessControlBlock *pcb) {
    print("——————————————————————————————————————————————————\n");

    print("Name                ");
    print(get_name(pcb));
    print("\n");

    print("Class               ");
    print(get_class(pcb));
    print("\n");

    print("Priority            ");
    print(get_priority(pcb));
    print("\n");

    print("Execution State     ");
    print(get_execution_state(pcb));
    print("\n");

    print("Dispatching State   ");
    print(get_dispatching_state(pcb));
    print("\n");

    print("——————————————————————————————————————————————————\n");
}
