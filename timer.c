#include "timer.h"

void delay_1ms(unsigned int n)		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	while (n) {
        i = 2;
        j = 199;
        do
        {
            while (--j);
        } while (--i);
        n--;
	}
}

void delay_1000ms(unsigned int n)		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
    while (n) {
        i = 8;
        j = 1;
        k = 243;
        do
        {
            do
            {
                while (--k);
            } while (--j);
        } while (--i);
        n--;
    }
}

unsigned char _TH0 = 0;
unsigned char _TL0 = 0;

/*
 * Init the timer counter(TH0:TL0) to trigger timer interrupt after time_interval(us)
 * Note: timer counter increments by 1 every 1.0850694444444442 us
 */
#define INIT_16BIT_TIMER_COUNTER(time_interval) {\
    unsigned int counter = 0xFFFF - \
        (unsigned int)((double)time_interval / 1.0850694444444442) + 1;\
    TH0 = _TH0 = counter >> 8;\
    TL0 = _TL0 = counter & 0x00FF;\
}

#define RESET_16BIT_TIMER_COUNTER() {\
    TH0 = _TH0;\
    TL0 = _TL0;\
}

/*
 * Start the Timer0
 */
void sys_timer0_init()
{
    TMOD = TMOD & 0xF0;
    TMOD = TMOD | 0x01;
    TF0 = 0;
    TR0 = 1;
    INIT_16BIT_TIMER_COUNTER(TIMER0_INTERVAL); //Trap: function call and division operation consumes much us 
    //which makes stopwatch not accurate, that's why we write RESET_16BIT_TIMER_COUNTER, see demo of Timer0_Routine()
    ET0 = 1;
    EA = 1;
    PT0 = 0;
}

int one_second_counts = 1000000 / TIMER0_INTERVAL;

/*
 * Interrupt handler for Timer0, triggers every TIMER0_INTERVAL us
 * Demo(show stopwatch):
 * void Timer0_Routine() interrupt 1
 * {
 *     static int timer0_counter = 0;
 *     static int seconds = 0;
 *     RESET_16BIT_TIMER_COUNTER();
 *     timer0_counter++;
 *     if (timer0_counter >= one_second_counts) {
 *         seconds++;
 *         LCD_Init();
 *         LCD_ShowNum(1, 1, seconds, 5);
 *         timer0_counter = 0;
 *     }
 * }
 */
void Timer0_Routine() interrupt 1
{
    static int timer0_counter = 0;
    static int seconds = 0;
    
    RESET_16BIT_TIMER_COUNTER();
    timer0_counter++;
    if (timer0_counter >= one_second_counts) {
        seconds++;
        LCD_Init();
        LCD_ShowNum(1, 1, seconds, 5);
        timer0_counter = 0;
    }
}
