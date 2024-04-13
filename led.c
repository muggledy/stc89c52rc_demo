#include "led.h"

/*
 * Led
 */

const uint8_t leds[8] = {D1, D2, D3, D4, D5, D6, D7, D8};

/*
 * Nixie tube
 */

#define N0 0x3F //abcdef, 00111111
#define N1 0x06 //bc, 00000110
#define N2 0x5B //abged, 01011011
#define N3 0x4F //abgcd, 01001111
#define N4 0x66 //bcgf, 01100110
#define N5 0x6D //afgcd, 01101101
#define N6 0x7D //afgcde, 01111101
#define N7 0x07 //abc, 00000111
#define N8 0x7F //abcdefg, 01111111
#define N9 0x6F //abcdfg, 01101111

#define N_WITH_DOT(NX) ((NX) | 0x80)

const uint8_t digits[10] = {N0, N1, N2, N3, N4, N5, N6, N7, N8, N9};
#if 0
const uint8_t digits_with_dot[10] = 
    {N_WITH_DOT(N0), N_WITH_DOT(N1), N_WITH_DOT(N2), N_WITH_DOT(N3), 
    N_WITH_DOT(N4), N_WITH_DOT(N5), 
    N_WITH_DOT(N6), N_WITH_DOT(N7), N_WITH_DOT(N8), N_WITH_DOT(N9)};
#endif

void show_digit(uint8_t nixie_idx, uint8_t digit, bool_t show_dot)
{   
    P0 = 0;
    P2 = 0xFF; //slake all leds
    nixie_idx %= 8;
    digit %= 10;

    P2_4 = (nixie_idx & 0x4) ? 1 : 0;
    P2_3 = (nixie_idx & 0x2) ? 1 : 0;
    P2_2 = (nixie_idx & 0x1) ? 1 : 0;
#if 0
    P0 = show_dot ? digits_with_dot[digit] : digits[digit]; //Program Size: data=9.0 xdata=172 code=4421
#endif
	P0 = show_dot ? N_WITH_DOT(digits[digit]) : digits[digit]; //Program Size: data=9.0 xdata=162 code=4414
#if 0
	P0 = show_dot ? N_WITH_DOT(0 == digit ? N0 : (1 == digit ? N1 : 
		(2 == digit ? N2 : (3 == digit ? N3 : (4 == digit ? N4 : 
		(5 == digit ? N5 : (6 == digit ? N6 : (7 == digit ? N7 : 
		(8 == digit ? N8 : N9))))))))) : 
		(0 == digit ? N0 : (1 == digit ? N1 : (2 == digit ? N2 : 
		(3 == digit ? N3 : (4 == digit ? N4 : (5 == digit ? N5 : 
		(6 == digit ? N6 : (7 == digit ? N7 : (8 == digit ? N8 : N9))))))))); //Program Size: data=9.0 xdata=152 code=4525
#endif
}
