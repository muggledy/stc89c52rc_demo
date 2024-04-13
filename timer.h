#ifndef __TIMER_H__
#define __TIMER_H__

#include <REGX52.H>
#include <INTRINS.H>
#include "global.h"
#include "string.h"

extern void delay_1ms(uint16_t repeat);
extern void delay_1000ms(uint16_t repeat);

#define TIMER0_INTERVAL 50000 //unit: us, valid range(@11.0592MHz): [2, 71112] 
//which makes "0xFFFF - (unsigned int)((double)TIMER0_INTERVAL / MICRO_SEC_TIME_PER_COUNT) + 1" be in range of [65535, 0], 
//[65535, 0] is 16bit timer counter(TH0:TL0)'s valid range, the left boundary value means minimum time interruption(1.0850694444444442 us), 
//the right means maximum. note: considering the weak performance of the MCU(e.g., "for (;i<100;i++);" consumes 871 us), 
//TIMER0_INTERVAL should not be too small
#define MICRO_SEC_TIME_PER_COUNT 1.0850694444444442 //@11.0592MHz

#define CALC_ACC_MICRO_SEC_TIME //for get_system_up_time()
#ifdef SHOW_WITH_LCD
#define SHOW_TIME_WITH_LCD
#endif

typedef struct time_ {
    uint32_t seconds; //can record approximate 135 years
    uint32_t micro_secs; //< 1000000us (i.e., < 1s)
} time_t;

extern void sys_timer0_init();
extern void get_system_up_time(time_t *t);
extern void show_sys_up_time_with_lcd();

#define GLOBAL_TASK_LIST_MAX_NUM 6 //<=255
#define TASK_MAX_EXECUTE_TIME_IN_ONE_INTERRUPT 700000 //us

typedef struct sched_task_ {
    uint8_t flag;
#define SCHED_TASK_IS_VALID 0x01
#define SCHED_TASK_IS_RUNNING 0x02
    void (*func)(void *param);
	void *param;
	uint16_t interval; //seconds
} sched_task_t;

extern sched_task_t global_task_list[];

extern sched_task_t* sched_add(uint16_t interval, void (*func)(void *), void *param);
extern void sched_del(sched_task_t *task);

#endif