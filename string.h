#ifndef __STRING_H__
#define __STRING_H__

#include "global.h"

#define STR3_LEN 4
#define STR5_LEN 6
#define STR10_LEN 11
typedef char (*str3)[STR3_LEN]; 
typedef char (*str5)[STR5_LEN]; 
typedef char (*str10)[STR10_LEN]; 
#define INIT_STR3(variable) char variable[STR3_LEN]
#define INIT_STR5(variable) char variable[STR5_LEN]
#define INIT_STR10(variable) char variable[STR10_LEN]

extern uint8_t strlen(char *str);
extern void reverse_string(char *str, uint8_t len);
extern void convert_uint8_to_str(uint8_t n, str3 s);
extern void convert_uint16_to_str(uint16_t n, str5 s);
extern void convert_uint32_to_str(uint32_t n, str10 s);
extern char* concate_two_strs(char *str1, char *str2, char *dst);

#endif