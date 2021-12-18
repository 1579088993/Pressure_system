#include "stm32f10x_conf.h"
/**
 * 说明：这里传感器初始化就是IIC的初始化
 */
void Pre_Init(void)
{
    Iic_Init();
}

u8 Pre_Check(void)
{
    u8 flag = 0;
    Iic_Start();
    Iic_Send_Byte(0x81);
    while (Iic_Wait_Ack())
    {
        flag = Iic_Read_Byte(1);
        while (flag != 0x40)      //如果返回不是0x40 ,返回1
            return 1;
    }
    delay_ms(8);
    return 0;
}

void Pre_Write(void)
{
    Iic_Start();
    Iic_Send_Byte(0x80);
    while (Iic_Wait_Ack());
    Iic_Send_Byte(0xAC);
    while(Iic_Wait_Ack());
    Iic_Stop(); 
}

void Pre_Read(void)
{
    u8 state = 0;
    Iic_Start();
    Iic_Send_Byte(0x81);
    while(Iic_Wait_Ack());
    state = Iic_Read_Byte(1);
    if(state == 0x40){
        PSB[0] = Iic_Read_Byte(1);
        PSB[1] = Iic_Read_Byte(0);
        Iic_Stop();
    }
    Iic_Stop();
}

float Pre_Cal(void)
{
    float pressure = 0.0;
    pressure = (float)(PSB[0]*256 + PSB[1]-16384)*3/32768;
    return pressure;
}