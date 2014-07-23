/*
 * DS1307_RTC.h
 *
 * Created: 7/22/2014 6:05:44 PM
 *  Author: A R M T
 */ 


#ifndef DS1307_RTC_H_
#define DS1307_RTC_H_

void rtc_init (void);
void rtc_clear_CH_bit (void);
void rtc_getTime(uint8_t *sec, uint8_t *min, uint8_t *hour);
void rtc_setTime(uint8_t sec, uint8_t min, uint8_t hour);
void rtc_getDate(uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year);
void rtc_setDate(uint8_t day, uint8_t date, uint8_t month, uint8_t year);
void rtc_setMode(uint8_t mode);

#endif /* DS1307_RTC_H_ */