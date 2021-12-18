外设模块：
    1：压力传感器：IIC；SCL->PA11.SDA->PA2                                             0.8
    2：温湿度传感器：IIC:SCL->PB5.SDA->PB6                                             0.8
    3：光敏传感器：IIC :SDA->PB7,SCL->PB9.INT->PB8                                     0.8     
    4：AD采集树径：SPI:SCK->PA5,MISO->PA6,MOSI->PA7,INT->PB0,IO->PB1                   0.5
    5：蓝牙：串口:TX->PA9,RX->PA10                                                     0.8
    6：NRF2401:SPI:SCK->PB13,MISO->PB14,MOSI->PB15,CSN->PB12,CE->PB11,IRQ->PB10       0.8
    7：电压报警：ADC:AD->PA1,BEEP->PA3                                                0.9
