#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "dri/pic.h"
#include "dri/serial.h"
#include "print/printf.h"

void kmain(uint64_t multiboot_magic, void *multiboot_data) {
    clear_screen();
    serial_init();
    pic_remap();
    gdt_init();
    idt_init();

    printf("+-----------------------------------+ \n");
    printf("|  Corydon Operating System Kernel  | \n");
    printf("|                                   | \n");
    printf("|  arch: x86_64          ver: 0.01  | \n");
    printf("+-----------------------------------+ \n");
}