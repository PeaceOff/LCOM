#ifndef _RTC_H
#define _RTC_H

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_REG_A 10
#define RTC_REG_B 11
#define RTC_REG_C 12
#define RTC_REG_D 13

#define RTC_UF 0x10
#define RTC_AF 0x20
#define RTC_PF 0x40


#define RTC_IRQ 8

int RTC_DisplayTime();

int RTC_getTime();

void RTC_secondsToString(int seconds, char* res);


#endif
