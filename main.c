#include "LCD1602.h"
#include "timer.h"

/*
 * STC89C52RC 40I-PDIP40
 */



void main()
{
    LCD_Init();
    LCD_ShowString(1, 1, "Hi, Muggledy!");
    sys_timer0_init();
    while(1) {
        
    }
}