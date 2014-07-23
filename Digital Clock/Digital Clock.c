/*
 * Digital_Clock.c
 *
 * Created: 7/22/2014 5:57:16 PM
 *  Author: Armia Wagdy
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#define  F_CPU	1000000UL
#include <util/delay.h>
#include <stdlib.h>
#include "DS1307_RTC.h"
#include "external_interrupts.h"
#include "LCD 4-bit.h"

/*************************************Declaring global variables************************************/

uint8_t str1[100] = {0}; 
uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;
uint8_t day = 0;
uint8_t date = 0;
uint8_t month = 0;
uint8_t year = 0;

/******************************************Declaring functions***************************************/

void update_time (void);
uint8_t BCD_to_int (uint8_t data);

/***************************************************************************************************/

int main(void)
{	
	app_init();
	rtc_setTime(0x23,0x59,0x55);
	rtc_setDate(0x01,0x30,0x04,0x14);
	rtc_getTime(&seconds, &minutes, &hours);
	rtc_getDate(&day, &date, &month, &year);
	seconds = BCD_to_int(seconds);
	minutes = BCD_to_int(minutes);
	hours   = BCD_to_int(hours);
	day		= BCD_to_int(day);
	date    = BCD_to_int(date);
	month   = BCD_to_int(month);
	year    = BCD_to_int(year);
	sei();
	while(1);
	
}

/********************************************ISR for INT0******************************************/

ISR(INT0_vect)
{
	cli();

	if(++seconds == 60)
	{
		seconds = 0;
		minutes++;
	}
	
	if(minutes == 60)
	{
		minutes = 0;
		hours++;
	}
	
	if(hours == 24)
	{
		hours = 0;
		date++;
		day++;
	}
	if(day == 8)
	{
		day = 1;
	}
	if(date == 32)
	{
		month++;
		date = 1;
	}
	else if(date == 31)
	{
		switch(month)
		{
			case 4:
			case 6:
			case 9:
			case 11:
			month++;
			date = 1;
			break;
		}
	}
	
	if(month == 13)
	{
		year++;
		month = 1;
	}
	
	update_time();
	update_date();
	
	sei();
}

/*************************************************************************************************/

void update_time (void)
{
	lcd_gotoxy(1,1);
	if(hours < 10)		lcdData('0');
	lcd_print(itoa((int)hours,str1,10));
	lcd_print(":");
	
	if(minutes < 10)	lcdData('0');
	lcd_print(itoa((int)minutes,str1,10));
	lcd_print(":");
	
	if(seconds < 10)	lcdData('0');
	lcd_print(itoa((int)seconds,str1,10));
}

/*************************************************************************************************/

void update_date (void)
{
	lcd_gotoxy(10,1);
	switch(day)
	{
		case 1:
			lcd_print("SAT");
			break;
		case 2:
			lcd_print("SUN");
			break;
		case 3:
			lcd_print("MON");
			break;
		case 4:
			lcd_print("TUE");
			break;
		case 5:
			lcd_print("WED");
			break;
		case 6:
			lcd_print("THR");
			break;
		case 7:
			lcd_print("FRI");
			break;
		default:
			lcd_print("ERR");
	}
	lcd_gotoxy(1,2);
	if(date < 10)		lcdData('0');
	lcd_print(itoa((int)date,str1,10));
	lcd_print(":");
	
	if(month < 10)	lcdData('0');
	lcd_print(itoa((int)month,str1,10));
	lcd_print(":");
	
	lcdData('2');
	lcdData('0');
	lcd_print(itoa((int)year,str1,10));
}

/*************************************************************************************************/

uint8_t BCD_to_int(uint8_t data)
{
	uint8_t temp = 0;
	temp += (data >> 4) * 10;
	temp += (data & 0x0F);
	return temp;
}

/*************************************************************************************************/

void app_init (void)
{
	lcd_init();
	rtc_init();
	set_externalInterrupt(INT0,FALLING_EDGE);
}