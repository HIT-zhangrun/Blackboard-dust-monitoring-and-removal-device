#include "key.h"

#include <reg52.h>
#include <intrins.h>
#include "delay.h"

sbit key_1 = P3^0;
sbit key_2 = P3^1;
sbit key_3 = P3^2;

static int16_t dust_monitor_val = 20;

void modify_dust_val(int32_t change)
{
    dust_monitor_val += change;
}

int16_t get_dust_monitor_val()
{
    return dust_monitor_val;
}

void key_monitor()
{
    if (key_1)
    {
        int8_t i = 200;
        while (i--)
        {
            if (key_2)
            {
                modify_dust_val(1);
            }
            if (key_3)
            {
                modify_dust_val(-1);
            }
            if (key_1)
            {
                break;
            }
        }
    }
}