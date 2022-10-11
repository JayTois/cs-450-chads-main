/*
  ----- serial.c -----

  Description..: Contains methods and variables used for
    serial input and output.
*/

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>
#include "modules/mpx_supt.h"

#define NO_ERROR 0

// Active devices used for serial I/O
int serial_port_out = 0;
int serial_port_in = 0;

/*
  Procedure..: init_serial
  Description..: Initializes devices for user interaction, logging, ...
*/
int init_serial(int device) {
    outb(device + 1, 0x00); //disable interrupts
    outb(device + 3, 0x80); //set line control register
    outb(device + 0, 115200 / 9600); //set bsd least sig bit
    outb(device + 1, 0x00); //brd most significant bit
    outb(device + 3, 0x03); //lock divisor; 8bits, no parity, one stop
    outb(device + 2, 0xC7); //enable fifo, clear, 14byte threshold
    outb(device + 4, 0x0B); //enable interrupts, rts/dsr set
    (void) inb(device);      //read bit to reset port
    return NO_ERROR;
}

/*
  Procedure..: serial_println
  Description..: Writes a message to the active serial output device.
    Appends a newline character.
*/
int serial_println(const char *msg) {
    int i;
    for (i = 0; *(i + msg) != '\0'; i++) {
        outb(serial_port_out, *(i + msg));
    }
    outb(serial_port_out, '\r');
    outb(serial_port_out, '\n');
    return NO_ERROR;
}

/*
  Procedure..: serial_print
  Description..: Writes a message to the active serial output device.
*/
int serial_print(const char *msg) {
    int i;
    for (i = 0; *(i + msg) != '\0'; i++) {
        outb(serial_port_out, *(i + msg));
    }
    if (*msg == '\r') outb(serial_port_out, '\n');
    return NO_ERROR;
}

/*int (*student_read)(char *buffer, int *count);
  Procedure..: set_serial_out
  Description..: Sets serial_port_out to the given device address.
    All serial output, such as that from serial_println, will be
    directed to this device.
*/
int set_serial_out(int device) {
    serial_port_out = device;
    return NO_ERROR;
}

/*
  Procedure..: set_serial_in
  Description..: Sets serial_port_in to the given device address.
    All serial input, such as console input via a virtual machine,
    QEMU/Bochs/etc, will be directed to this device.
*/
int set_serial_in(int device) {
    serial_port_in = device;
    return NO_ERROR;
}

int *polling(char *buf, int *buf_size) {
    const char backspace = 8;
    const char carriage_return = 13;
    const char special_keys = 27;
    const char delete_1 = 126;
    const char delete_2 = 127;

    int i = 0;

    while (1) {
        // Read the least significant bit (LSB) of the Line Status Register (LSR).
        // When the LSB of the LSR is set, a byte is ready to be read from the Communication Port 1 (COM1) Register.

        // This guard clause causes the program to loop until the LSB of the LSR is set.
        if (!(inb(COM1 + 5) & 1)) {
            continue;
        }

        char c = inb(COM1);

        if (c == carriage_return) {
            break;
        }

        // **************************************************

        // Handle backspace and delete
        if (c == backspace || c == delete_1 || c == delete_2) {
            if (i > 0) {
                // Backup the cursor
                // Print a space to cover the old character
                // Backup the cursor again
                serial_print("\b");
                serial_print(" ");
                serial_print("\b");
                buf[i - 1] = '\0';
                i--;
            }

            continue;
        }

        // Ignore special keys
        if (c == special_keys) {
            inb(COM1);
            inb(COM1);
            continue;
        }

        // Add the character, print it, and increment the index
        if (i + 1 < *buf_size) {
            buf[i] = c;
            serial_print(buf + i);
            i++;
        }
    }

    return buf_size;
}
