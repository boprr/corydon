#include "dri/pic.h"
#include "dri/serial.h"
#include "gdt.h"
#include "idt.h"
#include "print/printf.h"
#include "utils.h"

void kmain() {
    serial_init();
    pic_remap();
    gdt_init();
    idt_init();
}