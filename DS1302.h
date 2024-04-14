#ifndef __DS1302_H__
#define __DS1302_H__

#include <REGX52.H>
#include "global.h"

#define DS1302_SCLK P3_6
#define DS1302_IO   P3_4
#define DS1302_CE   P3_5

/*
 * Demo:
 * INIT_DS1302();
 * CLOSE_DS1302_WRITE_PROTECT();
 * write_byte_into_ds1302(DS1302_SECOND_W_CMD, dec2_to_bcd8421(12)); //write into DS1302's Seconds Register
 * byte = read_byte_from_ds1302(DS1302_SECOND_R_CMD); //read from DS1302's Seconds Register
 * LCD_Init();
 * LCD_ShowHexNum(1, 1, byte, 2); //LCD_ShowNum(1, 1, bcd8421_to_dec2(byte), 2);
*/

#define INIT_DS1302() {\
    DS1302_CE = 0;\
    DS1302_SCLK = 0;\
}

extern void write_byte_into_ds1302(uint8_t cmd, uint8_t byte);
extern uint8_t read_byte_from_ds1302(uint8_t cmd);

#define DS1302_SECOND_W_CMD 0x80
#define DS1302_MINUTE_W_CMD 0x82
#define DS1302_HOUR_W_CMD   0x84
#define DS1302_DAY_W_CMD    0x86
#define DS1302_MONTH_W_CMD  0x88
#define DS1302_WEEK_W_CMD   0x8A
#define DS1302_YEAR_W_CMD   0x8C
#define DS1302_WP_W_CMD     0x8E

#define DS1302_SECOND_R_CMD 0x81 //DS1302_WP_W_CMD | 0x01
#define DS1302_MINUTE_R_CMD 0x83
#define DS1302_HOUR_R_CMD   0x85
#define DS1302_DAY_R_CMD    0x87
#define DS1302_MONTH_R_CMD  0x89
#define DS1302_WEEK_R_CMD   0x8B
#define DS1302_YEAR_R_CMD   0x8D
#define DS1302_WP_R_CMD     0x8F

#define CLOSE_DS1302_WRITE_PROTECT() {\
    write_byte_into_ds1302(DS1302_WP_W_CMD, 0x00);\
}

#define OPEN_DS1302_WRITE_PROTECT() {\
    write_byte_into_ds1302(DS1302_WP_W_CMD, 0x80);\
}

typedef struct ds1302_datetime_ {
    uint8_t year; //00~99
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t week;
} ds1302_datetime_t;

extern ds1302_datetime_t ds1302_datetime;

#if 0
extern uint8_t dec2_to_bcd8421(uint8_t dec2);
extern uint8_t bcd8421_to_dec2(uint8_t bcd8421);
#else
#define dec2_to_bcd8421(dec2)    ((((dec2)/10)<<4) + (dec2)%10)
#define bcd8421_to_dec2(bcd8421) ((((bcd8421)>>4)*10) + ((bcd8421)&0x0F)) //((bcd8421)/16*10+(bcd8421)%16)
#endif

extern void ds1302_set_time();
extern void ds1302_read_time();

#endif