#include "utils.h"

#include <stdint.h>

#include "idt.h"
#include "print/debugf.h"
#include "print/printf.h"

void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

void io_wait(void) { outb(0x80, 0); }

void panic() {
    // uint64_t stack_pointer = push_registers();
    // cpu_status *status = (cpu_status *)stack_pointer;
    // TODO: make this work

    debugf("--- KERNEL PANIC ---\n");
    /*
    debugf("ds: %p\n", status->ds);
    debugf("r15: %p\n", status->r15);
    debugf("r14: %p\n", status->r14);
    debugf("r13: %p\n", status->r13);
    debugf("r12: %p\n", status->r12);
    debugf("r11: %p\n", status->r11);
    debugf("r10: %p\n", status->r10);
    debugf("r9: %p\n", status->r9);
    debugf("r8: %p\n", status->r8);
    debugf("rbp: %p\n", status->rbp);
    debugf("rdi: %p\n", status->rdi);
    debugf("rsi: %p\n", status->rsi);
    debugf("rdx: %p\n", status->rdx);
    debugf("rcx: %p\n", status->rcx);
    debugf("rbx: %p\n", status->rbx);
    debugf("rax: %p\n", status->rax);
    debugf("rip: %p\n", status->rip);
    debugf("cs: %p\n", status->cs);
    debugf("flags: %p\n", status->flags);
    debugf("rsp: %p\n", status->rsp);
    debugf("ss: %p\n", status->ss);
    */
    debugf("--- END OF KERNEL PANIC ---\n");
    while (1) {
        asm volatile("cli; hlt");
    }
}