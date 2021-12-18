#include "stm32f10x_conf.h"
/**
 * 延时函数
 */
static u8 fac_us = 0;
static u16 fac_ms = 0;

void delay_init(void)
{

    SysTick->LOAD = 72000-1; 
    SysTick->CTRL &= (0<<2)|(0<<1)|(0<<0); //使用外部参考时钟,不产生异常。失能时钟
    fac_us = 72/8;
    fac_ms = fac_us*1000;
}
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us;
    SysTick->VAL = 0;
    SysTick->CTRL = 1;//开始计数
    do{
        temp = SysTick->CTRL;
    }
    while((temp&0x01)&&!(temp&(1<<16)));//等待时间到
    SysTick->CTRL = 0x00;//关闭计数器
    SysTick->VAL = 0x00;//清空计数器
}

void delay_ms(u32 nms)
{
    u32 temp;
    SysTick->LOAD = nms * fac_ms;
    SysTick->VAL = 0x00;
    SysTick->CTRL = 0x01; //开始计数
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到
    SysTick->CTRL = 0x00;                           //关闭计数器
    SysTick->VAL = 0x00;                            //清空计数器
}