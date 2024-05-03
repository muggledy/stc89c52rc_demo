#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/*SHOW_WITH_LCD and EN_LED_BLINK are mutually exclusive. 
SHOW_ANIMATE_LED_HELLO and others are mutually exclusive*/
//#define SHOW_WITH_LCD //further can control SHOW_TIME_WITH_LCD, etc.
//#define EN_LED_BLINK
//#define EN_SERIAL
//#define SHOW_ANIMATE_LED_HELLO
//#define TIMER_BASED_KEY_SCAN //just leading to different implementations of func get_key() in keyboard.c
                             //TIMER_BASED_KEY_SCAN: non-blocking detection of key input
                             //further can control TIMER_BASED_NIXIE_SHOW

#ifdef SHOW_WITH_LCD /*SHOW_TIME_WITH_LCD and RTC_READ_WRITE_SHOW are mutually exclusive*/
//#define SHOW_TIME_WITH_LCD //show stopwatch
//#define SHOW_DS1302_RTC_WITH_LCD //show RTC(Real Time Clock)
#endif

#ifdef TIMER_BASED_KEY_SCAN
//#define TIMER_BASED_NIXIE_SHOW //further implement non-blocking multi-Nixie dynamic display: 
                               //see show_8_digit_decimal() in led.c
#endif

#define NULL  0
#define true  1
#define false 0
typedef bit bool_t;

typedef unsigned char  uint8_t;   //0 to 255
typedef signed char    int8_t;    //-128 to +127
typedef unsigned short uint16_t;  //0 to 65535, type int also 16bits
typedef signed short   int16_t;   //-32768 to +32767
typedef unsigned long  uint32_t;  //0 to 4294967295
typedef signed long    int32_t;   //-2147483648 to +2147483647
typedef float          float32_t; //32bits, also is double

#define SET_FLAG(obj, field, flag) (obj)->field |= flag
#define CLR_FLAG(obj, field, flag) (obj)->field &= (~flag)
#define TST_FLAG(obj, field, flag) ((obj)->field & flag)

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MIN(a, b) ((a) <= (b) ? (a) : (b))

#endif