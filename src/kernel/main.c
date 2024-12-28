#include "dri/serial.h"
#include "idt.h"
#include "print/debugf.h"
#include "print/printf.h"
#include "utils.h"

void kmain() {
    serial_init();
    idt_init();

    printf("Welcome to Corydon 0.01 test \n");
}
