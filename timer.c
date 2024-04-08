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
 * Note: timer counter increments by 1 every MICRO_SEC_TIME_PER_COUNT us
 */
#define INIT_16BIT_TIMER_COUNTER(time_interval) {\
    unsigned int counter = 0xFFFF - \
        (unsigned int)((double)time_interval / MICRO_SEC_TIME_PER_COUNT) + 1;\
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
    //which makes stopwatch not accurate, that's why we introduce RESET_16BIT_TIMER_COUNTER, see demo of Timer0_Routine()
    ET0 = 1;
    EA = 1;
    PT0 = 0;
}

unsigned int timer0_one_second_counts = 1000000 / TIMER0_INTERVAL;
unsigned int timer0_counter = 0;
unsigned long sys_timer0_seconds = 0;

/* system running time(s): sys_timer0_seconds + ((TIMER0_INTERVAL * timer0_counter) / 1000000)
 * + ((TH0:TL0 - _TH0:_TL0) * MICRO_SEC_TIME_PER_COUNT / 1000000), the last(3-th) item can 
 * be directly ignored(the deviation is only in TIMER0_INTERVAL us)*/
void get_system_up_time(time_t *t)
{
    unsigned char CUR_TH0 = TH0;
    unsigned char CUR_TL0 = TL0;
    unsigned short TH0_DIFF = CUR_TH0 - _TH0;
    if (NULL == t) {
        return;
    } //Q: we may can't get accurate current system up time, for the timer0(or others) interrupt may 
    //come herein, and process_due_tasks_with_min_heap() may consume much ms
    t->seconds = sys_timer0_seconds;
    t->micro_secs = (TIMER0_INTERVAL * timer0_counter) 
#ifndef CALC_ACC_MICRO_SEC_TIME
        + 0;
#else
        + (unsigned int)(((TH0_DIFF << 8) + (CUR_TL0 - _TL0)) * MICRO_SEC_TIME_PER_COUNT);
#endif
}

void show_sys_up_time_with_lcd()
{
	LCD_Init();
    LCD_ShowNum(1, 1, sys_timer0_seconds, 5);
#if 0
    time_t t;
    get_system_up_time(&t);
    LCD_ShowNum(1, 1, t.seconds, 5);
#endif
}

/* Scheduled tasks are on the second level, if current due tasks can't be processed in one second, 
 * tasks due in the next second will not be processed in time, cause ghost delay */
void process_due_tasks_with_min_heap()
{
    return;
}

/*
 * Interrupt handler for Timer0, triggers 
 * every TIMER0_INTERVAL us
 * The corresponding interrupt vector will point to the address of 
 * this function(Timer0_Routine), when timer0 interrupt occurs, 
 * this function will be called
 * Demo(show stopwatch):
 * void Timer0_Routine() interrupt 1
 * {
 *     static int timer0_counter = 0;
 *     static int seconds = 0;
 *     RESET_16BIT_TIMER_COUNTER();
 *     timer0_counter++;
 *     if (timer0_counter >= timer0_one_second_counts) {
 *         seconds++;
 *         LCD_Init();
 *         LCD_ShowNum(1, 1, seconds, 5);
 *         timer0_counter = 0;
 *     }
 * }
 */
void Timer0_Routine() interrupt 1
{
    RESET_16BIT_TIMER_COUNTER();
    timer0_counter++;
    if (timer0_counter >= timer0_one_second_counts) {
        timer0_counter = 0;
        sys_timer0_seconds++;
        if (0 == sys_timer0_seconds) { //sys_timer0_seconds overflow occurs
            /*handle exception*/
        }
#ifdef SHOW_TIME_WITH_LCD
        show_sys_up_time_with_lcd();
#endif
        //the time-consuming operations are as follows
        process_due_tasks_with_min_heap();
    }
}
