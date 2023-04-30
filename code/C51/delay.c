#include "delay.h"

void DelayMs(unsigned int nms)
{
    unsigned char i, j;
    while(nms--)
    {
        i = 15;
        j = 90;
        do
        {
            while (--j);
        } while (--i);
    }
}
