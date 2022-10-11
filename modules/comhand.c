#include "helpers.h"
#include <string.h>
#include "mpx_supt.h"
#include "commands.h"
#include "pcb.h"

#define INPUT_BUFFER_SIZE 32 // Refactoring could be done on code that uses *_BUFFER_SIZE
#define RESPONSE_BUFFER_SIZE 2

/**
 * Parses user input, selects the proper command, performs syntax validation, and calls the command
 */
void comhand(void) {
    welcome();

    while (1) {
        print(">>> ");

        char input_buffer[INPUT_BUFFER_SIZE] = {0};
        int input_buffer_size = INPUT_BUFFER_SIZE;
        sys_req(READ, DEFAULT_DEVICE, input_buffer, &input_buffer_size);

        print("\n");

        char *delim_cmd = " ";
        char *cmd = strtok(input_buffer, delim_cmd);

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "version") == 0) {
            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'version'\n"
                      "Usage: version\n");
                continue;
            }

            version();
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "help") == 0) {
            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'help'\n"
                      "Usage: help\n");
                continue;
            }

            help();
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "shutdown") == 0) {
            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'shutdown'\n"
                      "Usage: shutdown\n");
                continue;
            }

            print("Are you sure you want to shutdown [Y/n]? ");

            char response_buffer[RESPONSE_BUFFER_SIZE] = {0};
            int response_buffer_size = RESPONSE_BUFFER_SIZE;
            sys_req(READ, DEFAULT_DEVICE, response_buffer, &response_buffer_size);

            print("\n");

            if (strcmp(response_buffer, "Y") == 0) {
                break;
            }

            print("Aborting...\n");
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "get_date") == 0) {
            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'get_date'\n"
                      "Usage: get_date\n");
                continue;
            }

            get_date();
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "set_date") == 0) {
            char *date = strtok(NULL, delim_cmd);

            if (date == NULL) {
                print("Too few arguments for command 'set_date'\n"
                      "Usage: set_date mm/dd/yyyy\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'set_date'\n"
                      "Usage: set_date mm/dd/yyyy\n");
                continue;
            }

            char *delim_date = "/";
            char *mm_s = strtok(date, delim_date);
            char *dd_s = strtok(NULL, delim_date);
            char *yyyy_s = strtok(NULL, delim_date);

            if (mm_s == NULL || dd_s == NULL || yyyy_s == NULL) {
                print("Too few arguments for parameter 'mm/dd/yyyy'\n"
                      "Usage: set_date mm/dd/yyyy\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for parameter 'mm/dd/yyyy'\n"
                      "Usage: set_date mm/dd/yyyy\n");
                continue;
            }

            int mm = atoi(mm_s);
            int dd = atoi(dd_s);
            int yyyy = atoi(yyyy_s);
            set_date(mm, dd, yyyy);
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "get_time") == 0) {
            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'get_time'\n"
                      "Usage: set_time\n");
                continue;
            }

            get_time();
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "set_time") == 0) {
            char *time = strtok(NULL, delim_cmd);

            if (time == NULL) {
                print("Too few arguments for command 'set_time'\n"
                      "Usage: set_time hh:mm:ss\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'set_time'\n"
                      "Usage: set_time hh:mm:ss\n");
                continue;
            }

            char *delim_time = ":";
            char *hh_s = strtok(time, delim_time);
            char *mm_s = strtok(NULL, delim_time);
            char *ss_s = strtok(NULL, delim_time);

            if (hh_s == NULL || mm_s == NULL || ss_s == NULL) {
                print("Too few arguments for parameter 'hh:mm:ss'\n"
                      "Usage: set_time hh:mm:ss\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for parameter 'hh:mm:ss'\n"
                      "Usage: set_time hh:mm:ss\n");
                continue;
            }

            int hh = atoi(hh_s);
            int mm = atoi(mm_s);
            int ss = atoi(ss_s);
            set_time(hh, mm, ss);
            continue;
        }

        // ——————————————————————————————————————————————————
        // END R1
        // BEGIN R2
        // ——————————————————————————————————————————————————

        // ——————————————————————————————————————————————————
        // BEGIN R2 PERMANENT COMMANDS
        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "set_priority") == 0) {
            char *name = strtok(NULL, delim_cmd);
            char *priority_s = strtok(NULL, delim_cmd);

            if (name == NULL || priority_s == NULL) {
                print("Too few arguments for command 'set_priority'\n"
                      "Usage: set_priority name [0-9]{1}\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'set_priority'\n"
                      "Usage: set_priority name [0-9]{1}\n");
                continue;
            }

            int priority = atoi(priority_s);
            set_priority(name, priority);
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "suspend") == 0) {
            char *name = strtok(NULL, delim_cmd);

            if (name == NULL) {
                print("Too few arguments for command 'suspend'\n"
                      "Usage: suspend name\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'suspend'\n"
                      "Usage: suspend name\n");
                continue;
            }

            suspend(name);
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "resume") == 0) {
            char *name = strtok(NULL, delim_cmd);

            if (name == NULL) {
                print("Too few arguments for command 'resume'\n"
                      "Usage: resume name\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'resume'\n"
                      "Usage: resume name\n");
                continue;
            }

            resume(name);
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "show") == 0) {
            char *name = strtok(NULL, delim_cmd);

            if (name == NULL) {
                print("Too few arguments for command 'show'\n"
                      "Usage: show name\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'show'\n"
                      "Usage: show name\n");
                continue;
            }

            show(name);
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "show_ready") == 0) {
            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'show_ready'\n"
                      "Usage: show_ready\n");
                continue;
            }

            show_ready();
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "show_blocked") == 0) {
            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'show_blocked'\n"
                      "Usage: show_blocked\n");
                continue;
            }

            show_blocked();
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "show_all") == 0) {
            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'show_all'\n"
                      "Usage: show_all\n");
                continue;
            }

            show_all();
            continue;
        }

        // ——————————————————————————————————————————————————
        // END R2 PERMANENT COMMANDS
        // BEGIN R2 TEMPORARY COMMANDS
        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "create") == 0) {
            char *name = strtok(NULL, delim_cmd);
            char *class_s = strtok(NULL, delim_cmd);
            char *priority_s = strtok(NULL, delim_cmd);

            if (name == NULL || class_s == NULL || priority_s == NULL) {
                print("Too few arguments for command 'create'\n"
                      "Usage: create name [SYSTEM, USER]{1} priority\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'create'\n"
                      "Usage: create name [SYSTEM, USER]{1} priority\n");
                continue;
            }

            int class = -1;

            if (strcmp(class_s, "USER") == 0) {
                class = USER;
            } else if (strcmp(class_s, "SYSTEM") == 0) {
                class = SYSTEM;
            }

            if (class == -1) {
                print("Syntax error on argument 'class' for command 'create'\n"
                      "Usage: create name [SYSTEM, USER]{1} priority\n");
                continue;
            }

            int priority = atoi(priority_s);
            create(name, class, priority);
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "delete") == 0) {
            char *name = strtok(NULL, delim_cmd);

            if (name == NULL) {
                print("Too few arguments for command 'delete'\n"
                      "Usage: delete name\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'delete'\n"
                      "Usage: delete name\n");
                continue;
            }

            delete(name);
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "block") == 0) {
            char *name = strtok(NULL, delim_cmd);

            if (name == NULL) {
                print("Too few arguments for command 'block'\n"
                      "Usage: block name\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'block'\n"
                      "Usage: block name\n");
                continue;
            }

            block(name);
            continue;
        }

        // ——————————————————————————————————————————————————

        if (strcmp(cmd, "unblock") == 0) {
            char *name = strtok(NULL, delim_cmd);

            if (name == NULL) {
                print("Too few arguments for command 'unblock'\n"
                      "Usage: unblock name\n");
                continue;
            }

            if (strtok(NULL, delim_cmd) != NULL) {
                print("Too many arguments for command 'unblock'\n"
                      "Usage: unblock name\n");
                continue;
            }

            unblock(name);
            continue;
        }

        // ——————————————————————————————————————————————————
        // END R2 TEMPORARY COMMANDS
        // ——————————————————————————————————————————————————

        // ——————————————————————————————————————————————————
        // END R2
        // ——————————————————————————————————————————————————

        print("Unrecognized command\n"
              "Type 'help' for more information\n");
    }
}