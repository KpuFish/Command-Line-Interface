/**
 * Module      : UTILL
 * File Name   : COMMON.c
 * Description : UTILL Collection
 * Who         : Deckster
 **/
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "common.h"
#include "stm32f4xx_hal.h"

//----------------------------------------
// Ascii to Hex Conversion
//----------------------------------------
int atoh(uint8_t *s)
{
	int step = 1, tmp = 0, str_len, i;

	str_len = strlen((char *)s)-1;

	for (i = str_len; i >= 0; i--) {
		if (s[i] <= 0x39) {
			tmp += ((s[i]-0x30) * step);
        } else if ((0x61<=s[i]) && (s[i]<=0x66)) {
			tmp += ((s[i]-0x57) * step);
        } else if ((0x41<=s[i]) && (s[i]<=0x46)) {
			tmp += ((s[i]-0x37) * step);
        } else {
			tmp = 0;
        }
		step *= 16;
	}
	return tmp;
}

//----------------------------------------
// Delay usec from the tick timer 1us
//----------------------------------------
extern TIM_HandleTypeDef htim1;
void Delay_us(uint32_t time)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);
   
   while (__HAL_TIM_GET_COUNTER(&htim1) < time)
      ;
}
