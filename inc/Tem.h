#ifndef __TEM_H
#define __TEM_H



#define TEM_IIC_SCL PBout(6)
#define TEM_IIC_SDA PBout(5)
#define READ_TEM_SDA PBin(5)

//#include "stm32f10x_conf.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    void Tem_Iic_Init(void);
    void Tem_Iic_Start(void);
    void Tem_Iic_Stop(void);
    u8 Tem_Iic_Wait_Ack(void);
    void Tem_Iic_Ack(void);
    void Tem_Iic_Nack(void);
    void Tem_Iic_Send_Byte(u8 txd);
    u8 Tem_Iic_Read_Byte(u8 ack);
    void Tem_Write(void);
    void Tem_Read(void); void Data_Cal(void);
    void Data_Cal(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
