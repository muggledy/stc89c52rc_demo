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
	CLR_FLAG(task, flag, SCHED_TASK_IS_VALID);
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
		if (TST_FLAG(global_task_list + i, flag, SCHED_TASK_IS_VALID) 
				&& (global_task_list[i].interval <= 0) 
				&& !TST_FLAG(global_task_list+i, flag, SCHED_TASK_IS_RUNNING)) {
			SET_FLAG(global_task_list+i, flag, SCHED_TASK_IS_RUNNING); //avoid re-excute task for nested interrupt occurs
			global_task_list[i].func(global_task_list[i].param);
			sched_del(global_task_list+i);
			get_system_up_time(&cur_t);
			if ((cur_t.seconds > enter_t.seconds) || 
					((cur_t.micro_secs - enter_t.micro_secs) >= TASK_MAX_EXECUTE_TIME_IN_ONE_INTERRUPT)) {
				return;
			}
		}
	}
    return;
}

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
    RESET_16BIT_TIMER_COUNTER();
    timer0_counter++;
    if (timer0_counter >= timer0_one_second_counts) {
        timer0_counter = 0;
        sys_timer0_seconds++;
        if (0 == sys_timer0_seconds) { //sys_timer0_seconds overflow occurs
            /*handle exception*/
        }
#ifdef SHOW_TIME_WITH_LCD
        show_sys_up_time_with_lcd();
#endif
        //the time-consuming operations are as follows
        process_due_tasks();
    }
}
