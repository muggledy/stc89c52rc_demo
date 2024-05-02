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
#define INIT_16BIT_TIMER_COUNTER(time_interval) {\
    uint32_t counter = 0xFFFF - \
        (uint32_t)((float32_t)time_interval / MICRO_SEC_TIME_PER_COUNT) + 1;\
    SET_16BIT_TIMER_COUNTER(counter);\
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
