#include "stm32f10x_conf.h"

u8 PSB[2] ;

int main()
{
    int i,status ;
    Stm32_Clock_Init(9);
    delay_init();
    Nrf2401_Init(); //2401初始化
    uart_init(72,115200);
    while (NRF24L01_Check());
    Tem_Iic_Init();//温度传感器初始化
    Tem_Write();
    Lig_Iic_Init(); //光传感器初始化
    Lig_Write();  //光传感器写配置
    status = Lig_Status();
    //TIM3_Int_Init(4999, 7199); //500ms进一次中断

    while (i > 0)
    {
        i++;
        //delay_ms(5000);
    }
}

