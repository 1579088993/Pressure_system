#include "stm32f10x_conf.h"
/**
 * 
 * 电压报警：ADC:AD->PA1,BEEP->PA3，zh
 * 
 * 这个函数要放在定时器里进行处理，定时检测。
 */

void Vol_Check(void)
{
    u8 voltage = 0;
    //Vol_Init();   
    voltage = Get_Adc_Average(ADC_CH1, 10);
    if (voltage < 10)
    {
        BEEP = 1; //打开蜂鸣器
    }
}


void Vol_Init(void)
{
    RCC->APB2ENR |= 1<<2;  //使能GPIOA
    GPIOA->CRL &= 0XFFFF0FFF;//初始化PA3
    GPIOA->CRL |= 0X00003000; //配置成输出模式，通用推挽输出，最大速率50MHz..
    BEEP = 0;//关闭蜂鸣器输出
    GPIOA->CRL &= 0XFFFFFF0F; //初始化PA1
    RCC->APB2ENR |= 1<<9; //使能ADC1
    RCC->APB1RSTR |= 1<<9;//ADC1复位
    RCC->APB1RSTR &= ~(1<<9);//复位结束
    RCC->CFGR &= ~(3<<14); //分频因子清零
    RCC->CFGR |= 2<<14; //
    ADC1->CR1 &= 0XF0FFFF; //工作模式清零
    ADC1->CR1 |= 0<<16; //独立工作模式
    ADC1->CR1 &= ~(1<<8); //非扫描模式
    ADC1->CR2 &= ~(1<<1);//单词转换模式
    ADC1->CR2 &= ~(7<<17);
    ADC1->CR2 |= 7<<17;//软件控制转换
    ADC1->CR2 |= 1<<20;//使用外部触发
    ADC1->CR2 &= ~(1<<11);//右对齐
    ADC1->SQR1 &= ~(0XF<<20);//
    ADC1->SQR1 |= 0<<20;//1个转换在规则序列里
    ADC1->SMPR2&=~(3*1);   //通道1采样时间清空	  
 	ADC1->SMPR2|=7<<(3*1); //通道1  239.5周期,提高采样时间可以提高精确度	 
	ADC1->CR2|=1<<0;	   //开启AD转换器	 
	ADC1->CR2|=1<<3;       //使能复位校准  
	while(ADC1->CR2&1<<3); //等待校准结束 			 
    //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 		 
	ADC1->CR2|=1<<2;        //开启AD校准	   
	while(ADC1->CR2&1<<2);  //等待校准结束
	//该位由软件设置以开始校准，并在校准结束时由硬件清除 

}

u16 Get_Adc(u8 ch)   
{
	//设置转换序列	  		 
	ADC1->SQR3&=0XFFFFFFE0;//规则序列1 通道ch
	ADC1->SQR3|=ch;		  			    
	ADC1->CR2|=1<<22;       //启动规则转换通道 
	while(!(ADC1->SR&1<<1));//等待转换结束	 	   
	return ADC1->DR;		//返回adc值	
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}


//Get_Adc_Average(ADC_CH1,10);