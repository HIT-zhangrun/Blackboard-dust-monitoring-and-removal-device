#include "led_buzz.h"
#include "delay.h"
#include "adc.h"

sbit led_r  = P3^0;
sbit led_y  = P3^1;
sbit led_g  = P3^2;
sbit buzzer = P0^3;
sbit fan    = P0^7;

void led_buzz_fan_monitor()
{
    // int32_t monitor_val = 0;
    // monitor_val = get_dust_monitor_val();
    // int32_t adc_val get_adc_val();

    // if (adc_val > monitor_val)
    // {
    //     set_system_error();
    // }
    // if (adc_val > monitor_val / 2)
    // {
    //     set_system_warn();
    // }
    // else
    // {
    //     set_system_normal();
    // }
}

void led_init()
{
    set_led_rgb(1, 1, 1);
}
