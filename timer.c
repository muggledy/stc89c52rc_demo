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

#if 1
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

uint8_t _TH0 = 0;
uint8_t _TL0 = 0;

/*
 * Init the timer counter(TH0:TL0) to trigger timer interrupt after time_interval(us)
 * Note: timer counter increments by 1 every MICRO_SEC_TIME_PER_COUNT us
 */
#define INIT_16BIT_TIMER_COUNTER(time_interval) {\
    uint32_t counter = 0xFFFF - \
        (uint32_t)((float32_t)time_interval / MICRO_SEC_TIME_PER_COUNT) + 1;\
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

uint32_t timer0_one_second_counts = 1000000 / TIMER0_INTERVAL;
uint32_t timer0_counter = 0;
uint32_t sys_timer0_seconds = 0;
#ifdef TIMER_BASED_KEY_SCAN
uint8_t timer0_20ms_counts = 20000 / TIMER0_INTERVAL;
uint8_t timer0_key_detect_counter = 0;
#endif

/* system running time(s): sys_timer0_seconds + ((TIMER0_INTERVAL * timer0_counter) / 1000000)
 * + ((TH0:TL0 - _TH0:_TL0) * MICRO_SEC_TIME_PER_COUNT / 1000000), the last(3-th) item can 
 * be directly ignored(the deviation is only in TIMER0_INTERVAL us)*/
void get_system_up_time(time_t *t)
{
    uint8_t CUR_TH0 = TH0;
    uint8_t CUR_TL0 = TL0;
    uint16_t TH0_DIFF = CUR_TH0 - _TH0;
    if (NULL == t) {
        return;
    } //Q: we may can't get accurate current system up time, for the timer0(or others) interrupt may 
    //come herein, and process_due_tasks_with_min_heap() may consume much ms
    t->seconds = sys_timer0_seconds;
    t->micro_secs = (TIMER0_INTERVAL * timer0_counter) 
#ifndef CALC_ACC_MICRO_SEC_TIME
        + 0;
#else
        + (uint32_t)(((TH0_DIFF << 8) + (CUR_TL0 - _TL0)) * MICRO_SEC_TIME_PER_COUNT);
#endif
}

void convert_time_t_to_hours(time_t *time, uint16_t *days, uint8_t *hours, 
	uint8_t *minutes, uint8_t *seconds, uint32_t *micro_secs)
{
	uint32_t t;
	*days = time->seconds / 86400;
	t = time->seconds % 86400;
	*hours = t / 3600;
	t %= 3600;
	*minutes = t / 60;
	*seconds = t % 60;
	*micro_secs = time->micro_secs;
}

#ifdef SHOW_TIME_WITH_LCD
void show_sys_up_time_with_lcd()
{
#if 0
	LCD_Init();
	LCD_ShowNum(1, 1, sys_timer0_seconds, 5);
#else
	char _string[29]; //can't defined after micro_secs, why?
	time_t t;
	uint16_t days; //need 5 chars to strore
	uint8_t hours; //need 3 chars to strore
	uint8_t minutes;
	uint8_t seconds;
	uint32_t micro_secs; //need 10 chars to strore
	INIT_STR5(days_str);
	INIT_STR3(hours_str);
	INIT_STR3(minutes_str);
	INIT_STR3(seconds_str);
	INIT_STR10(micro_secs_str);
	get_system_up_time(&t);
	convert_time_t_to_hours(&t, &days, &hours, &minutes, &seconds, &micro_secs);
	convert_uint16_to_str(days, &days_str);
	convert_uint8_to_str(hours, &hours_str);
	convert_uint8_to_str(minutes, &minutes_str);
	convert_uint8_to_str(seconds, &seconds_str);
	convert_uint32_to_str(micro_secs, &micro_secs_str);
	concate_two_strs(concate_two_strs(concate_two_strs(concate_two_strs(concate_two_strs(
		concate_two_strs(concate_two_strs(concate_two_strs(
		days_str, ":", _string), hours_str, _string), ":", _string), minutes_str, _string), ":", _string), 
		seconds_str, _string), ":", _string), micro_secs_str, _string);
	LCD_Init();
	LCD_ShowString(1, 1, _string);
#endif
}
#endif

