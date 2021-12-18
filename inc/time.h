#ifndef __TIME_H
#define __TIME_H

//#include "stm32f10x_conf.h"



#ifdef __cplusplus
extern "C"
{
#endif                                    /* __cplusplus */
    void TIM3_Int_Init(u16 arr, u16 psc);
    void TIM3_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
