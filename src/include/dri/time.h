#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "utils.h"

#define PIT_FREQ 1193182
#define TIMER_HZ 1000  // 1ms

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint64_t  year;
} rtc_state;

extern uint64_t timer; // ms since first irq init
extern rtc_state rtc;

void timer_init();
void timer_irq();
void sleep(uint64_t ms);
void rtc_read_from_cmos();
uint64_t rtc_to_unix();

#endif
