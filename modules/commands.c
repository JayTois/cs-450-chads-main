#include <core/io.h>
#include <string.h>

#include "pcb.h"
#include "queue.h"
#include "helpers.h"
#include "mpx_supt.h"

#define RTC_IN_PORT 0x70
#define RTC_OUT_PORT 0x71

#define RTC_MONTH_REGISTER 0x08
#define RTC_DAY_REGISTER 0x07
#define RTC_YEAR_REGISTER 0x09

#define RTC_HOURS_REGISTER 0x04
#define RTC_MINUTES_REGISTER 0x02
#define RTC_SECONDS_REGISTER 0x00

/**
 * Prints a welcome message
 */
void welcome(void) {
    print("⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠛⠋⠉⠈⠉⠉⠉⠉⠛⠻⢿⣿⣿⣿⣿⣿⣿⣿\n"
          "⣿⣿⣿⣿⣿⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⢿⣿⣿⣿⣿\n"
          "⣿⣿⣿⣿⡏⣀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿\n"
          "⣿⣿⣿⢏⣴⣿⣷⠀⠀⠀⠀⠀⢾⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿\n"
          "⣿⣿⣟⣾⣿⡟⠁⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣷⢢⠀⠀⠀⠀⠀⠀⠀⢸⣿\n"
          "⣿⣿⣿⣿⣟⠀⡴⠄⠀⠀⠀⠀⠀⠀⠙⠻⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⣿\n"
          "⣿⣿⣿⠟⠻⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠶⢴⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⣿\n"
          "⣿⣁⡀⠀⠀⢰⢠⣦⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⡄⠀⣴⣶⣿⡄⣿\n"
          "⣿⡋⠀⠀⠀⠎⢸⣿⡆⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⠗⢘⣿⣟⠛⠿⣼\n"
          "⣿⣿⠋⢀⡌⢰⣿⡿⢿⡀⠀⠀⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣧⢀⣼\n"
          "⣿⣿⣷⢻⠄⠘⠛⠋⠛⠃⠀⠀⠀⠀⠀⢿⣧⠈⠉⠙⠛⠋⠀⠀⠀⣿⣿⣿⣿⣿\n"
          "⣿⣿⣧⠀⠈⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠟⠀⠀⠀⠀⢀⢃⠀⠀⢸⣿⣿⣿⣿\n"
          "⣿⣿⡿⠀⠴⢗⣠⣤⣴⡶⠶⠖⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡸⠀⣿⣿⣿⣿\n"
          "⣿⣿⣿⡀⢠⣾⣿⠏⠀⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠉⠀⣿⣿⣿⣿\n"
          "⣿⣿⣿⣧⠈⢹⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿\n"
          "⣿⣿⣿⣿⡄⠈⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣾⣿⣿⣿⣿⣿\n"
          "⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
          "⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
          "⣿⣿⣿⣿⣿⣦⣄⣀⣀⣀⣀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
          "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
          "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠙⣿⣿⡟⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
          "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠁⠀⠀⠹⣿⠃⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
          "⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⢐⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
          "⣿⣿⣿⣿⠿⠛⠉⠉⠁⠀⢻⣿⡇⠀⠀⠀⠀⠀⠀⢀⠈⣿⣿⡿⠉⠛⠛⠛⠉⠉\n"
          "⣿⡿⠋⠁⠀⠀⢀⣀⣠⡴⣸⣿⣇⡄⠀⠀⠀⠀⢀⡿⠄⠙⠛⠀⣀⣠⣤⣤⠄⠀\n"
          " ________  ___  ___  ________  ________       ________  ________      \n"
          "|\\   ____\\|\\  \\|\\  \\|\\   __  \\|\\   ___ \\     |\\   __  \\|\\   ____\\     \n"
          "\\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\_|\\ \\  __\\ \\  \\|\\  \\ \\  \\___|_    \n"
          " \\ \\  \\    \\ \\   __  \\ \\   __  \\ \\  \\ \\\\ \\|\\__\\ \\  \\\\\\  \\ \\_____  \\   \n"
          "  \\ \\  \\____\\ \\  \\ \\  \\ \\  \\ \\  \\ \\  \\_\\\\ \\|__|\\ \\  \\\\\\  \\|____|\\  \\  \n"
          "   \\ \\_______\\ \\__\\ \\__\\ \\__\\ \\__\\ \\_______\\    \\ \\_______\\____\\_\\  \\ \n"
          "    \\|_______|\\|__|\\|__|\\|__|\\|__|\\|_______|     \\|_______|\\_________\\\n"
          "                                                          \\|_________|\n"
          "Type 'help' for more information\n");
}

