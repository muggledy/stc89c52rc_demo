#include "string.h"

uint8_t strlen(char *str)
{
	uint8_t i = 0;
	while(0 != str[i]) {
		i++;
	}
	return i;
}

void reverse_string(char *str, uint8_t len)
{
	uint8_t i = 0;
	char t;
    if (0 == len) {
    	len = strlen(str);
	}
    for (; i < len / 2; i++) {
        t = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = t;
    }
}

#define CONVERT_UINT_TO_STR_BODY {\
	uint8_t idx = 0;\
	if(0 == n) {\
		(*s)[0] = '0';\
		(*s)[1] = 0;\
		return;\
	}\
	while(n) {\
		(*s)[idx++] = n%10 + '0';\
		n /= 10;\
	}\
	(*s)[idx] = 0;\
	reverse_string(*s, idx);\
}

void convert_uint8_to_str(uint8_t n, str3 s)
{
	CONVERT_UINT_TO_STR_BODY;
}

void convert_uint16_to_str(uint16_t n, str5 s)
{
	CONVERT_UINT_TO_STR_BODY;
}

void convert_uint32_to_str(uint32_t n, str10 s)
{
	CONVERT_UINT_TO_STR_BODY;
}

char* concate_two_strs(char *str1, char *str2, char *dst)
{
	char *src_ptr = str1;
	char *dst_ptr = dst;
	while(0 != *src_ptr) {
		*(dst_ptr++) = *(src_ptr++);
	}
	src_ptr = str2;
	while(0 != *src_ptr) {
		*(dst_ptr++) = *(src_ptr++);
	}
    *dst_ptr = 0;
	return dst;
}
