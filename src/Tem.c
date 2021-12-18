#include "stm32f10x_conf.h"

/**
 *温湿度传感器的初始化以及IIC配置 
 * 接口
 *      SCL:PB5
 *      SDA:PB6
 * IIC从机地址位1000000    .7位
 */
u16 tem_data[2] ;
u16 hum_data[2] ;
u16 Tem_Data = 0;
u16 Hum_Data = 0;

void Tem_Write(void)
{
    Tem_Iic_Start();
    Tem_Iic_Send_Byte(0x80);   //从机地址1000000,最后一位为0：Write
    while(Tem_Iic_Wait_Ack());  //返回1说明未等到ACK信号，就死循环
    Tem_Iic_Send_Byte(0x02);   //0x02为配置寄存器，
    while(Tem_Iic_Wait_Ack());  //返回1说明未等到ACK信号，就死循环
    Tem_Iic_Send_Byte(0x15);   //写入高8位数据
    while (Tem_Iic_Wait_Ack()); //返回1说明未等到ACK信号，就死循环
    Tem_Iic_Send_Byte(0x00);   //写入低八位数据
    while (Tem_Iic_Wait_Ack()); //返回1说明未等到ACK信号，就死循环
    Tem_Iic_Stop();
}

void Tem_Read(void)
{
    Tem_Iic_Start();
    Tem_Iic_Send_Byte(0x80); //从机地址1000000,最后一位为0：WRITE
    while (Tem_Iic_Wait_Ack()); //返回1说明未等到ACK信号，就死循环

    Tem_Iic_Send_Byte(0x00);//寄存器地址位00，说明读温度
    while (Tem_Iic_Wait_Ack()); //返回1说明未等到ACK信号，就死循环

    Tem_Iic_Start();
    Tem_Iic_Send_Byte(0x81); //从机地址1000000,最后一位为1：READ
    while (Tem_Iic_Wait_Ack()); //返回1说明未等到ACK信号，就死循环

    tem_data[0] = Tem_Iic_Read_Byte(1);
    tem_data[1] = Tem_Iic_Read_Byte(0);
    Tem_Iic_Stop();

    Tem_Iic_Start();
    Tem_Iic_Send_Byte(0x80); //从机地址1000000,最后一位为0：READ
    while (Tem_Iic_Wait_Ack()); //返回1说明未等到ACK信号，就死循环

    Tem_Iic_Send_Byte(0x01); //寄存器地址位01，说明读湿度
    while (Tem_Iic_Wait_Ack()); //返回1说明未等到ACK信号，就死循环

    Tem_Iic_Start();
    Tem_Iic_Send_Byte(0x81); //从机地址1000000,最后一位为1：READ
    while (Tem_Iic_Wait_Ack()); //返回1说明未等到ACK信号，就死循环
    
    hum_data[0] = Tem_Iic_Read_Byte(1);
    hum_data[1] = Tem_Iic_Read_Byte(0);
    Tem_Iic_Stop();
}
/*
*计算温湿度公式
*
*/
void Data_Cal(void)
{
    Tem_Data = (tem_data[0] << 8) + tem_data[1];
    Hum_Data = (hum_data[0] << 8) + hum_data[1];
    Tem_Data = (Tem_Data / 2 ^ 16) * 165 - 40;
    Hum_Data = (Hum_Data / 2 ^ 16) * 1;
}


void Tem_Iic_Init(void)
{
    RCC->APB2ENR |= 1 << 3;   //使能GPIOB
    GPIOB->CRL &= 0XF00FFFFF; //清零PB5.6的管脚配置，使其处于模拟输入状态
    //GPIOB->CRL |= 0X03300000;   //配置成输出模式，通用推挽输出，最大速率50MHz..
    GPIOB->CRL |= 0X03300000; //配置成开漏输出可以避免来回切换
    GPIOB->ODR |= 3 << 5;     //输出高
}

void Tem_Iic_Start(void)
{
    TEM_IIC_SCL = 1;
    TEM_IIC_SDA = 1;
    delay_us(2);
    TEM_IIC_SDA = 0;
    delay_us(1);
    TEM_IIC_SCL = 0;
    delay_us(2);
}

void Tem_Iic_Stop(void)
{
    TEM_IIC_SCL = 0;
    TEM_IIC_SDA = 0;
    delay_us(2);
    TEM_IIC_SCL = 1;
    delay_us(1);
    TEM_IIC_SDA = 1;
    delay_us(2);
    TEM_IIC_SCL = 0;
}

u8 Tem_Iic_Wait_Ack(void)
{
    u8 tem_iicTime = 0;
    TEM_IIC_SDA = 1;
    TEM_IIC_SCL = 1;
    delay_us(1);
    while (READ_TEM_SDA)
    {
        tem_iicTime++;
        if(tem_iicTime>250)
        {
            Tem_Iic_Stop();
            return 1;
        }
    }
    TEM_IIC_SCL = 0;
    return 0; 
}

void Tem_Iic_Ack(void)
{
    TEM_IIC_SCL = 0;
    TEM_IIC_SDA = 0;
    delay_us(2);
    TEM_IIC_SCL = 1;
    delay_us(2);
    TEM_IIC_SCL = 0;
}

void Tem_Iic_Nack(void)
{
    TEM_IIC_SCL = 0;
    TEM_IIC_SDA = 1;
    delay_us(2);
    TEM_IIC_SCL = 1;
    delay_us(2);
    TEM_IIC_SCL = 0;
}

void Tem_Iic_Send_Byte(u8 txd)
{
    u8 t = 0;
    TEM_IIC_SCL = 0;
    for (t = 0; t < 8; t++)
    {
        TEM_IIC_SDA = (txd&0x80)>>7;
        txd<<=1;
        TEM_IIC_SCL = 1;
        delay_us(2);
        TEM_IIC_SCL = 0;
        delay_us(1);
    } 
}

u8 Tem_Iic_Read_Byte(u8 ack)
{
    u8 i = 0;
    u8 recieve = 0;
    for ( i = 0; i < 8; i++)
    {
        TEM_IIC_SCL = 0;
        delay_us(2);
        TEM_IIC_SCL = 1;
        recieve<<=1;
        if(READ_TEM_SDA)
            recieve++;
        delay_us(2);
    }
    if(ack)
        Tem_Iic_Ack();
    else 
        Tem_Iic_Nack();
    return recieve;   
}
