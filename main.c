/*
 * termometr
 * main.c
 *
 *  Created on: 29 paź 2017
 *      Author: zbyszek
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include <string.h>
#include <stdio.h>
#include "ds18b20.h"
#include "onewire.h"

#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_PIN PB5
#define LED_TOGGLE() (LED_PORT ^= 1<<LED_PIN)

#define MAX_DEVICES 5//maximum number of devices on 1 wire line

int main()
{
	initUART();
	RX_INTEN();//enable RX interrupt
	sei();
	//
	uint64_t ROM, deviceAddress;
	uint8_t familyCode;
	uint8_t LD = 0 ;			//lastDeviation,
	uint64_t deviceList[MAX_DEVICES];
	uint8_t count = 0;
	char buff[40];  //just a message buffer
	do {
		ROM = ow_readNextROM(&LD);
		if (ROM && count<MAX_DEVICES)
		{
			deviceList[count++] = ROM;
		}
		else break;
	}
	while(LD);
	writeString("\e[1;1H\e[2J");	// clear terminal window
	sprintf(buff, "Found %d devices.\n\r", count);
	writeString(buff);

	for(uint8_t i=0; i<count; i++)
	{
		familyCode = (uint8_t)deviceList[i] & 0xff;
		deviceAddress = (deviceList[i] >> 8) & 0x00ffffffffffff;
		/*uint64_t copy = deviceList[i];
		for(uint8_t n=0; n<64; n++)
		{
			(copy&1) ?writeString("1"):writeString("0");
			copy >>=1;
		}
		writeString("\n\r");*/
		sprintf(buff, "#%d family code: 0x%x\n\r", i+1, familyCode);
		writeString(buff);
		sprintf(buff, "#%d address: 0x%012lx\n\n\r", i+1, deviceAddress);
		writeString(buff);
	}
	//writeString("\e[1;1H\e[2J");

	while(1)
	{
		for(uint8_t i=0; i<count; i++)
		{
			uint16_t temp_raw = ds_convertAndRead(deviceList[i]);
			sprintf(buff, "#%d temperature is: %.1f celsius.\n\r", i+1, (float)temp_raw/16);
			writeString(buff);
		}
		sprintf(buff, "\033[%d;0H", 3*count+2);//move cursor
		writeString(buff);
	}
	return 0;
}
ISR(USART_RX_vect)
{
	LED_TOGGLE();
	char c = getByte();
	switch(c)
	{
	case 13://CR
		writeString("\r\n");
		break;
	default:
		putByte(c);
		break;
	}
}
