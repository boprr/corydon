#ifndef UTILS_H
#define UTILS_H
#include "stdint.h"

static inline void outb(uint16_t port, uint8_t val);
static inline uint8_t inb(uint16_t port);
static inline void wait(void);

#endif 