#include "stm32f10x_conf.h"
/**
 *USART配置文件 
 */
#if EN_USART1_RX //如果使能了接收
u8 USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0; //接收状态标记

void USART1_IRQHandler(void) //串口1中断服务程序
{
    u8 res;
    if (USART1->SR & (1 << 5)) //接收到数据
    {
        res = USART1->DR;
        if ((USART_RX_STA & 0x8000) == 0) //接收未完成
        {
            if (USART_RX_STA & 0x4000) //接收到了0x0d
            {
                if (res != 0x0a)
                    USART_RX_STA = 0; //接收错误,重新开始
                else
                    USART_RX_STA |= 0x8000; //接收完成了
            }
            else //还没收到0X0D
            {
                if (res == 0x0d)
                    USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = res;
                    USART_RX_STA++;
                    if (USART_RX_STA > (USART_REC_LEN - 1))
                        USART_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
        }
    }
}
#endif

void uart_init(u32 pclk2, u32 bound)
{
    float temp;
    u16 mantissa;
    u16 fraction;
    temp = (float)(pclk2 * 1000000) / (bound * 16); //得到USARTDIV
    mantissa = temp;                                //得到整数部分
    fraction = (temp - mantissa) * 16;              //得到小数部分
    mantissa <<= 4;
    mantissa += fraction;
    RCC->APB2ENR |= 1 << 2;      //使能GPIOA
    RCC->APB2ENR |= 1 << 14;     //使能串口时钟
    GPIOA->CRH &= 0XFFFFF00F;    //清零PA9.10的管脚配置，
    GPIOA->CRH |= 0X000000B0;    //配置成输出模式，推挽输出，最大速率50MHz
    GPIOA->CRH |= 0X00000800;    //上拉输入
    RCC->APB2RSTR |= 1 << 14;    //复位串口1
    RCC->APB2RSTR &= ~(1 << 14); //停止复位
    USART1->BRR = mantissa;      // 波特率设置
    USART1->CR1 |= 0X200C;       //1位停止,无校验位.
#if EN_USART1_RX                 //如果使能了接收
    //使能接收中断
    USART1->CR1 |= 1 << 5;              //接收缓冲区非空中断使能
    MY_NVIC_Init(3, 3, USART1_IRQn, 2); //组2，最低优先级
#endif
}
void UART_Send(uint16_t Data)
{
    while ((USART1->SR & 0X40) ==0);
    USART1->DR = Data ;
    
}


