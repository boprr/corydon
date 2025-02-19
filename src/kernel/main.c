#include <stdint.h>

#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "dri/pic.h"
#include "dri/serial.h"
#include "dri/timer.h"
#include "print/printf.h"

void kmain(uint64_t in_multiboot_magic, void *in_multiboot_data) {
    gdt_init();
    pic_remap();
    idt_init();
    timer_init();

    clear_screen();
    serial_init();

    printf("+-----------------------------------+ \n");
    printf("|  Corydon Operating System Kernel  | \n");
    printf("|                                   | \n");
    printf("|  arch: x86_64          ver: 0.01  | \n");
    printf("+-----------------------------------+ \n");

    printf("%u", timer);
    // printf("%i", timer);
}
