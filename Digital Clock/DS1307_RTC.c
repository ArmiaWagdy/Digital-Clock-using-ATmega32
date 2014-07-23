/*
 * DS1307_RTC.c
 *
 * Created: 7/22/2014 6:05:08 PM
 *  Author: A R M T
 */ 

#include <avr/io.h>
#include "TWI.h"
#include "DS1307_RTC.h"
#define  F_CPU  1000000UL
#include <util/delay.h>

uint8_t str[100] = {0};

/**********************************************************************************/
//This function initiates the DS1307 Module and set the control register to 0x10
//which means that the square wave enable pin is set to 1. and clear bits RS0 and RS1
//so that a 1KHz square wave will be available on SQW/OUT pin. 

void rtc_init(void)
{	
	TWI_start();						//Start condition
	TWI_sendAddress(0XD0,0);			//SLA + W
	TWI_sendData(0x07);					//Register pointer = 0x07 (Control Register)
	TWI_sendData(0x10);					//Set SQWE bit to '1'
	TWI_stop();							//Stop condition
	rtc_clear_CH_bit();					//Clear CH bit to enable the oscillator
}

/**********************************************************************************/
//This function clears the CH bit in the first place in DS1307 RAM which is 0x00
//as the CH bit is undefined upon reset and it must equal to zero to enable the 
//oscillator. 

void rtc_clear_CH_bit(void)
{
	uint8_t seconds = 0;
	
	TWI_start();						//Start condition	
	TWI_sendAddress(0xD0,0);			//SLA + W
	TWI_sendData(0x00);					//Register pointer = 0x00
	TWI_start();						//Repeated start condition
	TWI_sendAddress(0xD0,1);			//SLA + R
	seconds = TWI_receiveData(1);		//Read one byte and return NACK
	seconds &= ~(1 << 7);				//Clear CH bit (bit number 7)
	TWI_stop();							//Stop condition
	
	TWI_start();						//Start condition
	TWI_sendAddress(0xD0,0);			//SLA + W
	TWI_sendData(0x00);					//Register pointer = 0x00
	TWI_sendData(seconds);				//Write seconds again (after clearing bit 7)
	TWI_stop();							//Stop condition
}

/**********************************************************************************/

void rtc_getTime(uint8_t* sec, uint8_t* min, uint8_t* hour)
{
	TWI_start();
	TWI_sendAddress(0xD0,WRITE);
	TWI_sendData(0x00);
	TWI_stop();
	
	TWI_start();
	TWI_sendAddress(0xD0,READ);
	*sec  = TWI_receiveData(0);
	*min  = TWI_receiveData(0);
	*hour = TWI_receiveData(1);
	TWI_stop();	
}

/**********************************************************************************/
void rtc_setTime(uint8_t sec, uint8_t min, uint8_t hour)
{
	TWI_start();
	TWI_sendAddress(0xD0,WRITE);
	TWI_sendData(0x00);
	TWI_sendData(hour);
	TWI_sendData(min);
	TWI_sendData(sec);
	TWI_stop();
}

/**********************************************************************************/

void rtc_getDate(uint8_t* day, uint8_t* date, uint8_t* month, uint8_t* year)
{
	TWI_start();
	TWI_sendAddress(0xD0,WRITE);
	TWI_sendData(0x03);
	TWI_stop();
	
	TWI_start();
	TWI_sendAddress(0xD0,READ);
	*day   = TWI_receiveData(0);
	*date  = TWI_receiveData(0);
	*month = TWI_receiveData(0);
	*year  = TWI_receiveData(1);
	TWI_stop();
}

/**********************************************************************************/
void rtc_setDate(uint8_t day, uint8_t date, uint8_t month, uint8_t year)
{
	TWI_start();
	TWI_sendAddress(0xD0,WRITE);
	TWI_sendData(0x03);
	TWI_sendData(day);
	TWI_sendData(date);
	TWI_sendData(month);
	TWI_sendData(year);
	TWI_stop();
}

/**********************************************************************************/

void rtc_setMode (uint8_t mode)
{
	uint8_t hours = 0;
	if(mode == 12)
	{
		TWI_start();
		TWI_sendAddress(0xD0,WRITE);
		TWI_sendData(0x02);
		TWI_start();
		TWI_sendAddress(0xD0,READ);
		hours = TWI_receiveData(1);
			lcd_print(itoa((int)hours,str,10));
			_delay_ms(2000);
		hours |= (1 << 6);
			lcd_print(itoa((int)hours,str,10));
			_delay_ms(2000);
		TWI_stop();
		
		TWI_start();
		TWI_sendAddress(0xD0,WRITE);
		TWI_sendData(0x02);
		TWI_sendData(hours);
		TWI_stop();
	}
	
	else
	{
		TWI_start();
		TWI_sendAddress(0xD0,WRITE);
		TWI_sendData(0x02);
		TWI_start();
		TWI_sendAddress(0xD0,READ);
		hours = TWI_receiveData(1);
		hours &= ~(1 << 6);
		TWI_stop();
		
		TWI_start();
		TWI_sendAddress(0xD0,WRITE);
		TWI_sendData(0x02);
		TWI_sendData(hours);
		TWI_stop();
	}
}

/**********************************************************************************/


//void display(uint8_t status)
//{
	//lcd_clear();
	//itoa((int)status,str,16);
	//lcd_gotoxy(1,1);
	//lcd_print("Status = 0x");
	//lcd_print(str);
	//_delay_ms(1000);
//}
//
/**********************************************************************************/