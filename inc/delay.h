/*
*延时函数
*
*/

#ifndef __DELAY_H
#define __DELAY_H

//#include "stm32f10x_conf.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void delay_init(void);
void delay_us(u32 nus);
void delay_ms(u32 nms);

#ifdef __cplusplus
} 
#endif /* __cplusplus */

#endif 
