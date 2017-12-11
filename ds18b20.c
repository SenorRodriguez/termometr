/*
 * ds18b20.c
 *
 * Created: 18.02.2016 21:36:23
 *  Author: Zbyszek
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ds18b20.h"
#include "onewire.h"


//read temperature
int16_t ds_convertAndRead(uint64_t ROM) {
	ds_convert(ROM);
	while(!ow_readbit()); //Wait for conversion to complete. Until then DQ is being kept LOW by the slave
	return ds_read(ROM);
}

uint8_t ds_convert(uint64_t ROM)
{
#if DS18B20_INT_DISABLE == 1
	cli();
#endif

	ow_reset();							//reset line
	if (ROM)
	{
		//select device
		//ow_writebyte(DS18B20_CMD_MATCHROM);
		//send ROM data to select
		//ow_writeROM(ROM);
	}
	ow_writebyte(DS18B20_CMD_SKIPROM);		//We don't need ROM info, so lets skip it
	ow_writebyte(DS18B20_CMD_CONVERTTEMP);	//Start conversion. Data will be available in scratchpad in next read cycle.
#if DS18B20_INT_DISABLE == 1
	sei();
#endif
	return 0;
}

int16_t ds_read(uint64_t ROM)
{
	uint8_t temp_lo, temp_hi;
	int16_t temp;

	#if DS18B20_INT_DISABLE == 1
	cli();
	#endif

	ow_reset();								//reset line
	if (ROM)
	{
		//select device
		ow_writebyte(DS18B20_CMD_MATCHROM);
		//send ROM data to select
		ow_writeROM(ROM);
	}
	else ow_writebyte(DS18B20_CMD_SKIPROM);	//skip ROM

	ow_writebyte(DS18B20_CMD_RSCRATCHPAD);	//read temperature from scratchpad

	//temperature is kept in two subsequent bytes.
	temp_lo = ow_readbyte();				//read lower half
	temp_hi = ow_readbyte();				//read higher half

	#if DS18B20_INT_DISABLE == 1
	sei();
	#endif
	//combine two bytes tgether
	temp = ( temp_hi << 8 ) + temp_lo;
	return temp;
}
