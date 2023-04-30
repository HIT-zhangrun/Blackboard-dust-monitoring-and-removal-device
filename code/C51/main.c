#include "reg51.h"
#include "adc.h"
#include "lcd.h"
#include "key.h"
#include "delay.h"

sbit led_r  = P0^0;
sbit led_y  = P0^1;
sbit led_g  = P0^2;

sbit CS   = P3^4;
sbit CLK  = P3^5;
sbit DIDO = P3^6;

static float adc_val = 0;

int32_t adc_sample()
{
    uint8_t i, dat;
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

    adc_val = dat;
    adc_val = adc_val * 5 / 255;

    if (adc_val < 0.5)
    {
        adc_val = 0.5;
    }
    if (adc_val > 3.5)
    {
        adc_val = 3.5;
    }
    adc_val = (adc_val - 0.5) / 6.0;

    return (dat);
    CS = 1;                                  //ADC0832停止工作
}

void set_led_rgb(uint8_t on1, uint8_t on2, uint8_t on3)
{
    led_g = !on1;
    led_y = !on2;
    led_r = !on3;
}

void main()
{
    // LCD_Init();
    // LCD_Clear();

    lcd_test();
    set_led_rgb(1, 1, 1);
    DelayMs(100);
    set_led_rgb(0, 0, 0);

    // lcd_display(i);
    DelayMs(20);

    adc_sample();
    lcd_display(adc_val);



    while(1)
    {
        DelayMs(20);

    adc_sample();

    lcd_display(adc_val);

        // int32_t adc_val = adc_sample()
        // lcd_display(adc_val);
        // key_monitor();
        // led_buzz_fan_monitor();
    }
}
