#ifndef __USART_H
#define __USART_H

//#include "stm32f10x_conf.h"

#define USART_REC_LEN 200              //定义最大接收字节数 200
#define EN_USART1_RX 1                 //使能（1）/禁止（0）串口1接收
extern u8 USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;               //接收状态标记
//如果想串口中断接收，请不要注释以下宏定义
#define USART_IT_PE ((uint16_t)0x0028)
#define USART_IT_TXE ((uint16_t)0x0727)
#define USART_IT_TC ((uint16_t)0x0626)
#define USART_IT_RXNE ((uint16_t)0x0525)
#define USART_IT_IDLE ((uint16_t)0x0424)
#define USART_IT_LBD ((uint16_t)0x0846)
#define USART_IT_CTS ((uint16_t)0x096A)
#define USART_IT_ERR ((uint16_t)0x0060)
#define USART_IT_ORE ((uint16_t)0x0360)
#define USART_IT_NE ((uint16_t)0x0260)
#define USART_IT_FE ((uint16_t)0x0160)

#define USART_FLAG_CTS ((uint16_t)0x0200)
#define USART_FLAG_LBD ((uint16_t)0x0100)
#define USART_FLAG_TXE ((uint16_t)0x0080)
#define USART_FLAG_TC ((uint16_t)0x0040)
#define USART_FLAG_RXNE ((uint16_t)0x0020)
#define USART_FLAG_IDLE ((uint16_t)0x0010)
#define USART_FLAG_ORE ((uint16_t)0x0008)
#define USART_FLAG_NE ((uint16_t)0x0004)
#define USART_FLAG_FE ((uint16_t)0x0002)
#define USART_FLAG_PE ((uint16_t)0x0001)

#define IT_Mask ((uint16_t)0x001F) /*!< USART Interrupt Mask */
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    void uart_init(u32 pclk2,u32 bound);
    void UART_Send(uint16_t Data);
    ITStatus USART_GetITStatus(USART_TypeDef *USARTx, uint16_t USART_IT);
    FlagStatus USART_GetFlagStatus(USART_TypeDef *USARTx, uint16_t USART_FLAG);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
