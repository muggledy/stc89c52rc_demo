#ifndef __AT24C02_H__
#define __AT24C02_H__

#include "global.h"
#include "I2C.h"

#define AT24C02_W_ADDRESS 0xA0
#define AT24C02_R_ADDRESS 0xA1

/*
 * AT24C02 capacity is 256 bytes, valid word_address is 0~255.
 * Each time data is written into AT24C02, you must wait 3~5ms before performing other operations
 * Demo:
 * AT24C02_WriteByte(1, 66);
 * delay_1ms(5); //waiting AT24C02 write byte into hardware, otherwise, failed if reading without delay
 * LCD_ShowNum(2, 1, AT24C02_ReadByte(1), 3);
 */
extern void AT24C02_WriteByte(uint8_t word_address, uint8_t byte);
extern uint8_t AT24C02_ReadByte(uint8_t word_address);

#endif