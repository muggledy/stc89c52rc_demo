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

#define LED_BLINK

#ifdef LED_BLINK
/*Demo of scedular, add
	P2 = 0xFE;
	sched_add(1, led_blink_handler, NULL);
after sys_timer0_init(), try it!*/
#include <REGX52.H>
#include "INTRINS.H"
void led_blink_handler(void *param)
{
	P2 = _cror_(P2, 1); //_crol_
	sched_add(1, led_blink_handler, NULL);
}
#endif

void main()
{
#ifdef SHOW_WITH_LCD
    LCD_Init();
    LCD_ShowString(1, 1, "Hi, Muggledy!");
#endif
    sys_timer0_init();
	init_sched_task_list(); //fix a bug that sched task not take effect
#ifdef LED_BLINK
	P2 = 0xFE;
	if (sched_add(1, led_blink_handler, NULL)) {
		P2 = 0x7F; //to identify whether the task is created successfully
	}
#endif
    while(1) {
        /*do something*/
    }
}