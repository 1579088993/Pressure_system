#ifndef __AD1255_H
#define __AD1255_H

#define STATUS_ADDRESS  0X00H
#define MUX_ADDRESS     0X01H
#define ADCON_ADDRESS   0X02H
#define DRATE_ADDRESS   0X03H
#define IO_ADDRESS      0X04H
#define OFC0_ADDRESS    0X05H
#define OFC1_ADDRESS    0X06H
#define OFC2_ADDRESS    0X07H
#define FSC0_ADDRESS    0X08H
#define FSC1_ADDRESS    0X09H
#define FSC2_ADDRESS    0X0AH

#define WAKEUP          0X00   //0XFF也可
#define RDATA           0X01
#define RDATAC          0X03
#define SDATAC          0X0F
//#define RREG          0X1XH 0X0XH
//#define WREG          0X5XH 0X0XH
#define SELFCAL         0XF0
#define SELFOCAL        0XF1
#define SELFGCAL        0XF2
#define SYSOCAL         0XF3
#define SYSGCAL         0XF4
#define SYNC            0XFC
#define STANDBY         0XFD
#define RESET           0XFE



#define INT0 PBin(0)
// #define IIC_SCL PAout(11)
// #define IIC_SDA PAout(2)
// #define READ_SDA PAin(2)



#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    void Ad1255_Init(void);
    void ADS1255_Write_reg(u8 ADS1255_address, u8 ADS1255_data);
    u8 ADS1255_read_reg(u8 ADS1255_address);
    u32 ADS1255_read_date(void);
    void ADS1255_WAKEUP(void);
    void ADS1255_SELFCAL(void);
    void ADS1255_SYNC(void);
    void ADS1255_RESET(void);
    void ADS1255_reg_Init(void);
    
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
