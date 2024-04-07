#ifndef __TIMER_H__
#define __TIMER_H__

#include <REGX52.H>
#include <INTRINS.H>
#include "LCD1602.h"
#include "global.h"

extern void delay_1ms(unsigned int repeat);
extern void delay_1000ms(unsigned int repeat);

#define TIMER0_INTERVAL 100000 //us
#define MICRO_SEC_TIME_PER_COUNT 1.0850694444444442 //@11.0592MHz

#define CALC_ACC_MICRO_SEC_TIME //for get_system_up_time()
#define SHOW_TIME_WITH_LCD

typedef struct time_ {
    unsigned long seconds;
    unsigned int micro_secs; //<1000000us
} time_t;

/*
#define SCHED_TASK_IS_RUNNING 0x01

typedef struct sched_task_ {
    unsigned char flag;
    
} sched_task_t;
*/

extern void sys_timer0_init();
extern void get_system_up_time(time_t *t);
extern void show_sys_up_time_with_lcd();

#endif