sched_task_t global_task_list[GLOBAL_TASK_LIST_MAX_NUM];

sched_task_t* sched_add(uint16_t interval, void (*func)(void *), void *param)
{
	uint8_t i = 0;
	for (; i<GLOBAL_TASK_LIST_MAX_NUM; i++) {
		if (TST_FLAG(global_task_list + i, flag, SCHED_TASK_IS_VALID)) {
			continue;
		}
		global_task_list[i].flag = 0;
		SET_FLAG(global_task_list + i, flag, SCHED_TASK_IS_VALID);
		global_task_list[i].func = func;
		global_task_list[i].param = param;
		global_task_list[i].interval = interval;
		return global_task_list + i;
	}
	return NULL;
}

void sched_del(sched_task_t *task)
{
	if (!task) return;
	task->flag = 0; //include. CLR_FLAG(task, flag, SCHED_TASK_IS_VALID);
	//task->func = NULL;
	//task->interval = 0; //just for smaller data/xdata/code consume
}

void init_sched_task_list()
{
	uint8_t i = 0;
	for (; i<GLOBAL_TASK_LIST_MAX_NUM; i++) {
		CLR_FLAG(global_task_list+i, flag, SCHED_TASK_IS_VALID);
	}
}

/* Scheduled tasks are on the second level, if current due tasks can't be processed in one second, 
 * tasks due in the next second will not be processed in time, cause ghost delay */
void process_due_tasks()
{
	uint8_t i;
	time_t enter_t, cur_t; //initialization assignment consumes code flash space, omit(if you can) is better
	get_system_up_time(&enter_t);
	cur_t.seconds = 0;
	for (i = 0; i<GLOBAL_TASK_LIST_MAX_NUM; i++) {
		global_task_list[i].interval--;
		cur_t.seconds += TST_FLAG(global_task_list + i, flag, SCHED_TASK_IS_VALID); //just for a bit faster, maybe
	}
	if (!cur_t.seconds) return;
	for (i = 0; i<GLOBAL_TASK_LIST_MAX_NUM; i++) {
		ET0 = 0; //disable (timer0) interrupt
		if (TST_FLAG(global_task_list + i, flag, SCHED_TASK_IS_VALID) 
				&& (global_task_list[i].interval <= 0) 
				&& !TST_FLAG(global_task_list+i, flag, SCHED_TASK_IS_RUNNING)) {
			SET_FLAG(global_task_list+i, flag, SCHED_TASK_IS_RUNNING); //avoid re-excute task for nested interrupt occurs
			ET0 = 1; //enable interrupt
			global_task_list[i].func(global_task_list[i].param);
			sched_del(global_task_list+i);
			get_system_up_time(&cur_t);
			if ((cur_t.seconds > enter_t.seconds) || 
					((cur_t.micro_secs - enter_t.micro_secs) >= TASK_MAX_EXECUTE_TIME_IN_ONE_INTERRUPT)) {
				return;
			}
		}
		ET0 = 1;
	}
    return;
}

#ifdef SHOW_DS1302_RTC_WITH_LCD
uint8_t ds1302_rtc_mode = DS1302_RTC_SHOW;
uint8_t ds1302_rtc_setwhich = 0; //0:set year, 1:set month, 2:set day, 3:set hour, 4:set minute, 5:set second
uint8_t ds1302_rtc_flash = 0; //0:show null, 1:show num

