/*
 * Contains initialization processes.
 * Called after the bootloader.
 * Initial Kernel by: Forrest Desjardin (2013)
 * Modifications by: Andrew Duncan (2014), John Jacko (2017), Ben Smith (2018), and Alex Wilson (2019)
*/

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>
#include "../../modules/pcb.h"

//including comhand.h in here, ask about...
//the difference between "" and <>
#include "modules/mpx_supt.h"
#include "modules/comhand.h"
#include "../../modules/pcb.h"


void kmain(void) {

    // **************************************************

    // Uncomment if you want to access the multiboot header.
//    extern uint32_t magic;
//    extern void *mbd;
//    char *boot_loader_name = (char *) ((long *) mbd)[16];

    // **************************************************

    init_serial(COM1);
    set_serial_in(COM1);
    set_serial_out(COM1);

    klogv("Starting MPX boot sequence...");
    klogv("Initialized serial I/O on COM1 device.");

    mpx_init(MODULE_R2);

    // **************************************************

    // Check that the boot was successful when using grub.
    // Comment this out when booting the kernel directly using QEMU.
//    if (magic != 0x2BADB002) {
//        kpanic("Boot was not error free. Halting.");
//    }

    // **************************************************

    init_gdt();

    klogv("Initialized global descriptor table.");

    // **************************************************

    init_irq();
    init_pic();
    sti();
    init_idt();

    klogv("Initialized interrupt descriptor table.");

    // **************************************************

    init_paging();

    klogv("Initialized virtual memory.");

    // **************************************************
    //initialize the queues

    init_queues();

    klogv("Entering command handler...");

    comhand();

    klogv("Exited command handler.");

    // **************************************************

    klogv("Shutdown complete.");
    klogv("You may now turn off the machine. (QEMU: Ctrl+a x)");

    hlt();

    // **************************************************
}
