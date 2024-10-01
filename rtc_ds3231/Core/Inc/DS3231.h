/*
 * DS3231.h
 *
 *  Created on: Oct 1, 2024
 *      Author: Pheonix
 */

#ifndef INC_DS3231_H_
#define INC_DS3231_H_

#define set_seconds 0x0
#define set_minutes 0x1
#define set_hours 0x2
#define set_day 0x3
#define set_date 0x4
#define set_month 0x5
#define set_year 0x6
#define set_day_of_month 0x4
#define set_month 0x5
#define set_year 0x6



void rtc_init();

void rtc_set_time(int hour, int minute, int second);

void rtc_set_date(int hour, int minute, int second);

void rtc_get_time(int *hour, int *minute, int *second);

void rtc_get_date(int *hour, int *minute, int *second);

#endif /* INC_DS3231_H_ */
