/*
 * ds18b20.h
 *
 * Created: 18.02.2016 21:36:23
 *  Author: Zbyszek
 */ 

#ifndef DS18B20_H_
#define DS18B20_H_

//commands definitions
#define DS18B20_CMD_CONVERTTEMP		0x44
#define DS18B20_CMD_RSCRATCHPAD		0xbe
#define DS18B20_CMD_WSCRATCHPAD		0x4e
#define DS18B20_CMD_CPYSCRATCHPAD	0x48
#define DS18B20_CMD_RECEEPROM		0xb8
#define DS18B20_CMD_RPWRSUPPLY		0xb4
#define DS18B20_CMD_SEARCHROM		0xf0
#define DS18B20_CMD_READROM			0x33
#define DS18B20_CMD_MATCHROM		0x55
#define DS18B20_CMD_SKIPROM			0xcc
#define DS18B20_CMD_ALARMSEARCH		0xec

//Reading and writing data from ds chip is time-based, so we may want disable interrupts while doing so.
#define DS18B20_INT_DISABLE 1

//functions
// ds_convertAndRead(), sentd CONVERTTEMP command and WAITS for it to complete up to 750ms. Very imperfect solution.
int16_t ds_convertAndRead(uint64_t ROM);
// ds_convert(), only sends CONVERTTEMP command. Doesn't wait for completion. Non-blocking
uint8_t ds_convert(uint64_t ROM);
//ds_read(), reads previously converted data. Preferably to be used in some interrupt routine.
//Unaware of conversion status - make sure to wait sufficient amount of time after invoking ds_conver()
int16_t ds_read(uint64_t ROM);

#endif
