#ifndef __TIMER_H__
#define __TIMER_H__

#include <REGX52.H>
#include <INTRINS.H>
#include "global.h"
#include "string.h"

#if defined(SHOW_TIME_WITH_LCD)
#include "LCD1602.h"
#elif defined(SHOW_DS1302_RTC_WITH_LCD)
#include "LCD1602.h"
#include "DS1302.h"
#endif

extern void delay_1ms(uint16_t repeat);
extern void delay_1000ms(uint16_t repeat);
#if 1
extern void delay_10us(uint16_t n);
#endif

#define TIMER0_INTERVAL 50000 //unit: us, valid range(@11.0592MHz): [2, 71112] 
//which makes "0xFFFF - (unsigned int)((double)TIMER0_INTERVAL / MICRO_SEC_TIME_PER_COUNT) + 1" be in range of [65535, 0], 
//[65535, 0] is 16bit timer counter(TH0:TL0)'s valid range, the left boundary value means minimum time interruption(1.0850694444444442 us), 
//the right means maximum(1.0850694444444442*65536 us).
//note: considering the weak performance of the MCU(e.g., "for (;i<100;i++);" consumes 871 us), TIMER0_INTERVAL should not be too small
#define MICRO_SEC_TIME_PER_COUNT 1.0850694444444442 //@11.0592MHz 1000000us/(11.0592*10**6/12Hz)

#define CALC_ACC_MICRO_SEC_TIME //for get_system_up_time()

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

extern void init_sched_task_list();
extern sched_task_t* sched_add(uint16_t interval, void (*func)(void *), void *param);
extern void sched_del(sched_task_t *task);

#define MOD_ADD1(num, mod) ((1+(num)) % (mod))
#define MOD_DEC1(num, mod) (((mod)+(num)-1) % (mod)) //only used for clock time

#ifdef SHOW_DS1302_RTC_WITH_LCD
#define DS1302_RTC_SHOW 0
#define DS1302_RTC_EDIT 1
extern uint8_t ds1302_rtc_mode;
extern uint8_t ds1302_rtc_setwhich;
extern uint8_t ds1302_rtc_flash;
extern void show_ds1302_rtc_with_lcd(bit read_from_ds1302_flag);
extern uint8_t get_days_num(uint8_t year, uint8_t month);
#define SHOW_FLASH_RTC_WHEN_EDIT() {\
	if (0 == ds1302_rtc_setwhich) {\
		if (!ds1302_rtc_flash) LCD_ShowString(1, 1, "  ");\
		else LCD_ShowNum(1, 1, ds1302_datetime.year, 2);\
	} else if (1 == ds1302_rtc_setwhich) {\
		if (!ds1302_rtc_flash) LCD_ShowString(1, 4, "  ");\
		else LCD_ShowNum(1, 4, ds1302_datetime.month, 2);\
	} else if (2 == ds1302_rtc_setwhich) {\
		if (!ds1302_rtc_flash) LCD_ShowString(1, 7, "  ");\
		else LCD_ShowNum(1, 7, ds1302_datetime.day, 2);\
	} else if (3 == ds1302_rtc_setwhich) {\
		if (!ds1302_rtc_flash) LCD_ShowString(2, 1, "  ");\
		else LCD_ShowNum(2, 1, ds1302_datetime.hour, 2);\
	} else if (4 == ds1302_rtc_setwhich) {\
		if (!ds1302_rtc_flash) LCD_ShowString(2, 4, "  ");\
		else LCD_ShowNum(2, 4, ds1302_datetime.minute, 2);\
	} else if (5 == ds1302_rtc_setwhich) {\
		if (!ds1302_rtc_flash) LCD_ShowString(2, 7, "  ");\
		else LCD_ShowNum(2, 7, ds1302_datetime.second, 2);\
	}\
}
#define SHOW_RTC_SEPARATOR() {\
	LCD_ShowString(1, 3, "-");\
	LCD_ShowString(1, 6, "-");\
	LCD_ShowString(2, 3, ":");\
	LCD_ShowString(2, 6, ":");\
}
#endif

#endif