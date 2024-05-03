#include "global.h"
#include "timer.h"

/*
 * STC89C52RC 40I-PDIP40
 * Note(compile option):
 * 1) Memory Model: 
 *    Large: variables in XDATA
 * 2) Option for Target: 
 *    Device: Use Extended Linker(LX51) instead of BL51
 *    LX51 Misc - Misc controls: REMOVEUNUSED
 * Author: muggledy
 */

#ifdef SHOW_ANIMATE_LED_HELLO
#include "led.h"
#endif

#ifdef SHOW_WITH_LCD
#include "LCD1602.h"
#ifdef SHOW_DS1302_RTC_WITH_LCD
#include "keyboard.h"
#endif
#endif

#ifdef EN_SERIAL
#include "serial.h"
#endif

#ifdef EN_LED_BLINK
/*Demo of scedular, add
	P2 = 0xFE;
	sched_add(1, led_blink_handler, NULL);
after sys_timer0_init(), try it!*/
#include <REGX52.H>
#include "INTRINS.H"
void led_blink_handler(void *param)
{
	P2 = _cror_(P2, 1); //_crol_
#ifdef EN_SERIAL
	//UartSendByte(P2); //WARNNING: If a function is called in the main function, 
	//the function should not(not absolute) be called in the interrupt, otherwise 
	//it may cause reentrancy error. be careful! In fact, UartSendByte not affected
#endif
	sched_add(1, led_blink_handler, NULL);
}
#endif

void main()
{    
#ifdef SHOW_DS1302_RTC_WITH_LCD
    uint8_t key_num = 0;
#define UPDATE_RTC_DAY() {\
	if (ds1302_datetime.day > 28) {\
		key_num = get_days_num(ds1302_datetime.year, ds1302_datetime.month);\
		ds1302_datetime.day = MIN(ds1302_datetime.day, key_num);\
	}\
}
#endif
#ifdef SHOW_WITH_LCD
    LCD_Init();
#ifdef SHOW_DS1302_RTC_WITH_LCD
	SHOW_RTC_SEPARATOR();
	show_ds1302_rtc_with_lcd(0);
	ds1302_set_time(); //initialize datetime, modify ds1302_datetime in DS1302.c
#else
    LCD_ShowString(1, 1, "Hi, Muggledy!");
#endif
#endif
    sys_timer0_init();
	init_sched_task_list(); //fix a bug that sched task not take effect
#ifdef EN_SERIAL
	UartInit();
	UartSendMsg("hello");
#endif
#ifdef EN_LED_BLINK
	P2 = 0xFE;
	if (sched_add(1, led_blink_handler, NULL)) {
		P2 = 0x7F; //to identify whether the task is created successfully
	}
#ifdef EN_SERIAL
	UartSendByte(P2);
#endif
#endif
#ifdef SHOW_ANIMATE_LED_HELLO
	INIT_MATRIX_LED();
	show_animate_hello_with_led_dot_matrix();
#endif
    while(1) {
        /*do something*/
#ifdef SHOW_DS1302_RTC_WITH_LCD
        key_num = get_key();
		if (1 == key_num) {
			if (DS1302_RTC_SHOW == ds1302_rtc_mode) {
				ds1302_rtc_mode = DS1302_RTC_EDIT;
				LCD_ShowNum(2, 10, ds1302_rtc_setwhich, 2);
				ds1302_rtc_flash = 0;
				SHOW_FLASH_RTC_WHEN_EDIT();
				ds1302_rtc_flash = 1;
			}
			else {
				ds1302_set_time();
				LCD_ShowString(1, 9, "        "); //remedy
				LCD_ShowString(2, 9, "        ");
				ds1302_rtc_mode = DS1302_RTC_SHOW;
			}
		} else if ((2 == key_num) && (DS1302_RTC_EDIT == ds1302_rtc_mode)) {
			show_ds1302_rtc_with_lcd(0);
			ds1302_rtc_setwhich = MOD_ADD1(ds1302_rtc_setwhich, 6); //ds1302_rtc_setwhich++%=6;
			LCD_ShowNum(2, 10, ds1302_rtc_setwhich, 2);
			ds1302_rtc_flash = 0;
			SHOW_FLASH_RTC_WHEN_EDIT();
			ds1302_rtc_flash = 1;
		} else if ((3 == key_num) && (DS1302_RTC_EDIT == ds1302_rtc_mode)) {
			if (0 == ds1302_rtc_setwhich) {
				ds1302_datetime.year = MOD_ADD1(ds1302_datetime.year, 100);
				UPDATE_RTC_DAY();
			}
			else if (1 == ds1302_rtc_setwhich) {
				ds1302_datetime.month = MOD_ADD1(ds1302_datetime.month-1, 12) + 1; //12+1=1
				UPDATE_RTC_DAY();
			}
			else if (2 == ds1302_rtc_setwhich) ds1302_datetime.day = 
				MOD_ADD1(ds1302_datetime.day-1, get_days_num(ds1302_datetime.year, ds1302_datetime.month)) + 1;
			else if (3 == ds1302_rtc_setwhich) ds1302_datetime.hour = MOD_ADD1(ds1302_datetime.hour, 24);
			else if (4 == ds1302_rtc_setwhich) ds1302_datetime.minute = MOD_ADD1(ds1302_datetime.minute, 60); //59+1=0
			else if (5 == ds1302_rtc_setwhich) ds1302_datetime.second = MOD_ADD1(ds1302_datetime.second, 60);
			show_ds1302_rtc_with_lcd(0);
		} else if ((4 == key_num) && (DS1302_RTC_EDIT == ds1302_rtc_mode)) {
			if (0 == ds1302_rtc_setwhich) {
				ds1302_datetime.year = MOD_DEC1(ds1302_datetime.year, 100);
				UPDATE_RTC_DAY();
			}
			else if (1 == ds1302_rtc_setwhich) {
				ds1302_datetime.month = MOD_DEC1(ds1302_datetime.month-1, 12) + 1; //1-1=12
				UPDATE_RTC_DAY();
			}
			else if (2 == ds1302_rtc_setwhich) ds1302_datetime.day = 
				MOD_DEC1(ds1302_datetime.day-1, get_days_num(ds1302_datetime.year, ds1302_datetime.month)) + 1;
			else if (3 == ds1302_rtc_setwhich) ds1302_datetime.hour = MOD_DEC1(ds1302_datetime.hour, 24);
			else if (4 == ds1302_rtc_setwhich) ds1302_datetime.minute = MOD_DEC1(ds1302_datetime.minute, 60); //0-1=59
			else if (5 == ds1302_rtc_setwhich) ds1302_datetime.second = MOD_DEC1(ds1302_datetime.second, 60);
			show_ds1302_rtc_with_lcd(0);
		}
#endif
    }
}