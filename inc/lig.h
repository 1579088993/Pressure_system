#ifndef __LIG_H
#define __LIG_H



#define LIG_IIC_SCL PBout(9)
#define LIG_IIC_SDA PBout(7)
#define LIG_INT PBout(8) 
#define READ_LIG_SDA PBin(7)

//#include "stm32f10x_conf.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    void Lig_Iic_Init(void);
    void Lig_Iic_Start(void);
    void Lig_Iic_Stop(void);
    u8 Lig_Iic_Wait_Ack(void);
    void Lig_Iic_Ack(void);
    void Lig_Iic_Nack(void);
    void Lig_Iic_Send_Byte(u8 txd);
    u8 Lig_Iic_Read_Byte(u8 ack);
    void Lig_Write(void);
    void Lig_Read(void);
    u8 Lig_Status(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
