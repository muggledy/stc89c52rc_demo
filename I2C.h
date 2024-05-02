#ifndef __I2C_H__
#define __I2C_H__

#include "global.h"
#include <REGX52.H>

#define I2C_SCL P2_1
#define I2C_SDA P2_0

extern void I2C_Start();
extern void I2C_Stop();
extern void I2C_SendByte(uint8_t byte);
extern uint8_t I2C_RecvByte();
extern void I2C_SendAck(bool_t ack);
extern bool_t I2C_RecvAck();

#endif