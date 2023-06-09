#include "reg51.h"
#include "lcd.h"
#include "delay.h"

sbit led_r  = P0^0;
sbit led_y  = P0^1;
sbit led_g  = P0^2;
sbit CS   = P3^4;
sbit CLK  = P3^5;
sbit DIDO = P3^6;
sbit key1 = P3^2;
sbit key2 = P3^3;
sbit fan = P3^7;
sbit buzz = P0^3;

static float adc_val = 0;

void adc_sample()
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
    CS = 1;                                  //ADC0832停止工作
}

void set_led_rgb(uint8_t on1, uint8_t on2, uint8_t on3)
{
    led_g = on3;
    led_y = on2;
    led_r = on1;
}

static uint8_t dust_monitor_val = 35;

void int_0() interrupt 0
{
    DelayMs(2);
    if (key1 == 0)
    {
        dust_monitor_val += 1;
    }
}

void int_1() interrupt 2
{
    DelayMs(2);
    if (key2 == 0)
    {
        dust_monitor_val -= 1;
    }
}


void main()
{
    // LCD_Init();
    // LCD_Clear();
		buzz = 0;
	  fan = 0;
    lcd_test();

    set_led_rgb(1, 1, 1);


    EA = 1;
    EX0 = 1;
    IT0 = 1;
    EX1 = 1;
    IT1 = 1;

    while(1)
    {
        DelayMs(10);

        adc_sample();

        lcd_display(dust_monitor_val, adc_val);

        if (dust_monitor_val <= adc_val * 100)
        {
            set_led_rgb(0, 1, 1);
            buzz = 1;
            fan = 1;
            continue;
        }
        if (dust_monitor_val <= adc_val * 200)
        {
            set_led_rgb(1, 0, 1);
            buzz = 0;
            fan = 0;
            continue;
        }
        set_led_rgb(1, 1, 0);
        buzz = 0;
        fan = 0;
        continue;
    }
}
