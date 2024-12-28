#include "dri/serial.h"

#include "print/printf.h"
#include "utils.h"

void enable_serial(int device) {
    outb(device + 1, 0x00);  // Disable all interrupts
    outb(device + 3, 0x80);  // Enable DLAB (set baud rate divisor)
    outb(device + 0, 0x03);  // Set divisor to 3 (lo byte) 38400 baud
    outb(device + 1, 0x00);  //                  (hi byte)
    outb(device + 3, 0x03);  // 8 bits, no parity, one stop bit
    outb(device + 2, 0xC7);  // Enable FIFO, clear them, with 14-byte threshold
    outb(device + 4, 0x0B);  // IRQs enabled, RTS/DSR set
}

void serial_init() { enable_serial(COM1); }

void serial_write(int device, char chr) {
    while ((inb(device + 5) & 0x20) == 0);
    outb(device, chr);
}
