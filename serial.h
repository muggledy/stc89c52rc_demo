#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <REGX52.H>
#include "global.h"

//#define UART_REFLECTION_MODE //just for test

extern void UartInit(void);

/*TI==1 indicates that the transmission is succeed*/
#define UartSendByte(byte) {\
    SBUF = (uint8_t)byte;\
    while(0 == TI);\
    TI = 0;\
}

extern void UartSendMsg(char *msg);

#endif