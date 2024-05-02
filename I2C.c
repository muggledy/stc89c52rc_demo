#include "I2C.h"

void I2C_Start()
{
    I2C_SDA = 1; //current I2C_SCL must be 0
    I2C_SCL = 1;
    I2C_SDA = 0;
    I2C_SCL = 0;
}

void I2C_Stop()
{
    I2C_SDA = 0; //current I2C_SCL must be 0
    I2C_SCL = 1;
    I2C_SDA = 1;
}

void I2C_SendByte(uint8_t byte)
{
    uint8_t i = 0;
    for(; i<8; i++) {
        I2C_SDA = byte & (0x80 >> i);
        I2C_SCL = 1;
        I2C_SCL = 0;
    }
}

uint8_t I2C_RecvByte()
{
    uint8_t i = 0, byte = 0;
    I2C_SDA = 1;
    for(; i<8; i++) {
        I2C_SCL = 1;
        if (I2C_SDA) {
            byte |= (0x80 >> i);
        }
        I2C_SCL = 0;
    }
    return byte;
}

void I2C_SendAck(bool_t ack)
{
    I2C_SDA = ack;
    I2C_SCL = 1;
    I2C_SCL = 0;
}

bool_t I2C_RecvAck() //return 0:reply, 1:not reply
{
    bool_t ack = 0;
    I2C_SDA = 1;
    I2C_SCL = 1;
    ack = I2C_SDA;
    I2C_SCL = 0;
    return ack;
}
