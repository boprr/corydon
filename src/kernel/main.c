#include <stdbool.h>

#include "idt.h"
#include "inline.h"
#include "panic.h"
#include "printf.h"

void kmain() {
    printf("Booting Kernel... ");
    idt_init();
}