#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <REGX52.H>
#include "timer.h"
#include "global.h"

/*
 * Demo for K1~K4:
 * if CHECK_PRESS(KEY1) {
 *     ANTI_SHAKE(KEY1);
 *     LCD_ShowString(1, 1, "Hi, ZhangJing");
 * } else if CHECK_PRESS(KEY2) {
 *     ANTI_SHAKE(KEY2);
 *     LCD_ShowString(1, 1, "Hi, Celestezj");
 * } else if ...
 */

#define KEY1 P3_1
#define KEY2 P3_0
#define KEY3 P3_2
#define KEY4 P3_3

#define CHECK_PRESS(KEY) (0 == KEY)
#define ANTI_SHAKE(KEY) {\
    delay_1ms(20);\
    while (0 == KEY);\
    delay_1ms(20);\
}

extern uint8_t get_key();
#ifdef TIMER_BASED_KEY_SCAN
extern uint8_t which_key_is_pressed;
#if 0
extern uint8_t get_cur_key_state();
#else
#define get_cur_key_state() (CHECK_PRESS(KEY1) ? 1 : (CHECK_PRESS(KEY2) ? 2 : (CHECK_PRESS(KEY3) ? 3 : (CHECK_PRESS(KEY4) ? 4 : 0))))
#endif
#endif

/*
 * Demo for Matrix Keyboard(S1~S16):
 * while(1) {
 *     key = check_matrix_keyboard();
 *     if (0 != key) {
 *         LCD_ShowNum(1, 1, key, 2); //show the matrix keyboard number(1~16)
 *     }
 * }
 */

#define ANTI_SHAKE_MS 20
extern uint8_t check_matrix_keyboard_press();

#endif