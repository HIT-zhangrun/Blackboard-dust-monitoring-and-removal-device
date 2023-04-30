#include "adc.h"
#include "stdint.h"
#include <reg52.h>
#include <intrins.h>

sbit LED_ctl = P3^2
sbit CS   = P3^4
sbit CLK  = P3^5
sbit DIDO = P3^6

static int32_t adc_val = 0;

int32_t get_adc_val()
{
    return adc_val;
}

int32_t adc_sample()
{
    int32_t i, dat;
    LED_ctl = 1;
    CS = 1;              //ADC0832未工作时，CS端为高电平，此时芯片禁用
    CLK =1;
    CS = 0;
    DIDO = 1; CLK = 0;  CLK = 1; //SCLK第一个下降沿来到时，DI = 1启动ADC0832
    DIDO = 1; CLK = 0;  CLK = 1;  //SCLK第二个下降沿 
    DIDO = 0; CLK = 0;  CLK = 1; //SCLK第三个下降沿，发送1,0选择通道cho
    DIDO = 1;                                //释放总线
    for(i = 0; i < 8; i++)                  //第4个下降沿到第11个下降沿
    {
        CLK = 0;           //clk处于下降沿，每一个下降沿DO端输入下一个
        if(DIDO) dat |= 0x01;
        // dat = dat | 0x01,dat和0x01做“或”的运算，意思是最低位置1，其它位保持不变。
        CLK = 1;                       //clk处于上升沿
        dat <<= 1;                          //数据左移
    }
    LED_ctl = 0;
    adc_val = dat;
    return (dat);
    CS = 1;                                  //ADC0832停止工作
}
