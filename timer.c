#include "timer.h"

void delay_1ms(uint16_t n)		//@11.0592MHz
{
	uint8_t i, j;

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

#if 0
void delay_1000ms(uint16_t n)		//@11.0592MHz
{
	uint8_t i, j, k;

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

void delay_10us(uint16_t n)		//@11.0592MHz
{
	unsigned char i;

	while (n) {
		i = 2;
		while (--i);
		n--;
	}
}
#endif

/*
 * Init the timer counter(TH0:TL0) to trigger timer interrupt after time_interval(us)
 * Note: timer counter increments by 1 every MICRO_SEC_TIME_PER_COUNT us
 */
#define INIT_16BIT_TIMER0_COUNTER(time_interval) {\
    uint32_t counter = 0xFFFF - \
        (uint32_t)((float32_t)time_interval / MICRO_SEC_TIME_PER_COUNT) + 1;\
    SET_16BIT_TIMER0_COUNTER(counter);\
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
    INIT_16BIT_TIMER0_COUNTER(TIMER0_INTERVAL); //Trap: function call and division operation consumes much us 
    //which makes stopwatch not accurate, that's why we introduce RESET_16BIT_TIMER_COUNTER, see demo of Timer0_Routine()
    ET0 = 1;
    EA = 1;
    PT0 = 0; //1(Timer0优先级理论上应设置高于Timer1,但实测不行)
}

uint8_t _TH1 = 0;
uint8_t _TL1 = 0;
uint32_t timer1_one_second_counts = 1000000 / TIMER1_INTERVAL;
uint32_t timer1_counter = 0;

#define SET_16BIT_TIMER1_COUNTER_WITH_BACKUP(counter) {\
    TH1 = _TH1 = (counter) >> 8;\
    TL1 = _TL1 = (counter) & 0x00FF;\
}

#define INIT_16BIT_TIMER1_COUNTER(time_interval) {\
    uint32_t counter = 0xFFFF - \
        (uint32_t)((float32_t)time_interval / MICRO_SEC_TIME_PER_COUNT) + 1;\
    SET_16BIT_TIMER1_COUNTER_WITH_BACKUP(counter);\
}

/*
 * Start the Timer1
 */
void sys_timer1_init()		//1000微秒@11.0592MHz
{
	//AUXR &= 0xBF;   //定时器时钟12T模式
	TMOD &= 0x0F;   //设置定时器模式
    TMOD |= 0x10;   //16位定时器,TL1,TH1全用
	INIT_16BIT_TIMER1_COUNTER(TIMER1_INTERVAL);
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
    ET1 = 1;
    EA = 1;
    PT1 = 0;        //低优先级
}
