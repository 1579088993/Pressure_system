#ifndef __VOLTAGE_H
#define __VOLTAGE_H



//#include "stm32f10x_conf.h"

#define ADC_CH1     1//通道1(连接在PA1)
#define BEEP    PAout(3)


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
    void Adc_Init(void);                  //ADC通道初始化
    u16 Get_Adc(u8 ch);                   //获得某个通道值
    u16 Get_Adc_Average(u8 ch, u8 times); //得到某个通道10次采样的平均值

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
