#pragma once

typedef struct
{
    int sec;            // [0, 59]
    int min;            // [0, 59]
    int hour;           // [0, 23]
    int day;            // [1, 31]
    int month;          // [1, 12]
    int year;           // [1970, 2038]
    int weekDay;        // [0, 6] sunday = 0
    int yearDay;        // [0, 365]
    int tzOffset;       // offset in minutes
}
DateTime;

void RTC_GetTime(DateTime *dt);
void RTC_SetTime(const DateTime *dt);