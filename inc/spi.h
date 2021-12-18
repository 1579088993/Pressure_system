#ifndef __SPI_H
#define __SPI_H

//#include "stm32f10x_conf.h"

#define speed_2   ((uint16_t)0x0000)
#define speed_4   ((uint16_t)0x0008)
#define speed_8   ((uint16_t)0x0010)
#define speed_16  ((uint16_t)0x0018)
#define speed_32  ((uint16_t)0x0020)
#define speed_64  ((uint16_t)0x0028)
#define speed_128 ((uint16_t)0x0030)
#define speed_256 ((uint16_t)0x0038)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    void SPI2_Init(void);
    u8 SPI2_ReadWriteByte(u8 Txd);
    void SPI2_SetSpeed(u8 speed);
    void SPI1_Init(void);
    u8 SPI1_ReadWriteByte(u8 Txd);
    void SPI1_SetSpeed(u8 speed);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
