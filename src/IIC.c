#include "stm32f10x_conf.h"
/**
 * 压力传感器的IIC
 * SCL  --->PA11
 * SDA  --->PA2
 */
void Iic_Init(void)
{
    RCC->APB2ENR|=1<<2;            //使能GPIOA 
    GPIOA->CRL &= 0XFFFFF0FF;   //清零PA2的管脚配置，使其处于模拟输入状态
    GPIOA->CRH &= 0XFFFF0FFF;   //清零PA11的管脚配置，使其处于模拟输入状态
    //GPIOB->CRL |= 0X33000000;   //配置成输出模式，通用推挽输出，最大速率50MHz..
    GPIOA->CRL |= 0X00000700;     //配置成开漏输出可以避免来回切换
    GPIOA->CRH |= 0X00007000;     //配置成开漏输出可以避免来回切换
    GPIOA->ODR |= 1<<2;  //PA2输出高
    GPIOA->ODR |= 1<<11; //PA11输出高
}

void Iic_Start(void)
{
    //SDA_OUT();
    IIC_SCL = 1;
    IIC_SDA = 1;
    delay_us(5);
    IIC_SDA = 0;
    delay_us(4);
    IIC_SCL = 0;
}

void Iic_Stop(void)
{
    //SDA_OUT();
    IIC_SCL = 0;
    IIC_SDA = 0;
    delay_us(4);
    IIC_SCL = 1;
    delay_us(2);
    IIC_SDA = 1;
    delay_us(5);
    IIC_SCL = 0;
}

u8 Iic_Wait_Ack(void)
{
    u8 iicTime = 0;
    //SDA_IN();
    IIC_SDA = 1;
    delay_us(1);
    IIC_SCL = 1;
    delay_us(1);
    while (READ_SDA)
    {
        iicTime++;
        if(iicTime>250){
            Iic_Stop();
            return 1;
        }
    }
    IIC_SCL = 0;
    return 0;    
}

void Iic_Ack(void)
{
    IIC_SCL = 0;
    //SDA_OUT();
    IIC_SDA = 0;
    delay_us(2);
    IIC_SCL = 1;
    delay_us(2);
    IIC_SCL = 0;
}

void Iic_Nack(void)
{
    IIC_SCL = 0;
    //SDA_OUT();
    IIC_SDA = 1;
    delay_us(2);
    IIC_SCL = 1;
    delay_us(2);
    IIC_SCL = 0;
}

void Iic_Send_Byte(u8 txd)
{
    u8 t = 0;
    //SDA_OUT();
    IIC_SCL = 0;
    for (t = 0; t < 8; t++)
    {
        IIC_SDA = (txd&0x80)>>7;
        txd<<=1;
        delay_us(2);
        IIC_SCL = 1;
        delay_us(2);
        IIC_SCL = 0;
        delay_us(2);
    }  
}

u8 Iic_Read_Byte(u8 ack)
{
    u8 i = 0;
    u8 receive = 0;
    //SDA_IN();
    for (i = 0; i < 8; i++)
    {
        IIC_SCL = 0;
        delay_us(2);
        IIC_SCL = 1;
        receive<<=1;
        if(READ_SDA)
            receive++;
        delay_us(1);    
    }
    if(ack)
        Iic_Ack();
    else
        Iic_Nack();
    return receive;
}