/**
 * Prints the version of chadOS
 */
void version(void) {
    print("R2\n");
}

/**
 * Prints a list of commands, their descriptions, and their usages
 */
void help(void) {
    print("The following commands are available:\n"
          "  COMMAND       DESCRIPTION                                  USAGE\n"
          "  —————————————————————————————————————————————————————————————————————————————————————\n"
          "  version       Gets the version of chadOS                   version\n"
          "  help          Gets the list of available commands          help\n"
          "  shutdown      Terminates chadOS                            shutdown\n"
          "  get_date      Gets the system date                         get_date\n"
          "  set_date      Sets the system date                         set_date mm/dd/yy\n"
          "  get_time      Gets the system time                         get_time\n"
          "  set_time      Sets the system time                         set_time hh:mm:ss\n"
          "  set_priority  Sets the priority of a process               set_priority name [0-9]{1}\n"
          "  suspend       Sets the dispatching state to SUSPENDED      suspend name\n"
          "  resume        Sets the dispatching state to NOT_SUSPENDED  resume name\n"
          "  show          Gets a process                               show name\n"
          "  show_ready    Gets all ready processes                     show_ready\n"
          "  show_blocked  Gets all blocked processes                   show_blocked\n"
          "  show_all      Gets all processes                           show_all\n"
          "  create        Constructs a process                         create name [SYSTEM, USER]{1} priority\n"
          "  delete        Destructs a process                          delete name\n"
          "  block         Sets the execution state to BLOCKED          block name\n"
          "  unblock       Sets the execution state to READY            unblock name\n");
}

// Binary Coded Decimal (BCD) uses 4 bits to represent each digit.
// e.g., 45 => 0100 (4) 0101 (5)
/**
 * Prints the system date
 */
void get_date(void) {
    outb(RTC_IN_PORT, RTC_MONTH_REGISTER);
    unsigned char month_bcd = inb(RTC_OUT_PORT);

    outb(RTC_IN_PORT, RTC_DAY_REGISTER);
    unsigned char day_bcd = inb(RTC_OUT_PORT);

    outb(RTC_IN_PORT, RTC_YEAR_REGISTER);
    unsigned char year_bcd = inb(RTC_OUT_PORT);

    int month = month_bcd - (6 * (month_bcd >> 4));
    int day = day_bcd - (6 * (day_bcd >> 4));
    int year = year_bcd - (6 * (year_bcd >> 4));

    char month_s[3] = {0};
    char day_s[3] = {0};
    char year_s[3] = {0};

    itoa(month_s, month);
    itoa(day_s, day);
    itoa(year_s, year);

    print(month_s);
    print("/");
    print(day_s);
    print("/");
    print(year_s);
    print("\n");
}

/**
 * Sets the system date
 * @param mm Month
 * @param dd Day
 * @param yy Year
 */
void set_date(int mm, int dd, int yy) {
    if (mm < 1 || mm > 12) {
        print("Invalid month\n"
              "Month must be between 1 (inclusive) and 12 (inclusive)\n");
        return;
    }

    if (dd < 1 || dd > 31) {
        print("Invalid day\n"
              "Day must be between 1 (inclusive) and 31 (inclusive)\n");
        return;
    }

    if (yy < 0 || yy > 99) {
        print("Invalid year\n"
              "Year must be between 0 (inclusive) and 99 (inclusive)\n");
        return;
    }

    unsigned char month_bcd = dectobcd(mm);
    unsigned char day_bcd = dectobcd(dd);
    unsigned char year_bcd = dectobcd(yy);

    outb(RTC_IN_PORT, RTC_MONTH_REGISTER);
    outb(RTC_OUT_PORT, month_bcd);

    outb(RTC_IN_PORT, RTC_DAY_REGISTER);
    outb(RTC_OUT_PORT, day_bcd);

    outb(RTC_IN_PORT, RTC_YEAR_REGISTER);
    outb(RTC_OUT_PORT, year_bcd);
}

