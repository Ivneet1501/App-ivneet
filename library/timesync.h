#ifndef TIMESYNC_H
#define TIMESYNC_H

#include <stdint.h>

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} DateTime;

int isLeapYear(uint16_t year);
int isValidDateTime(DateTime dt);
uint32_t Apptime_gettimestamp(DateTime dt);
void Apptimer_performtimesync(uint32_t rtc, DateTime *dt);
void run_timesync_demo(void);  

#endif
