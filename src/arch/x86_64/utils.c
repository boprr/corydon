#include "utils.h"

#include <stdint.h>

#include "cpu/idt.h"
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

void *get_stack_pointer() {
    void *sp;

    // Inline assembly to read the stack pointer (x86-64)
    __asm__ volatile("mov %%rsp, %0" : "=r"(sp));  // For x86-64
    return sp;
}

void panic(char *message) {
    void *stack_pointer = get_stack_pointer();
    cpu_status *status = (cpu_status *)stack_pointer;

    debugf("--- KERNEL PANIC ---\n");
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
    debugf("--- END OF KERNEL PANIC --- %s", message);

    printf("--- KERNEL PANIC ---\n");
    printf("ds: %p      r15: %p\n", status->ds, status->r15);
    printf("r14: %p     r13: %p\n", status->r14, status->r13);
    printf("r12: %p     r11: %p\n", status->r12, status->r11);
    printf("r10: %p     r9: %p\n", status->r10, status->r9);
    printf("r8: %p      rbp: %p\n", status->r8, status->rbp);
    printf("rdi: %p     rsi: %p\n", status->rdi, status->rsi);
    printf("rdx: %p     rcx: %p\n", status->rdx, status->rcx);
    printf("rbx: %p     rax: %p\n", status->rbx, status->rax);
    printf("rip: %p     cs: %p\n", status->rip, status->cs);
    printf("flags: %p   rsp %p\n", status->flags, status->rsp);
    printf("ss: %p\n", status->ss);
    unsigned long cr2_value;
    __asm__ __volatile__(
        "mov %%cr2, %0"
        : "=r"(cr2_value)  // Move the value of CR2 into cr2_value
    );
    unsigned long cr3_value;
    __asm__ __volatile__(
        "mov %%cr3, %0"
        : "=r"(cr3_value)  // Move the value of CR3 into cr3_value
    );
    printf("cr2: %p\n", cr2_value);
    printf("cr3: %p\n", cr3_value);
    printf("--- END OF KERNEL PANIC --- %s", message);
    while (1) {
        asm volatile("cli; hlt");
    }
}
