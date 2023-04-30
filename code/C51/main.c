#include "reg51.h"
#include "adc.h"
#include "lcd.h"
#include "key.h"
#include "delay.h"

sbit led_r  = P0^0;
sbit led_y  = P0^1;
sbit led_g  = P0^2;

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
    float i = 0.1;

    lcd_test();
    set_led_rgb(1, 1, 1);
    DelayMs(100);
    set_led_rgb(0, 0, 0);

    lcd_display(i);
    DelayMs(20);



    while(1)
    {

        // int32_t adc_val = adc_sample()
        // lcd_display(adc_val);
        // key_monitor();
        // led_buzz_fan_monitor();
    }
}
