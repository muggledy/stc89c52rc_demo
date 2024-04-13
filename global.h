#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//#define SHOW_WITH_LCD

#ifdef SHOW_WITH_LCD
#include "LCD1602.h"
#endif

#define NULL 0
#define true 1
#define false 0
typedef unsigned char bool_t;

typedef unsigned char uint8_t; //0 to 255
typedef signed char int8_t; //-128 to +127
typedef unsigned short uint16_t; //0 to 65535, type int also 16bits
typedef signed short int16_t; //-32768 to +32767
typedef unsigned long uint32_t; //0 to 4294967295
typedef signed long int32_t; //-2147483648 to +2147483647
typedef float float32_t; //32bits, also is double

#define SET_FLAG(obj, field, flag) (obj)->field |= flag
#define CLR_FLAG(obj, field, flag) (obj)->field &= (~flag)
#define TST_FLAG(obj, field, flag) ((obj)->field & flag)

#endif