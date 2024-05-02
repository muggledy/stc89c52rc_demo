#include "AT24C02.h"

void AT24C02_WriteByte(uint8_t word_address, uint8_t byte)
{
    I2C_Start();
    I2C_SendByte(AT24C02_W_ADDRESS);
    I2C_RecvAck();
    I2C_SendByte(word_address);
    I2C_RecvAck();
    I2C_SendByte(byte);
    I2C_RecvAck();
    I2C_Stop();
}

uint8_t AT24C02_ReadByte(uint8_t word_address)
{
    uint8_t byte = 0;
    I2C_Start();
    I2C_SendByte(AT24C02_W_ADDRESS);
    I2C_RecvAck();
    I2C_SendByte(word_address);
    I2C_RecvAck();
    I2C_Start();
    I2C_SendByte(AT24C02_R_ADDRESS);
    I2C_RecvAck();
    byte = I2C_RecvByte();
    I2C_SendAck(1);
    I2C_Stop();
    return byte;
}
