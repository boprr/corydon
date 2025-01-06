#include "cpu/timer.h"

#include <stdint.h>

uint64_t timer = 0;

void timer_irq() { timer++; }  // 54.9254 ms