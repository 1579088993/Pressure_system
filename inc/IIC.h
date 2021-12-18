#ifndef __IIC_H
#define __IIC_H


#define IIC_SCL     PAout(11)
#define IIC_SDA     PAout(2)
#define READ_SDA    PAin(2)

//#include "stm32f10x_conf.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



void Iic_Init(void);
void Iic_Start(void);
void Iic_Stop(void);
u8 Iic_Wait_Ack(void);
void Iic_Ack(void);
void Iic_Nack(void);
void Iic_Send_Byte(u8 txd);
u8 Iic_Read_Byte(u8 rxd);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif 
