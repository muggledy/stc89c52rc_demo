#include "keyboard.h"

uint8_t get_key()
{
    if CHECK_PRESS(KEY1) {
        ANTI_SHAKE(KEY1);
        return 1;
    } else if CHECK_PRESS(KEY2) {
        ANTI_SHAKE(KEY2);
        return 2;
    } else if CHECK_PRESS(KEY3) {
        ANTI_SHAKE(KEY3);
        return 3;
    } else if CHECK_PRESS(KEY4) {
        ANTI_SHAKE(KEY4);
        return 4;
    }
	return 0;
}

uint8_t check_matrix_keyboard_press()
{
    uint8_t key = 0;

    P1 = 0xF7;
    if (0xF7 != P1) {
        delay_1ms(ANTI_SHAKE_MS);
        switch (P1) {
            case 0x77: key = 1; break;
            case 0xB7: key = 5; break;
            case 0xD7: key = 9; break;
            case 0xE7: key = 13; break;
        }
    }
    while(0xF7 != P1);
    
    P1 = 0xFB;
    if (0xFB != P1) {
        delay_1ms(ANTI_SHAKE_MS);
        switch (P1) {
            case 0x7B: key = 2; break;
            case 0xBB: key = 6; break;
            case 0xDB: key = 10; break;
            case 0xEB: key = 14; break;
        }
    }
    while(0xFB != P1);
    
    P1 = 0xFD;
    if (0xFD != P1) {
        delay_1ms(ANTI_SHAKE_MS);
        switch (P1) {
            case 0x7D: key = 3; break;
            case 0xBD: key = 7; break;
            case 0xDD: key = 11; break;
            case 0xED: key = 15; break;
        }
    }
    while(0xFD != P1);
    
    P1 = 0xFE;
    if (0xFE != P1) {
        delay_1ms(ANTI_SHAKE_MS);
        switch (P1) {
            case 0x7E: key = 4; break;
            case 0xBE: key = 8; break;
            case 0xDE: key = 12; break;
            case 0xEE: key = 16; break;
        }
    }
    while(0xFE != P1);
    
    return key;
}
