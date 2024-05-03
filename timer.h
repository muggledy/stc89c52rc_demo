#ifndef __TIMER_H__
#define __TIMER_H__

#include <REGX52.H>
#include <INTRINS.H>
#include "global.h"

extern void delay_1ms(uint16_t repeat);
#if 0
extern void delay_1000ms(uint16_t repeat);
extern void delay_10us(uint16_t n);
#endif

#define TIMER0_INTERVAL 50000 //unit: us, valid range(@11.0592MHz): [2, 71112] 
//which makes "0xFFFF - (unsigned int)((double)TIMER0_INTERVAL / MICRO_SEC_TIME_PER_COUNT) + 1" be in range of [65535, 0], 
//[65535, 0] is 16bit timer counter(TH0:TL0)'s valid range, the left boundary value means minimum time interruption(1.0850694444444442 us), 
//the right means maximum. note: considering the weak performance of the MCU(e.g., "for (;i<100;i++);" consumes 871 us), 
//TIMER0_INTERVAL should not be too small
#define MICRO_SEC_TIME_PER_COUNT 1.0850694444444442 //@11.0592MHz

extern void sys_timer0_init();

#define OPEN_TIMER0() {\
    TR0 = 1;\
}
#define CLOSE_TIMER0() {\
    TR0 = 0;\
}

/*wake up timer0 interrupt after 1.0850694444444442 us*/
#define WAKEUP_TIMER0_INTERRUPT_AT_ONCE() {\
    TH0 = 255;\
    TL0 = 255;\
}

#define SET_16BIT_TIMER0_COUNTER(counter) {\
    TH0 = (counter) >> 8;\
    TL0 = (counter) & 0x00FF;\
}

#define TIMER1_INTERVAL 71112

extern void sys_timer1_init();

extern uint8_t _TH1;
extern uint8_t _TL1;
extern uint32_t timer1_one_second_counts;
extern uint32_t timer1_counter;

#define RESET_16BIT_TIMER1_COUNTER() {\
    TH1 = _TH1;\
    TL1 = _TL1;\
}

#define OPEN_TIMER1() {\
    TR1 = 1;\
}
#define CLOSE_TIMER1() {\
    TR1 = 0;\
}

#endif