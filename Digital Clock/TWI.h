/*
 * TWI.h
 *
 * Created: 7/22/2014 12:32:15 PM
 *  Author: Armia Wagdy
 */ 


#ifndef TWI_H_
#define TWI_H_

#define  WRITE	0
#define  READ   1

void TWI_initMaster(uint8_t twsr, uint8_t twbr);
void TWI_start(void);
void TWI_sendAddress(uint8_t Address, uint8_t RW);
void TWI_sendData(uint8_t Data);
void TWI_stop(void);
uint8_t TWI_receiveData(uint8_t is_last_byte);
void TWI_initSlave (uint8_t slaveAddress);
void TWI_listen(void);
uint8_t TWI_readStatus(void);

#endif /* TWI_H_ */