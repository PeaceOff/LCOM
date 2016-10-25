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

/** @defgroup RTC RTC module
 * @{
 *
 * Module responsible for comunications with RTC
 *
 */

/**
 * @brief Function to Display Time in the Sceen
 *
 *
 * @return Retuns 1 if succeeded, 0 otherwise
 */
int RTC_DisplayTime();

/**
 * @brief Function to Get the current time
 *
 * @return Returns an integer with the time in seconds
 */
int RTC_getTime();

/**
 * @brief Function to Convert the secons in the format HH:MM:SS
 *
 * @param seconds the seconds to convert
 * @param res the char where the conversion is received
 *
 * @return void
 */

void RTC_secondsToString(int seconds, char* res);


#endif
