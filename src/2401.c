#include "stm32f10x_conf.h"
/**
 * 2401配置文件
 * IRQ   --->PB10
 * CE    --->PB11
 * CSN   --->PB12
 * SCK   --->PB13
 * MISO  --->PB14
 * MOSI  --->PB15
 */
const u8 TX_ADDRESS[TX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH] = {0x34, 0x43, 0x10, 0x10, 0x01};

void Nrf2401_Init(void)
{   
    RCC->APB2ENR |= 1<<3;//使能GPIOB
    GPIOB->CRH &= 0XFFF000FF;//清空GPIOB10.11.12的引脚配置
    GPIOB->CRH |= 0X00033000;//11.12配置成推挽输出  CE,CSN
    GPIOB->CRH |= 0X00000800; //IRQ配置成上拉输入
    GPIOB->ODR |= 7<<10;//全部初始输出高电平

    SPI2_Init();
    SPI2->CR1 &= ~(1 << 6); //SPI设备失能
    SPI2->CR1 &= ~(1 << 1); //空闲模式下SCK为0 CPOL=0
    SPI2->CR1 &= ~(1 << 0); //数据采样从第1个时间边沿开始,CPHA=0
    SPI2->CR1 |= 1 << 6;    //SPI设备使能
    GPIOB->ODR &= 0 << 11; //CE输出0，使能2401
    GPIOB->ODR |= 1<<12;   //CSN输出1，取消片选
}

//检测24L01是否存在
//返回值:0，成功;1，失败
u8 NRF24L01_Check(void)
{
    u8 buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
    u8 i;
    SPI2_SetSpeed(speed_4);                              //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
    NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, buf, 5); //写入5个字节的地址.
    NRF24L01_Read_Buf(TX_ADDR, buf, 5);                  //读出写入的地址
    for (i = 0; i < 5; i++)
        if (buf[i] != 0XA5)
            break;
    if (i != 5)
        return 1; //检测24L01错误
    return 0;     //检测到24L01
}
//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
u8 NRF24L01_Write_Reg(u8 reg, u8 value)
{
    u8 status;
    NRF24L01_CSN = 0;                 //使能SPI传输
    status = SPI2_ReadWriteByte(reg); //发送寄存器号
    SPI2_ReadWriteByte(value);        //写入寄存器的值
    NRF24L01_CSN = 1;                 //禁止SPI传输
    return (status);                  //返回状态值
}
//读取SPI寄存器值
//reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
    u8 reg_val;
    NRF24L01_CSN = 0;                   //使能SPI传输
    SPI2_ReadWriteByte(reg);           //发送寄存器号
    reg_val = SPI2_ReadWriteByte(0XFF); //读取寄存器内容
    NRF24L01_CSN = 1;                   //禁止SPI传输
    return (reg_val);                   //返回状态值
}
//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
    u8 status, u8_ctr;
    NRF24L01_CSN = 0;                 //使能SPI传输
    status = SPI2_ReadWriteByte(reg); //发送寄存器值(位置),并读取状态值
    for (u8_ctr = 0; u8_ctr < len + 1; u8_ctr++)
        pBuf[u8_ctr] = SPI2_ReadWriteByte(0XFF); //读出数据
    NRF24L01_CSN = 1;                            //关闭SPI传输
    return status;                               //返回读到的状态值
}
//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
    u8 status, u8_ctr;
    NRF24L01_CSN = 0;                 //使能SPI传输
    status = SPI2_ReadWriteByte(reg); //发送寄存器值(位置),并读取状态值
    for (u8_ctr = 0; u8_ctr < len; u8_ctr++)
        SPI2_ReadWriteByte(*pBuf++); //写入数据
    NRF24L01_CSN = 1;                //关闭SPI传输
    return status;                   //返回读到的状态值
}
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
    u8 sta;
    SPI2_SetSpeed(speed_4); //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
    NRF24L01_CE = 0;
    NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); //写数据到TX BUF  5个字节
    NRF24L01_CE = 1;                                        //启动发送
    while (NRF24L01_IRQ != 0)
        ;                                            //等待发送完成
    sta = NRF24L01_Read_Reg(STATUS);                 //读取状态寄存器的值
    NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta); //清除TX_DS或MAX_RT中断标志
    if (sta & MAX_TX)                                //达到最大重发次数
    {
        NRF24L01_Write_Reg(FLUSH_TX, 0xff); //清除TX FIFO寄存器
        return MAX_TX;
    }
    if (sta & TX_OK) //发送完成
    {
        return TX_OK;
    }
    return 0xff; //其他原因发送失败
}
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
    u8 sta;
    SPI2_SetSpeed(speed_4);          //spi速度为9Mhz（24L01的最大SPI时钟为10Mhz）
    sta = NRF24L01_Read_Reg(STATUS);                 //读取状态寄存器的值
    NRF24L01_Write_Reg(NRF_WRITE_REG + STATUS, sta); //清除TX_DS或MAX_RT中断标志
    if (sta & RX_OK)                                 //接收到数据
    {
        NRF24L01_Read_Buf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH); //读取数据
        NRF24L01_Write_Reg(FLUSH_RX, 0xff);                    //清除RX FIFO寄存器
        return 0;
    }
    return 1; //没收到任何数据
}
//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了
void NRF24L01_RX_Mode(void)
{
    NRF24L01_CE = 0;
    NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (u8 *)RX_ADDRESS, RX_ADR_WIDTH); //写RX节点地址

    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);              //使能通道0的自动应答
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);          //使能通道0的接收地址
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);                //设置RF通信频率
    NRF24L01_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //选择通道0的有效数据宽度
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);           //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
    NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);             //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
    NRF24L01_CE = 1;                                              //CE为高,进入接收模式
}
//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了
//CE为高大于10us,则启动发送.
void NRF24L01_TX_Mode(void)
{
    NRF24L01_CE = 0;
    NRF24L01_Write_Buf(NRF_WRITE_REG + TX_ADDR, (u8 *)TX_ADDRESS, TX_ADR_WIDTH);    //写TX节点地址
    NRF24L01_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, (u8 *)RX_ADDRESS, RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK

    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);      //使能通道0的自动应答    !!!!   01
    NRF24L01_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  //使能通道0的接收地址     !!!!   01
    NRF24L01_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a); //设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次   1a
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, 40);        //设置RF通道为40
    NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);   //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
    NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);     //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
    NRF24L01_CE = 1;                                      //CE为高,10us后启动发送
}