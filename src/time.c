#include "stm32f10x_conf.h"

void TIM3_Int_Init(u16 arr, u16 psc)
{
    RCC->APB1ENR |= 1 << 1;           //TIM3时钟使能
    TIM3->ARR = arr;                  //设定计数器自动重装值//刚好1ms
    TIM3->PSC = psc;                  //预分频器7200,得到10Khz的计数时钟
    TIM3->DIER |= 1 << 0;             //允许更新中断
    TIM3->CR1 |= 0x01;                //使能定时器3
    MY_NVIC_Init(1, 3, TIM3_IRQn, 2); //抢占1，子优先级3，组2
}

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & 0X0001) //溢出中断
    {
        while(1)
            ;
    }
    TIM3->SR &= ~(1 << 0); //清除中断标志位
}

