#include "stm32f10x_conf.h"
/**
 *SPI配置文件 
 */
void SPI2_Init(void)
{
    RCC->APB2ENR |= 1<<3;  //使能GPIOB
    RCC->APB1ENR |= 1<<14;//使能SPI2时钟
    GPIOB->CRH &= 0X000FFFFF; //初始化GPIOB13.14.15
    GPIOB->CRH |= 0XBBB00000;//配置成复用推挽输出，速度50MHz
    GPIOB->ODR |= 0X7<<13;//配置成上拉
    SPI2->CR1 |= 0<<10;//全双工
    SPI2->CR1 |= 1<<9;//软件NSS
    SPI2->CR1 |= 1<<8;
    SPI2->CR1 |= 1<<2;//配置成主机
    SPI2->CR1 |= 0<<11;//使用8位数据帧
    SPI2->CR1 |= 1<<1;//空闲状态下，SCK为高电平
    SPI2->CR1 |= 1<<0;//数据采样从第二个时钟沿开始。
    SPI2->CR1 |= 3<<3;//Fpclk/256   36M/256
    SPI2->CR1 |= 0<<7;//MSBfirst
    SPI2->CR1 |= 1<<6;//SPI使能
    
    SPI2_ReadWriteByte(0XFF);
}

void SPI1_Init(void)
{
    RCC->APB2ENR |= 1<<2;  //使能GPIOA
    RCC->APB2ENR |= 1<<12;//使能SPI1时钟
    GPIOA->CRL &= 0X000FFFFF; //初始化GPIOA5.6.7
    GPIOA->CRL |= 0XBBB00000;//配置成复用推挽输出，速度50MHz
    GPIOA->ODR |= 0X7<<5;//配置成上拉

    SPI1->CR1 |= 0<<10;//全双工
    SPI1->CR1 |= 1<<9;//软件NSS
    SPI1->CR1 |= 1<<8;
    SPI1->CR1 |= 1<<2;//配置成主机
    SPI1->CR1 |= 0<<11;//使用8位数据帧
    SPI1->CR1 |= 1<<1;//空闲状态下，SCK为高电平
    SPI1->CR1 |= 1<<0;//数据采样从第二个时钟沿开始。
    SPI1->CR1 |= 3<<3;//Fpclk/256   36M/256
    SPI1->CR1 |= 0<<7;//MSBfirst
    SPI1->CR1 |= 1<<6;//SPI使能
    
    SPI1_ReadWriteByte(0XFF);
}

void SPI2_SetSpeed(u8 speed)
{
    speed &= 0X07;    //限制范围
    SPI2->CR1 &= 0XFFC7;//失能SPI
    SPI2->CR1 |= speed; //设置SPI2速度
    SPI2->CR1 |= 1 << 6;//使能SPI
}

void SPI1_SetSpeed(u8 speed)
{
    speed &= 0X07;       //限制范围
    SPI1->CR1 &= 0XFFC7; //失能SPI
    SPI1->CR1 |= speed;  //设置SPI2速度
    SPI1->CR1 |= 1 << 6; //使能SPI
}


u8 SPI2_ReadWriteByte(u8 Txd)
{
    u16 retry = 0;
    while ((SPI2->SR&1<<1) == 0) //SR的第二位为0表示非空。
    {
        retry++;
        if(retry>=0XFFFE)
            retry = 0;//超时退出
    }
    SPI2->DR = Txd;
    retry = 0;
    while ((SPI2->SR&1<<0)==0)   //SR的第一位为0表示接受缓冲为空,即接受完
    {
        retry++;
        if (retry >= 0XFFFE)
            retry = 0; //超时退出
    }
    return SPI2->DR;//返回接收到的数据   
}

u8 SPI1_ReadWriteByte(u8 Txd)
{
    u16 retry = 0;
    while ((SPI1->SR & 1 << 1) == 0) //SR的第二位为0表示非空。
    {
        retry++;
        if (retry >= 0XFFFE)
            retry = 0; //超时退出
    }
    SPI1->DR = Txd;
    retry = 0;
    while ((SPI1->SR & 1 << 0) == 0) //SR的第一位为0表示接受缓冲为空,即接受完
    {
        retry++;
        if (retry >= 0XFFFE)
            retry = 0; //超时退出
    }
    return SPI1->DR; //返回接收到的数据
}