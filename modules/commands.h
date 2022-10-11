#ifndef COMMANDS_H
#define COMMANDS_H

// ——————————————————————————————————————————————————
// BEGIN R1
// ——————————————————————————————————————————————————
void welcome(void);
void version(void);
void help(void);
void get_date(void);
void set_date(int mm, int dd, int yyyy);
void get_time(void);
void set_time(int hh, int mm, int ss);
// ——————————————————————————————————————————————————
// END R1
// BEGIN R2
// ——————————————————————————————————————————————————
// ——————————————————————————————————————————————————
// BEGIN R2 PERMANENT COMMANDS
// ——————————————————————————————————————————————————
void set_priority(char* name, int priority);
void suspend(char* name);
void resume(char* name);
void show(char* name);
void show_ready();
void show_blocked();
void show_all();
// ——————————————————————————————————————————————————
// END R2 PERMANENT COMMANDS
// BEGIN R2 TEMPORARY COMMANDS
// ——————————————————————————————————————————————————
void create(char* name, int class, int priority);
void delete(char* name);
void block(char* name);
void unblock(char* name);
// ——————————————————————————————————————————————————
// END R2 TEMPORARY COMMANDS
// ——————————————————————————————————————————————————
// ——————————————————————————————————————————————————
// END R2
// ——————————————————————————————————————————————————

// ——————————————————————————————————————————————————
// BEGIN R3 TEMPORARY COMMANDS
// ——————————————————————————————————————————————————
void yield();

#endif
