#ifndef __LED_H__
#define __LED_H__

#include <REGX52.H>
#include "global.h"
#ifdef SHOW_ANIMATE_LED_HELLO
#include "timer.h"
#endif

/*
 * Demo for Led:
 * OPEN_LED(leds[0]) or OPEN_LED(D1)
 * CLOSE_LED(leds[0]) or CLOSE_LED(D1)
 * RESET_LED(LIGHT_ALL) or RESET_LED(SLAKE_ALL)
 */

#define D1 0xFE
#define D2 0xFD
#define D3 0xFB
#define D4 0xF7
#define D5 0xEF
#define D6 0xDF
#define D7 0xBF
#define D8 0x7F

#define RESET_LED(BOOL) (P2 = (BOOL) ? 0 : 0xFF)
#define OPEN_LED(DX)    (P2 &= DX)
#define CLOSE_LED(DX)   (P2 |= (~ DX))

#define LIGHT_ALL 1
#define SLAKE_ALL 0

extern const uint8_t leds[];

/*
 * Demo for Nixie Tube:
 * show_digit(0, 8, true); //show digit 8 with dot on the first nixie tube
 */

extern void show_digit(uint8_t nixie_idx, uint8_t digit, bool_t show_dot);

#ifdef TIMER_BASED_NIXIE_SHOW
extern uint8_t nixie_8_decimal[];
#define NIXIE_NULL 10
#define NIXIE_EOL  11
#define DIGIT_WITH_DOT(x)    ((x) | 0x80) //valid x:0~9
#define IS_DIGIT_WITH_DOT(x) ((x) & 0x80)
#define DIGIT_WITHOUT_DOT(x) ((x) & (~0x80))
#define init_8_digit_decimal() {\
    nixie_8_decimal[0] = NIXIE_EOL;\
}
/*
 * Demo: show_8_digit_decimal(2,DIGIT_WITH_DOT(5),DIGIT_WITH_DOT(0),NIXIE_NULL,6,NIXIE_EOL,7,NIXIE_NULL);
 * this will display:"25.0. 6   ". Note: displayed by timer0 interrupt
 */
#if 0
extern void show_8_digit_decimal(uint8_t n7, n6, n5, n4, n3, n2, n1, n0, dot_idx);
#else
#define show_8_digit_decimal(n7, n6, n5, n4, n3, n2, n1, n0) {\
    nixie_8_decimal[0] = n7;\
    nixie_8_decimal[1] = n6;\
    nixie_8_decimal[2] = n5;\
    nixie_8_decimal[3] = n4;\
    nixie_8_decimal[4] = n3;\
    nixie_8_decimal[5] = n2;\
    nixie_8_decimal[6] = n1;\
    nixie_8_decimal[7] = n0;\
}
#endif
/*after calling clear_8_digit_decimal(), the timer0 interrupt won't control nixie leds*/
#define clear_8_digit_decimal() {\
    nixie_8_decimal[0] = NIXIE_EOL;\
}
#endif

/*
 * Demo for LED Dot Matrix Screen(remember to remove J24 pin cap):
 *         (P07)(P06)  (P00)
 *          C0   C1 ... C7
 * (QH) L0
 * (QG) L1
 * ...
 * (QA) L7
 * 1. show the lower three lines of the dot matrix screen:
 * INIT_MATRIX_LED();
 * P0 = 0x00;
 * write_byte_into_74hc595(0x07);
 * 2. show the first three leds in the first column of the dot matrix screen:
 * INIT_MATRIX_LED();
 * show_matrix_led_column(1, 0xE0); //rows_mask:11100000. i.e., show_matrix_led_with_location(0xE0, 0x80)
 * 3. display the first three rows and first three columns in the upper left corner of the dot matrix screen:
 * INIT_MATRIX_LED();
 * show_matrix_led_with_location(0xE0, 0xE0); //rows_mask:11100000, cols_mask:11100000
 * 4. light the LED at the coordinate position (1, 3)/(row, column) of the dot matrix screen:
 * INIT_MATRIX_LED();
 * show_matrix_led_with_location(0x80, 0x20); //rows_mask:10000000, cols_mask:00100000
 */

#define SER P3_4
#define SCK P3_6 //i.e., SRCLK
#define RCK P3_5 //i.e., RCLK. sbit RCK = P3^5

#define INIT_MATRIX_LED() {\
	SCK = 0;\
	RCK = 0;\
	P0 = 0x00;\
    write_byte_into_74hc595(0);\
}

extern void write_byte_into_74hc595(uint8_t byte);
#if 0
extern void show_matrix_led_column(uint8_t column, uint8_t rows_mask);
#endif
extern void show_matrix_led_with_location(uint8_t rows_mask, uint8_t columns_mask);

#ifdef SHOW_ANIMATE_LED_HELLO
extern void show_animate_hello_with_led_dot_matrix();
#endif

#endif