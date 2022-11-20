#include <drivers/rtc.h>

#include <io_ports.h>

#define IO_RTC_INDEX                    0x70
#define IO_RTC_TARGET                   0x71

#define REG_SEC                         0x00
#define REG_SEC_ALARM                   0x01
#define REG_MIN                         0x02
#define REG_MIN_ALARM                   0x03
#define REG_HOUR                        0x04
#define REG_HOUR_ALARM                  0x05
#define REG_WEEK_DAY                    0x06
#define REG_DAY                         0x07
#define REG_MONTH                       0x08
#define REG_YEAR                        0x09
#define REG_A                           0x0a
#define REG_B                           0x0b
#define REG_C                           0x0c
#define REG_D                           0x0d

#define REGA_UIP                        (1 << 7)    // Update In Progress

#define REGB_HOURFORM                   (1 << 1)    // Hour Format (0 = 12hr, 1 = 24hr)
#define REGB_DM                         (1 << 2)    // Data Mode (0 = BCD, 1 = Binary)

static uint8_t RTC_Read(uint8_t addr)
{
    outportb(IO_RTC_INDEX, addr);
    return inportb(IO_RTC_TARGET);
}

static void RTC_Write(uint8_t addr, uint8_t val)
{
    outportb(IO_RTC_INDEX, addr);
    outportb(IO_RTC_TARGET, val);
}

static uint8_t BCD_ToBin(uint8_t val)
{
    return (val & 0xf) + (val >> 4) * 10;
}

static uint8_t Bin_ToBcd(uint8_t val)
{
    return ((val / 10) << 4) + (val % 10);
}

void RTC_GetTime(DateTime *dt)
{
    /*if (RTC_Read(REG_A) & REGA_UIP)
    {
        PitWait(3);
    }*/

    uint8_t sec = RTC_Read(REG_SEC);
    uint8_t min = RTC_Read(REG_MIN);
    uint8_t hour = RTC_Read(REG_HOUR);
    uint8_t weekDay = RTC_Read(REG_WEEK_DAY);
    uint8_t day = RTC_Read(REG_DAY);
    uint8_t month = RTC_Read(REG_MONTH);
    uint16_t year = RTC_Read(REG_YEAR);

    uint8_t regb = RTC_Read(REG_B);

    if (~regb & REGB_DM)
    {
        sec = BCD_ToBin(sec);
        min = BCD_ToBin(min);
        hour = BCD_ToBin(hour);
        day = BCD_ToBin(day);
        month = BCD_ToBin(month);
        year = BCD_ToBin(year);
    }

    year += 2000;

    weekDay--;

    dt->sec = sec;
    dt->min = min;
    dt->hour = hour;
    dt->day = day;
    dt->month = month;
    dt->year = year;
    dt->weekDay = weekDay;
    dt->tzOffset = 0;
}

void RTC_SetTime(const DateTime *dt)
{
    uint8_t sec = dt->sec;
    uint8_t min = dt->min;
    uint8_t hour = dt->hour;
    uint8_t day = dt->day;
    uint8_t month = dt->month;
    uint8_t year = dt->year - 2000;
    uint8_t weekDay = dt->weekDay + 1;

    if (sec >= 60 || min >= 60 || hour >= 24 || day > 31 || month > 12 || year >= 100 || weekDay > 7)
        return;

    uint8_t regb = RTC_Read(REG_B);

    if (~regb & REGB_DM)
    {
        sec = BCD_ToBin(sec);
        min = BCD_ToBin(min);
        hour = BCD_ToBin(hour);
        day = BCD_ToBin(day);
        month = BCD_ToBin(month);
        year = BCD_ToBin(year);
    }

    /*if (RTC_Read(REG_A) & REGA_UIP)
    {
        PitWait(3);
    }*/

    RTC_Write(REG_SEC, sec);
    RTC_Write(REG_MIN, min);
    RTC_Write(REG_HOUR, hour);
    RTC_Write(REG_WEEK_DAY, weekDay);
    RTC_Write(REG_DAY, day);
    RTC_Write(REG_MONTH, month);
    RTC_Write(REG_YEAR, year);
}