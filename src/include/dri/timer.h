#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "utils.h"

#define PIT_FREQ 1193182
#define TIMER_HZ 1000  // 1ms

extern uint64_t timer; // ms since first irq init

void timer_init();
void timer_irq();

void sleep(uint64_t ms);

#endif
