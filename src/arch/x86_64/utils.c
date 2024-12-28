#include "utils.h"

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

void panic(cpu_status* status) {
    debugf("ds: %i\n", status->ds);
    debugf("r15: %i\n", status->r15);
    debugf("r14: %i\n", status->r14);
    debugf("r13: %i\n", status->r13);
    debugf("r12: %i\n", status->r12);
    debugf("r11: %i\n", status->r11);
    debugf("r10: %i\n", status->r10);
    debugf("r9: %i\n", status->r9);
    debugf("r8: %i\n", status->r8);
    debugf("rbp: %i\n", status->rbp);
    debugf("rdi: %i\n", status->rdi);
    debugf("rsi: %i\n", status->rsi);
    debugf("rdx: %i\n", status->rdx);
    debugf("rcx: %i\n", status->rcx);
    debugf("rbx: %i\n", status->rbx);
    debugf("rax: %i\n", status->rax);
    debugf("vector: %i\n", status->vector);
    debugf("error: %i\n", status->error);
    debugf("rip: %i\n", status->rip);
    debugf("cs: %i\n", status->cs);
    debugf("flags: %i\n", status->flags);
    debugf("rsp: %i\n", status->rsp);
    debugf("ss: %i\n", status->ss);
    asm volatile("cli; hlt");
}