/**
 * Prints the system time
 */
void get_time(void) {
    outb(RTC_IN_PORT, RTC_HOURS_REGISTER);
    unsigned char hours_bcd = inb(RTC_OUT_PORT);

    outb(RTC_IN_PORT, RTC_MINUTES_REGISTER);
    unsigned char minutes_bcd = inb(RTC_OUT_PORT);

    outb(RTC_IN_PORT, RTC_SECONDS_REGISTER);
    unsigned char seconds_bcd = inb(RTC_OUT_PORT);

    int hours = hours_bcd - (6 * (hours_bcd >> 4));
    int minutes = minutes_bcd - (6 * (minutes_bcd >> 4));
    int seconds = seconds_bcd - (6 * (seconds_bcd >> 4));

    char hours_s[3] = {0};
    char minutes_s[3] = {0};
    char seconds_s[3] = {0};

    // changing the time to eastern.
    // UTC is 4 hours ahead
    hours -= 4;

    itoa(hours_s, hours);
    itoa(minutes_s, minutes);
    itoa(seconds_s, seconds);

    print(hours_s);
    print(":");
    print(minutes_s);
    print(":");
    print(seconds_s);
    print("\n");
}

/**
 * Sets the system time
 * @param hh Hours
 * @param mm Minutes
 * @param ss Seconds
 */
void set_time(int hh, int mm, int ss) {

    /*because gettime subtracts 4 hours,
    we have to add the four hours here initially
    because we want our user to get the right time
    but we also want eastern standard time
    */
    hh += 4;

    if (hh < 0 || hh >= 24) {
        print("Invalid hours\n"
              "Hours must be between 0 (inclusive) and 24 (exclusive)\n");
        return;
    }

    if (mm < 0 || mm >= 60) {
        print("Invalid minutes\n"
              "Minutes must be between 0 (inclusive) and 60 (exclusive)\n");
        return;
    }

    if (ss < 0 || ss >= 60) {
        print("Invalid seconds\n"
              "Seconds must be between 0 (inclusive) and 60 (exclusive)\n");
        return;
    }

    // using our decimal to bcd helper method to convert
    unsigned char hours_bcd = dectobcd(hh);
    unsigned char minutes_bcd = dectobcd(mm);
    unsigned char seconds_bcd = dectobcd(ss);

    // it is outb twice because we are referencing which register we want to write to...
    // then we write to the reigster the value we want.

    outb(RTC_IN_PORT, RTC_HOURS_REGISTER);
    outb(RTC_OUT_PORT, hours_bcd);

    outb(RTC_IN_PORT, RTC_MINUTES_REGISTER);
    outb(RTC_OUT_PORT, minutes_bcd);

    outb(RTC_IN_PORT, RTC_SECONDS_REGISTER);
    outb(RTC_OUT_PORT, seconds_bcd);
}

// ——————————————————————————————————————————————————
// END R1
// BEGIN R2
// ——————————————————————————————————————————————————

// ——————————————————————————————————————————————————
// BEGIN R2 PERMANENT COMMANDS
// ——————————————————————————————————————————————————

/**
 * Sets the priority of the process
 * @param name Process name
 * @param priority Process priority
 */
void set_priority(char *name, int priority) {
    struct ProcessControlBlock *pcb = find_pcb(name);

    if (pcb == NULL) {
        print("The process control block with the given name does not exist.\n");
        return;
    }

    if (priority < 0 || priority > 9) {
        print("Invalid priority\n"
              "The priority must be between 0 (inclusive) and 9 (inclusive)\n");
    }

    pcb->priority = priority;

    if (pcb->execution_state == READY) {
        remove(get_ready_queue(), pcb); // This may cause an error because memory gets deallocated
        enqueue_priority(get_ready_queue(), pcb);
    }
}

// ——————————————————————————————————————————————————

/**
 * Puts the process in the suspended state
 * @param name Process name
 */
