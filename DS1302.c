#include "DS1302.h"

ds1302_datetime_t ds1302_datetime = {24, 5, 2, 10, 46, 30, 4};

void write_byte_into_ds1302(uint8_t cmd, uint8_t byte)
{
    uint8_t i = 0;

    DS1302_CE = 1;
    for (i=0; i<8; i++) {
        DS1302_IO = cmd & (0x01 << i);
        DS1302_SCLK = 1;
        //delay_10us(1); //no need for STC89C52RC
        DS1302_SCLK = 0;
    }
    //delay_10us(1); //no need for STC89C52RC
    for (i=0; i<8; i++) {
        DS1302_IO = byte & (0x01 << i);
        DS1302_SCLK = 1;
        //delay_10us(1); //no need for STC89C52RC
        DS1302_SCLK = 0;
    }
    DS1302_CE = 0;
}

uint8_t read_byte_from_ds1302(uint8_t cmd)
{
    uint8_t i = 0;
    uint8_t byte = 0;

    DS1302_CE = 1;
    for (i=0; i<8; i++) {
        DS1302_IO = cmd & (0x01 << i);
        DS1302_SCLK = 0;
        //delay_10us(1); //no need for STC89C52RC
        DS1302_SCLK = 1;
    }
    //delay_10us(1); //no need for STC89C52RC
    for (i=0; i<8; i++) {
        DS1302_SCLK = 1;
        //delay_10us(1); //no need for STC89C52RC
        DS1302_SCLK = 0;
        if (DS1302_IO) {
            byte |= (0x01 << i);
        }
    }
    DS1302_CE = 0;
    DS1302_IO = 0;
    return byte;
}

#if 0
uint8_t dec2_to_bcd8421(uint8_t dec2) //dec2:two decimal digits
{
	return ((dec2/10)<<4) + dec2%10;
}

uint8_t bcd8421_to_dec2(uint8_t bcd8421)
{
	return (((bcd8421)>>4)*10) + ((bcd8421)&0x0F);
}
#endif

void ds1302_set_time()
{
    CLOSE_DS1302_WRITE_PROTECT();
    write_byte_into_ds1302(DS1302_YEAR_W_CMD, dec2_to_bcd8421(ds1302_datetime.year));
    write_byte_into_ds1302(DS1302_MONTH_W_CMD, dec2_to_bcd8421(ds1302_datetime.month));
    write_byte_into_ds1302(DS1302_DAY_W_CMD, dec2_to_bcd8421(ds1302_datetime.day));
    write_byte_into_ds1302(DS1302_HOUR_W_CMD, dec2_to_bcd8421(ds1302_datetime.hour));
    write_byte_into_ds1302(DS1302_MINUTE_W_CMD, dec2_to_bcd8421(ds1302_datetime.minute));
    write_byte_into_ds1302(DS1302_SECOND_W_CMD, dec2_to_bcd8421(ds1302_datetime.second));
    write_byte_into_ds1302(DS1302_WEEK_W_CMD, dec2_to_bcd8421(ds1302_datetime.week));
    OPEN_DS1302_WRITE_PROTECT();
}

void ds1302_read_time()
{
    ds1302_datetime.year = bcd8421_to_dec2(read_byte_from_ds1302(DS1302_YEAR_R_CMD));
    ds1302_datetime.month = bcd8421_to_dec2(read_byte_from_ds1302(DS1302_MONTH_R_CMD));
    ds1302_datetime.day = bcd8421_to_dec2(read_byte_from_ds1302(DS1302_DAY_R_CMD));
    ds1302_datetime.hour = bcd8421_to_dec2(read_byte_from_ds1302(DS1302_HOUR_R_CMD));
    ds1302_datetime.minute = bcd8421_to_dec2(read_byte_from_ds1302(DS1302_MINUTE_R_CMD));
    ds1302_datetime.second = bcd8421_to_dec2(read_byte_from_ds1302(DS1302_SECOND_R_CMD));
    ds1302_datetime.week = bcd8421_to_dec2(read_byte_from_ds1302(DS1302_WEEK_R_CMD));
}
