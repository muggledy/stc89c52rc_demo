#ifndef __DELAY_H__
#define __DELAY_H__

#include <REGX52.H>
#include <INTRINS.H>
#include "LCD1602.h"

extern void delay_1ms(unsigned int repeat);
extern void delay_1000ms(unsigned int repeat);

#define TIMER0_INTERVAL 50000 //us

extern void sys_timer0_init();

#endif