void show_ds1302_rtc_with_lcd(bit read_from_ds1302_flag) //inparam 0: read from ds1302_datetime
{
	if (read_from_ds1302_flag) ds1302_read_time();
	LCD_ShowNum(1, 1, ds1302_datetime.year, 2);
	LCD_ShowNum(1, 4, ds1302_datetime.month, 2);
	LCD_ShowNum(1, 7, ds1302_datetime.day, 2);
	LCD_ShowNum(2, 1, ds1302_datetime.hour, 2);
	LCD_ShowNum(2, 4, ds1302_datetime.minute, 2);
	LCD_ShowNum(2, 7, ds1302_datetime.second, 2);
}

uint8_t get_days_num(uint8_t year, uint8_t month)
{
	if ((1==month) || (3==month) || (5==month) || (7==month) || (8==month) || (10==month) || (12==month)) {
		return 31;
	} else if ((4==month) || (6==month) || (9==month) || (11==month)) {
		return 30;
	} else if ((0==year%4) && ((0!=year%100) || (0==year%400))) {
		return 29;
	} else {
		return 28;
	}
}
#endif

#ifdef TIMER_BASED_KEY_SCAN
#define DETECT_WHICH_KEY_IS_PRESSED() {\
    if ((0 != key_state_last) && (0 == key_state_now)) {\
        if ((1 == key_state_last) && (0 == key_state_now)) {\
            which_key_is_pressed = 1;\
        } else if ((2 == key_state_last) && (0 == key_state_now)) {\
            which_key_is_pressed = 2;\
        } else if ((3 == key_state_last) && (0 == key_state_now)) {\
            which_key_is_pressed = 3;\
        } else if ((4 == key_state_last) && (0 == key_state_now)) {\
            which_key_is_pressed = 4;\
        }\
    }\
}
#endif

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
#ifdef TIMER_BASED_KEY_SCAN
    static uint8_t key_state_last = 0, key_state_now = 0;
#ifdef TIMER_BASED_NIXIE_SHOW
    static uint8_t nixie_8_decimal_cursor = 0;
    uint8_t cur_nixie_val = 0;
#endif
#endif
    RESET_16BIT_TIMER_COUNTER();
    timer0_counter++;
#ifdef TIMER_BASED_KEY_SCAN
    timer0_key_detect_counter++;
    if (timer0_key_detect_counter >= timer0_20ms_counts) {
        timer0_key_detect_counter = 0;
        key_state_now = get_cur_key_state();
        DETECT_WHICH_KEY_IS_PRESSED();
        key_state_last = key_state_now;
    }
#ifdef TIMER_BASED_NIXIE_SHOW
    cur_nixie_val = nixie_8_decimal[nixie_8_decimal_cursor];
    if (NIXIE_EOL != cur_nixie_val) {
        if (NIXIE_NULL != cur_nixie_val) {
            show_digit(7-nixie_8_decimal_cursor, DIGIT_WITHOUT_DOT(cur_nixie_val), 
                IS_DIGIT_WITH_DOT(cur_nixie_val));
        }
        nixie_8_decimal_cursor = MOD_ADD1(nixie_8_decimal_cursor, 8);
    } else {
        nixie_8_decimal_cursor = 0;
    }
#endif
#endif
    if (timer0_counter >= timer0_one_second_counts) {
        timer0_counter = 0;
        sys_timer0_seconds++;
        if (0 == sys_timer0_seconds) { //sys_timer0_seconds overflow occurs
            /*handle exception*/
        }
#if defined(SHOW_TIME_WITH_LCD)
        show_sys_up_time_with_lcd();
#elif defined(SHOW_DS1302_RTC_WITH_LCD)
		SHOW_RTC_SEPARATOR(); //remedy
		if (DS1302_RTC_SHOW == ds1302_rtc_mode) {
			show_ds1302_rtc_with_lcd(1);
		} else if (DS1302_RTC_EDIT == ds1302_rtc_mode) {
			SHOW_FLASH_RTC_WHEN_EDIT();
			ds1302_rtc_flash = !ds1302_rtc_flash;
		}
#endif
        //the time-consuming operations are as follows
        process_due_tasks();
    }
}
