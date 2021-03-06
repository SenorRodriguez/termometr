/*
 * onewire.h
 *
 * Created: 19.02.2016 17:03:26
 *  Author: Zbyszek
 */ 


#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include <avr/io.h>

//define connection
#ifndef OW_PORT
#define OW_PORT PORTC
#endif
#ifndef OW_DDR
#define OW_DDR	DDRB
#endif
#ifndef OW_PIN
#define OW_PIN	PINB
#endif
#ifndef OW_DQ
#define OW_DQ	PB3		//OneWire data line
#endif
//delay definitions (by Atmel's application note)

#define DELAY_A 6
#define DELAY_B 64
#define DELAY_C 60
#define DELAY_D 10
#define DELAY_E 9
#define DELAY_F 55
#define DELAY_G 0
#define DELAY_H 480
#define DELAY_I 70
#define DELAY_J 410

#define OW_CMD_SEARCHROM		0xf0

#define ERR_ROM_SEARCH_FAILED	0xff

uint8_t ow_reset();
void ow_writebit(uint8_t bit);
uint8_t ow_readbit(void);
void ow_writebyte(uint8_t data);
void ow_writeROM(uint64_t ROM);
uint8_t ow_readbyte(void);
uint8_t ow_enumerate(volatile uint64_t* bitPattern, uint8_t lastDeviation);
uint64_t ow_readNextROM(volatile uint8_t* lastDeviation);


#endif /* ONEWIRE_H_ */
