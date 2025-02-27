#include <stdint.h>

#include "boot/bootloader.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "dri/pic.h"
#include "dri/serial.h"
#include "dri/time.h"
#include "mem/liballoc.h"
#include "mem/pmm.h"
#include "print/printf.h"

extern void sse_enable();

uint64_t multiboot_magic;
void *multiboot_data;

void kmain(uint64_t in_multiboot_magic, void *in_multiboot_data) {
    multiboot_data = in_multiboot_data;
    multiboot_magic = in_multiboot_magic;

    sse_enable();
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

    pmm_init();
}
