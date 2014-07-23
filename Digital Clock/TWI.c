/*
 * TWI.c
 *
 * Created: 7/22/2014 12:31:38 PM
 *  Author: Armia Wagdy
 */ 

#include <avr/io.h>
#include "TWI.h"

/************************************************************************************/

void TWI_initMaster(uint8_t twsr, uint8_t twbr)
{
    TWSR = twsr;					//Set prescaler bits
	TWBR = twbr;					//Set prescaler bits
	TWCR |= (1 << TWEN);			//Enable the TWI Module
}

/************************************************************************************/

void TWI_start(void)
{
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
	while ((TWCR & (1 << TWINT)) == 0);
}

/************************************************************************************/

void TWI_sendAddress(uint8_t Address, uint8_t RW)
{
	Address |= RW;
	TWDR = Address;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while ((TWCR & (1 << TWINT)) == 0);
}

/************************************************************************************/

void TWI_sendData(uint8_t Data)
{
	TWDR = Data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while ((TWCR & (1 << TWINT)) == 0);
}

/************************************************************************************/

void TWI_stop(void)
{
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
}

/************************************************************************************/

uint8_t TWI_receiveData(uint8_t is_last_byte)
{
	if(is_last_byte)
	{
		//Receive data and return NACK
		TWCR = (1 << TWEN) | (1 << TWINT);
	}
	else
	{
		//Receive data and return ACK
		TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	}
	while((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}

/************************************************************************************/

void TWI_initSlave (uint8_t slaveAddress)
{
	TWCR = (1 << TWEN);
	TWAR = slaveAddress;
	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
}

/************************************************************************************/

void TWI_listen(void)
{
	while ((TWCR & (1 << TWINT)) == 0);
}

/************************************************************************************/

uint8_t TWI_readStatus(void)
{
	uint8_t status = 0;
	status = TWSR & 0xF8;
	return status;
}

/************************************************************************************/