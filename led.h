#ifndef __LED_H__
#define __LED_H__

#include <REGX52.H>
#include "global.h"

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

extern void show_digit(uint8_t nixie_idx, uint8_t digit, uint8_t show_dot);

#endif