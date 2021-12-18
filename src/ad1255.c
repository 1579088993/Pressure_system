#include "stm32f10x_conf.h"

/**
 * DRDY --->PB0
 * CS   --->PB1
 * SCK  --->PA5
 * MISO --->PA6
 * MOSI --->PA7
 * 
 */
void Ad1255_Init(void)
{
    RCC->APB2ENR |= 1 << 3;   //使能GPIOB
    
    GPIOB->CRL &= 0XFFFFFF00; //清空GPIOB1.0的引脚配置
    GPIOB->CRL |= 0X00000030; //3.配置成推挽输出  ,CS
    GPIOB->CRL |= 0X00000004; //DRDY配置成浮空输入
    GPIOB->ODR |= 3 << 0;    //全部初始输出高电平

    SPI1_Init();
    SPI1->CR1 &= ~(1 << 6); //SPI1设备失能
    SPI1->CR1 &= ~(1 << 1); //空闲模式下SCK为0 CPOL=0
    SPI1->CR1 |= 1 << 6;    //SPI设备使能

    GPIOB->ODR |= 1 << 1; //CS_1
    GPIOA->ODR |= 0 << 5; //SCK_0.
    GPIOA->ODR |= 1 << 6; //DI_1
    /**
     * CS_1
     * SCK_0
     * DI_1
     */
}

/**
 *ADS1255根据要求写寄存器和命令字 ，注意，这里只能写一个
 */
void ADS1255_Write_reg(u8 ADS1255_address,u8 ADS1255_data)
{ 
    SPI1_ReadWriteByte(ADS1255_address);  //写数据，前四位地址是0101 xxxx
    SPI1_ReadWriteByte(0x00);   //写一个数据,数据数-1
    SPI1_ReadWriteByte(ADS1255_data);
    delay_us(2);
}
/**
 * 读寄存器的数据
 * 
 */
u8 ADS1255_read_reg(u8 ADS1255_address)
{
    uint8_t reg_data;
    SPI1_ReadWriteByte(ADS1255_address); //读寄存器，前四位 0001 xxxx
    SPI1_ReadWriteByte(0x00); //读取一个字节  数据数-1
    delay_us(10);             //此处延时只需大于t6=7us即可，无需太长
    reg_data = SPI1_ReadWriteByte(0x05);  //此处写什么无所谓
    return reg_data;
}
/**
 * 读一个数据，当DRDY = 0;
 */
u32 ADS1255_read_date(void)
{
    uint32_t ADZ;
    uint8_t data1, data2, data3;
    //while(INT0==0);
    while (INT0 == 1);
    SPI1_ReadWriteByte(0x01);
    delay_us(10);
    data1 = SPI1_ReadWriteByte(0x02);  //这里写入什么都无所谓
    data2 = SPI1_ReadWriteByte(0x02);
    data3 = SPI1_ReadWriteByte(0x02);
    delay_us(1);
    ADZ = data1 * 65536 + data2 * 256 + data3;
    return ADZ;
}

/**
 * 唤醒和同步
 */
void ADS1255_WAKEUP(void)
{
    while(INT0 ==1);
    SPI1_ReadWriteByte(WAKEUP); //
}
/**
 * 偏移自校准，也可以用STATUS寄存器里的ACAL来自校准
 */
void ADS1255_SELFCAL(void)
{
    while(INT0 ==1);
    SPI1_ReadWriteByte(SELFCAL);
}
/**
 * 同步命令
 * 紧接着用WAKE UP命令进行同步
 */
void ADS1255_SYNC(void)
{
    while(INT0 ==1);
    SPI1_ReadWriteByte(SYNC);
}
/**
 * 复位命令
 * 不管ACAL如何，都执行一个自校准
 */
void ADS1255_RESET(void)
{
    while(INT0 ==1);
    SPI1_ReadWriteByte(RESET);
}

/**
 * 对寄存器设置初始化
 */
void ADS1255_reg_Init(void)
{
    
    while (INT0 == 1);                          //DREY为1时，得等待。
    ADS1255_Write_reg(0x50, 0x04);              //使能自动校验
                                                // ADS1255_write_reg(0x50,0x00);   //失能自动校验,代替上句读取寄存器返回值将出错，为什么？？
    delay_us(2);
    while (INT0 == 1);
    ADS1255_Write_reg(0x51, 0x00); //  默认输入AIN0
    delay_us(2);
    while (INT0 == 1);
    ADS1255_Write_reg(0x52, 0x00); //ADCON设置Clock Out OFF，Sensor Detect OFF，PAG=1
    delay_us(2);
    while (INT0 == 1);
    ADS1255_Write_reg(0x53, 0xC0); //设置数据率即data rate，3.75K
    delay_us(2);
    while (INT0 == 1);
    ADS1255_Write_reg(0x54, 0x00); // 这个寄存器可以使用默认值
    delay_us(2);
}


// u8 ADS1255_Read_ID(void)
// {   
//     u8 id = 0; 
//     while (INT0 == 0); 
//     //id = 
// }

