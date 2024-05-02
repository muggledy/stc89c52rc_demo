#include "global.h"
#include "timer.h"
#include <REGX52.H>

#define BUZZER P2_5

/*音符表,对应编号(0~36)用于索引note_freq_tbl音频表中对应的"频率"
(实际非频率值,但和频率正相关),
P:休止符,L:低音,M:中音,H:高音,下划线:升半音*/
#define P	0
#define L1	1
#define L1_	2
#define L2	3
#define L2_	4
#define L3	5
#define L4	6
#define L4_	7
#define L5	8
#define L5_	9
#define L6	10
#define L6_	11
#define L7	12
#define M1	13
#define M1_	14
#define M2	15
#define M2_	16
#define M3	17
#define M4	18
#define M4_	19
#define M5	20
#define M5_	21
#define M6	22
#define M6_	23
#define M7	24
#define H1	25
#define H1_	26
#define H2	27
#define H2_	28
#define H3	29
#define H4	30
#define H4_	31
#define H5	32
#define H5_	33
#define H6	34
#define H6_	35
#define H7	36

/*不同频率音符对应的时钟中断计数器初值(与上述音符表一一对应),计数器每溢出一次就
翻转一下蜂鸣器电平信号,
通过定时器发出指定频率的声音:计数器初值越高,振动周期时长越短,振动频率越高,音调越高,
反之音调越低.第一个音符比较特殊,是为休止符,无声,对应计数器初值设为特殊0,表示不向
蜂鸣器发出振动信号
note_freq_tbl计算(python):
hz = [262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 
 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 
 932, 988, 1046, 1109, 1175, 1245, 1318, 1397, 1480, 1568, 1661, 1760, 
 1865, 1976]
#count = [(65535 - ((1000000/(_*2)) / (1000000/(11.0592*10**6/12))) + 1) for _ in hz]
count = [round((65535 - ((1000000/(_*2)) / (1/(11.0592/12))) + 1)) for _ in hz]
'''(1/(11.0592/12))是计数器增1消耗的微秒数,(1000000/(_*2))是特定频率音符两次振动信号
之间的微秒数'''
print(f"[{','.join([str(_) for _ in count])}]")
*/
uint16_t note_freq_tbl[] = {0,
    63777,63872,63969,64054,64140,64216,64291,64360,64426,64489,64547,64603,
    64655,64704,64751,64795,64837,64876,64913,64948,64981,65012,65042,65070,
    65095,65120,65144,65166,65186,65206,65225,65242,65259,65274,65289,65303
}; //@11.0592MHz

#define NOTE_BASE_SPEED 125 //ms
#define MUSIC_TBL_ITEM_LEN 2

/*音乐：[<音符,时长(倍率)>, ...]*/
uint8_t code music_tbl [] = {
    //1
	P,	4,
	P,	4,
	P,	4,
	M6,	2,
	M7,	2,
	
	H1,	4+2,
	M7,	2,
	H1,	4,
	H3,	4,
	
	M7,	4+4+4,
	M3,	2,
	M3,	2,
	
	//2
	M6,	4+2,
	M5,	2,
	M6, 4,
	H1,	4,
	
	M5,	4+4+4,
	M3,	4,
	
	M4,	4+2,
	M3,	2,
	M4,	4,
	H1,	4,
	
	//3
	M3,	4+4,
	P,	2,
	H1,	2,
	H1,	2,
	H1,	2,
	
	M7,	4+2,
	M4_,2,
	M4_,4,
	M7,	4,
	
	M7,	8,
	P,	4,
	M6,	2,
	M7,	2,
	
	//4
	H1,	4+2,
	M7,	2,
	H1,	4,
	H3,	4,
	
	M7,	4+4+4,
	M3,	2,
	M3,	2,
	
	M6,	4+2,
	M5,	2,
	M6, 4,
	H1,	4,
	
	//5
	M5,	4+4+4,
	M2,	2,
	M3,	2,
	
	M4,	4,
	H1,	2,
	M7,	2+2, //沿音写为一个音符
	H1,	2+4,
	
	H2,	2,
	H2,	2,
	H3,	2,
	H1,	2+4+4,
	
	//6
	H1,	2,
	M7,	2,
	M6,	2,
	M6,	2,
	M7,	4,
	M5_,4,
	
	
	M6,	4+4+4,
	H1,	2,
	H2,	2,
	
	H3,	4+2,
	H2,	2,
	H3,	4,
	H5,	4,
	
	//7
	H2,	4+4+4,
	M5,	2,
	M5,	2,
	
	H1,	4+2,
	M7,	2,
	H1,	4,
	H3,	4,
	
	H3,	4+4+4+4,
	
	//8
	M6,	2,
	M7,	2,
	H1,	4,
	M7,	4,
	H2,	2,
	H2,	2,
	
	H1,	4+2,
	M5,	2+4+4,
	
	H4,	4,
	H3,	4,
	H3,	4,
	H1,	4,
	
	//9
	H3,	4+4+4,
	H3,	4,
	
	H6,	4+4,
	H5,	4,
	H5,	4,
	
	H3,	2,
	H2,	2,
	H1,	4+4,
	P,	2,
	H1,	2,
	
	//10
	H2,	4,
	H1,	2,
	H2,	2,
	H2,	4,
	H5,	4,
	
	H3,	4+4+4,
	H3,	4,
	
	H6,	4+4,
	H5,	4+4,
	
	//11
	H3,	2,
	H2,	2,
	H1,	4+4,
	P,	2,
	H1,	2,
	
	H2,	4,
	H1,	2,
	H2,	2+4,
	M7,	4,
	
	M6,	4+4+4,
	P,	4,
    
    0xFF //终止标志
};
uint16_t music_tbl_cursor = 0;

void main()
{
    sys_timer0_init(); //init TH0 and TL0 herein should be small, 0 is also ok

    while(1) {
        if (0xFF != music_tbl[music_tbl_cursor]) {
            WAKEUP_TIMER0_INTERRUPT_AT_ONCE();
            delay_1ms(NOTE_BASE_SPEED * music_tbl[music_tbl_cursor+1]); //此delay期间播放当前音符(即按照当前音符的频率向蜂鸣器发送振动信号)
            CLOSE_TIMER0();
            delay_1ms(5); //delay 5ms作为两个音符之间的短暂停顿,此delay期间无声(通过关闭定时器不产生蜂鸣器振动信号实现)
            OPEN_TIMER0();
            music_tbl_cursor += MUSIC_TBL_ITEM_LEN; //visit next music note
        } else {
            CLOSE_TIMER0();
            break;
        }
    }
    while(1);
}

void Timer0_Routine() interrupt 1
{
    if (0 == note_freq_tbl[music_tbl[music_tbl_cursor]]) return;

    SET_16BIT_TIMER_COUNTER(note_freq_tbl[music_tbl[music_tbl_cursor]]);
    BUZZER = !BUZZER; //向蜂鸣器发出振动信号,且每次信号翻转,即010101...
}
