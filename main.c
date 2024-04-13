#include "global.h"
#include "timer.h"

/*
 * STC89C52RC 40I-PDIP40
 * by muggledy
 */

#if 1
/*Demo of scedular, add
	P2 = 0xFE;
	sched_add(1, led_blink_handler, NULL);
after sys_timer0_init(), try it!*/
#include <REGX52.H>
#include "INTRINS.H"
void led_blink_handler(void *param)
{
	P2 = _cror_(P2, 1);
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
	P2 = 0xFE;
	sched_add(1, led_blink_handler, NULL);
    while(1) {
        /*do something*/
    }
}