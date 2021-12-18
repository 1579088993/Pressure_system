#include "stm32f10x_conf.h"
/**
 * 光敏传感器的IIC
 * SCL   --->PB9
 * SDA   --->PB7
 * INT   --->PB8
 */

u16 lig_data[4] ;
u16 lig_ch1_data = 0;
u16 lig_ch0_data = 0;


u8 Lig_Status(void)
{
    u8 lig_status = 0;

    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x52);
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Send_Byte(0x8C);//状态寄存器
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x53);
    while(Lig_Iic_Wait_Ack());
    
    lig_status = Lig_Iic_Read_Byte(0); //Bit7:0 ALS Data is Valid,1 ALS Data is Invalid; Bit6-4 :000 ALS measured data in Gain 1x;  Bit3: 0 interrupt signal INACTIVE ; Bit2 : 0 OLD data ,1 NEW data;  Bit1-0: Don't Care. 
    Lig_Iic_Stop();
    return lig_status;
}
void Lig_Write(void)
{
    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x52);
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Send_Byte(0x80);//控制寄存器
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Send_Byte(0x01); //增益为1
    while(Lig_Iic_Wait_Ack());
    Lig_Iic_Stop();

    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x52);
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Send_Byte(0x85);//测量时间寄存器
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Send_Byte(0x12); //int time = 200ms,Meas rate = 200ms;
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Stop();
    delay_ms(10);
}

void Lig_Read(void)
{
    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x52);
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Send_Byte(0x88);//CH1 low
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x53);
    while(Lig_Iic_Wait_Ack());
    lig_data[0] = Lig_Iic_Read_Byte(0);
    Lig_Iic_Stop();

    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x52);
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Send_Byte(0x89);//CH1 High
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x53);
    while(Lig_Iic_Wait_Ack());
    lig_data[1] = Lig_Iic_Read_Byte(0);
    Lig_Iic_Stop();

    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x52);
    while(Lig_Iic_Wait_Ack());
    Lig_Iic_Send_Byte(0x8A);//CH0 low
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x53);
    while(Lig_Iic_Wait_Ack());
    lig_data[2] = Lig_Iic_Read_Byte(0);
    Lig_Iic_Stop();

    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x52);
    while(Lig_Iic_Wait_Ack());
    Lig_Iic_Send_Byte(0x8B);//CH0 High
    while(Lig_Iic_Wait_Ack());

    Lig_Iic_Start();
    Lig_Iic_Send_Byte(0x53);
    while(Lig_Iic_Wait_Ack());
    lig_data[3] = Lig_Iic_Read_Byte(0);
    Lig_Iic_Stop();

    lig_ch1_data = (lig_data[1] << 8) | lig_data[0];
    lig_ch0_data = (lig_data[3] << 8) | lig_data[2];
    
}



void Lig_Iic_Init(void)
{
    RCC->APB2ENR |= 1 << 3;   //使能GPIOB
    GPIOB->CRL &= 0X0FFFFFFF; //清零PB7的管脚配置，使其处于模拟输入状态
    GPIOB->CRH &= 0XFFFFFF00; //清零PB8.9的管脚配置，使其处于模拟输入状态
    //GPIOB->CRL |= 0X03300000;   //配置成输出模式，通用推挽输出，最大速率50MHz..
    GPIOB->CRL |= 0X30000000; //配置成开漏输出可以避免来回切换
    GPIOB->CRH |= 0X00000033; //配置成开漏输出可以避免来回切换
    GPIOB->ODR |= 7 << 6;     //输出高
    delay_ms(100);
}

void Lig_Iic_Start(void)
{
    LIG_IIC_SCL = 1;
    LIG_IIC_SDA = 1;
    delay_us(5);
    LIG_IIC_SDA = 0;
    delay_us(4);
    LIG_IIC_SCL = 0;
}

void Lig_Iic_Stop(void)
{
    LIG_IIC_SCL = 0;
    LIG_IIC_SDA = 0;
    delay_us(4);
    LIG_IIC_SCL = 1;
    delay_us(2);
    LIG_IIC_SDA = 1;
    delay_us(5);
    LIG_IIC_SCL = 0;
}

u8 Lig_Iic_Wait_Ack(void)
{
    u8 lig_iicTime = 0;
    LIG_IIC_SDA = 1;
    delay_us(2);
    LIG_IIC_SCL = 1;
    delay_us(2);
    while (READ_LIG_SDA)
    {
        lig_iicTime++;
        if (lig_iicTime > 250)
        {
            Lig_Iic_Stop();
            return 1;
        }
    }
    LIG_IIC_SCL = 0;
    return 0;
}

void Lig_Iic_Ack(void)
{
    LIG_IIC_SCL = 0;
    LIG_IIC_SDA = 0;
    delay_us(2);
    LIG_IIC_SCL = 1;
    delay_us(2);
    LIG_IIC_SCL = 0;
}

void Lig_Iic_Nack(void)
{
    LIG_IIC_SCL = 0;
    LIG_IIC_SDA = 1;
    delay_us(2);
    LIG_IIC_SCL = 1;
    delay_us(2);
    LIG_IIC_SCL = 0;
}

void Lig_Iic_Send_Byte(u8 txd)
{
    u8 t = 0;
    LIG_IIC_SCL = 0;
    for (t = 0; t < 8; t++)
    {
        LIG_IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1;
        delay_us(2);
        LIG_IIC_SCL = 1;
        delay_us(2);
        LIG_IIC_SCL = 0;
        delay_us(2);
    }
}

u8 Lig_Iic_Read_Byte(u8 ack)
{
    u8 i = 0;
    u8 recieve = 0;
    for (i = 0; i < 8; i++)
    {
        LIG_IIC_SCL = 0;
        delay_us(2);
        LIG_IIC_SCL = 1;
        recieve <<= 1;
        if (READ_LIG_SDA)
            recieve++;
        delay_us(2);
    }
    if (ack)
        Lig_Iic_Ack();
    else
        Lig_Iic_Nack();
    return recieve;
}