void suspend(char *name) {
    struct ProcessControlBlock *pcb = find_pcb(name);

    if (pcb == NULL) {
        print("The process control block with the given name does not exist.\n");
        return;
    }

    pcb->dispatching_state = SUSPENDED;
}

// ——————————————————————————————————————————————————

/**
 * Puts the process in the not suspended state
 * @param name Process name
 */
void resume(char *name) {
    struct ProcessControlBlock *pcb = find_pcb(name);

    if (pcb == NULL) {
        print("The process control block with the given name does not exist.\n");
        return;
    }

    pcb->dispatching_state = NOT_SUSPENDED;
}

// ——————————————————————————————————————————————————

/**
 * Displays the name, class, priority, execution status, and dispatching status
 * @param name Process name
 */
void show(char *name) {
    struct ProcessControlBlock *pcb = find_pcb(name);

    if (pcb == NULL) {
        print("The process control block with the given name does not exist.\n");
        return;
    }

    print_pcb(pcb);
}

// ——————————————————————————————————————————————————

/**
 * For each ready process, displays the name, class, priority, execution status, and dispatching status
 */
void show_ready() {
    struct Node *n = get_ready_queue()->front;
    while (n != NULL) {
        print_pcb(n->pcb);
        n = n->next;
    }
}

// ——————————————————————————————————————————————————

/**
 * For each blocked process, displays the name, class, priority, execution status, and dispatching status
 */
void show_blocked() {
    struct Node *n = get_blocked_queue()->front;
    while (n != NULL) {
        print_pcb(n->pcb);
        n = n->next;
    }
}

// ——————————————————————————————————————————————————

/**
 * For each process, displays the name, class, priority, execution status, and dispatching status
 */
void show_all() {
    show_ready();
    show_blocked();
}

// ——————————————————————————————————————————————————
// END R2 PERMANENT COMMANDS
// BEGIN R2 TEMPORARY COMMANDS
// ——————————————————————————————————————————————————

/**
 * Creates a new process using a default constructor and the provided arguments and inserts it into the blocked queue
 * @param name Process name
 * @param class Process class
 * @param priority Process priority
 */
void create(char *name, int class, int priority) {
    struct ProcessControlBlock *pcb = setup_pcb(name, class, priority);

    if (pcb == NULL) {
        print("The process control block with the given name already exists.\n");
        return;
    }

    enqueue_priority(get_ready_queue(), pcb);
}

// ——————————————————————————————————————————————————

/**
 * Deletes the process
 * @param name Process name
 */
void delete(char *name) { // Could be made more efficient by combining find and remove
    struct ProcessControlBlock *pcb = find_pcb(name);

    if (pcb == NULL) {
        print("The process control block with the given name does not exist.\n");
        return;
    }

    remove_pcb(pcb);
}

// ——————————————————————————————————————————————————

/**
 * Put the process in the blocked state
 * @param name Process name
 */
void block(char *name) {
    struct ProcessControlBlock *pcb = find(get_ready_queue(), name);
    if (pcb == NULL) {
        print("The process control block with the given name does not exist in the ready queue.\n");
        return;
    }
    pcb->execution_state = BLOCKED;
    remove(get_ready_queue(), pcb);
    enqueue_fifo(get_blocked_queue(), pcb);
}

// ——————————————————————————————————————————————————

/**
 * Puts the process in the ready state
 * @param name Process name
 */
void unblock(char *name) {
    struct ProcessControlBlock *pcb = find(get_blocked_queue(), name);
    if (pcb == NULL) {
        print("The process control block with the given name does not exist in the blocked queue.\n");
        return;
    }
    pcb->execution_state = READY;
    remove(get_blocked_queue(), pcb);
    enqueue_priority(get_ready_queue(), pcb);
}

// ——————————————————————————————————————————————————
// END R2 TEMPORARY COMMANDS
// ——————————————————————————————————————————————————

// ——————————————————————————————————————————————————
// END R2
// ——————————————————————————————————————————————————

// ——————————————————————————————————————————————————
// BEGIN R3 TEMPORARY COMMANDS
// ——————————————————————————————————————————————————

void yield(){
    sys_req(IDLE, 0, NULL, NULL);
}