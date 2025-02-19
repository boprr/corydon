#include <stdint.h>

#include "dri/time.h"
#include "utils.h"

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

#define CURRENT_YEAR 2025  // fallback (no ACPI)

int day_in_months[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};  // leap year

int century_reg = 0x00;  // TODO: ACPI

rtc_state rtc;

uint8_t cmos_get_register(int reg) {
    outb(CMOS_ADDR, reg);
    return inb(CMOS_DATA);
}

int cmos_get_uip_flag() {
    outb(CMOS_ADDR, 0x0A);
    return (inb(CMOS_DATA) & 0x80);
}

void rtc_read_from_cmos() {
    uint8_t century;
    uint8_t register_b;

    while (cmos_get_uip_flag());
    rtc.second = cmos_get_register(0x00);
    rtc.minute = cmos_get_register(0x02);
    rtc.hour = cmos_get_register(0x04);
    rtc.day = cmos_get_register(0x07);
    rtc.month = cmos_get_register(0x08);
    rtc.year = cmos_get_register(0x09);

    if (century_reg != 0) {
        century = cmos_get_register(century_reg);
    }

    register_b = cmos_get_register(0x0B);

    if (!(register_b & 0x04)) {
        rtc.second = (rtc.second & 0x0F) + ((rtc.second / 16) * 10);
        rtc.minute = (rtc.minute & 0x0F) + ((rtc.minute / 16) * 10);
        rtc.hour = ((rtc.hour & 0x0F) + (((rtc.hour & 0x70) / 16) * 10)) |
                   (rtc.hour & 0x80);
        rtc.day = (rtc.day & 0x0F) + ((rtc.day / 16) * 10);
        rtc.month = (rtc.month & 0x0F) + ((rtc.month / 16) * 10);
        rtc.year = (rtc.year & 0x0F) + ((rtc.year / 16) * 10);
        if (century_reg != 0) {
            century = (century & 0x0F) + ((century / 16) * 10);
        }
    }

    // Convert 12 hour clock to 24 hour clock if necessary
    if (!(register_b & 0x02) && (rtc.hour & 0x80)) {
        rtc.hour = ((rtc.hour & 0x7F) + 12) % 24;
    }

    // Calculate the full (4-digit) year
    if (century_reg != 0) {
        rtc.year += century * 100;
    } else {
        rtc.year += (CURRENT_YEAR / 100) * 100;
        if (rtc.year < CURRENT_YEAR) rtc.year += 100;
    }
}

int is_leap_year(uint64_t year) {
    if (year % 400 == 0) {
        return 1;
    } else if (year % 100 == 0) {
        return 0;
    } else if (year % 4 == 0) {
        return 1;
    } else {
        return 0;
    }
}

uint64_t rtc_to_unix() {
    uint64_t unix_time = 0;

    int leap_year;
    for (uint64_t i = 1970; i < rtc.year; i++) {
        leap_year = is_leap_year(i);
        unix_time += (365 + leap_year) * 86400;  // 24*60*60
    }

    leap_year = is_leap_year(rtc.year);
    for (int i = 0; i < (rtc.month - 1); i++) {
        unix_time += day_in_months[leap_year][i] * 86400;
    }

    unix_time += (rtc.day - 1) * 86400;
    unix_time += rtc.hour * 3600;  // 60*60
    unix_time += rtc.minute * 60;
    unix_time += rtc.second;

    return unix_time;
}
