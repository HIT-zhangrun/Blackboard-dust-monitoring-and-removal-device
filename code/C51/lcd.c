#include <reg52.h>
#include <intrins.h>
#include "lcd.h"
#include "delay.h"
#include <stdio.h>

sbit RS = P0^6;
sbit RW = P0^5;
sbit EN = P0^4;

#define RS_CLR RS=0
#define RS_SET RS=1
#define RW_CLR RW=0
#define RW_SET RW=1
#define EN_CLR EN=0
#define EN_SET EN=1

#define DataPort P2

void DelayUs2x(unsigned char t)
{
    while(--t);
}

void LCD_Write_Com(unsigned char com)
{
    DataPort= com;
    RS_CLR;
    RW_CLR;
    DelayMs(10);
    EN_SET;
    _nop_();
    EN_CLR;
}

void LCD_Write_Data(unsigned char Data)
{
    DataPort = Data;
    RS_SET;
    RW_CLR;
    DelayMs(10);
    EN_SET;
    _nop_();
    EN_CLR;
}

void LCD_Clear(void)
{
    LCD_Write_Com(0x01);
    DelayMs(20);
}

void LCD_Write_String(unsigned char x, unsigned char y, unsigned char *s)
{
    if (y == 0)
    {
        LCD_Write_Com(0x80 + x);     //表示第一行
    }
    else
    {
        LCD_Write_Com(0xC0 + x);      //表示第二行
    }
    while (*s)
    {
        LCD_Write_Data( *s);
        s++;
    }
}

void LCD_Write_Char(unsigned char x, unsigned char y, unsigned char Data)
{
    if (y == 0)
    {
        LCD_Write_Com(0x80 + x);
    }
    else
    {
        LCD_Write_Com(0xC0 + x);
    }
    LCD_Write_Data(Data);
}

unsigned char code mychar[64] = {0x0e, 0x02, 0x0e, 0x02, 0x0e, 0x00, 0x00, 0x00};

void LCD_FillCGRAM(void)            //自定义字符的填充函数
{
    unsigned char i;
    LCD_Write_Com(0x40);             //写指令告诉是在写自定义字符
    for (i = 0; i < 8; i++)         //i <= 64
    {
        LCD_Write_Data(mychar[i]);   //把3的字模数据写进去
    }
}

void LCD_Init(void)
{
    LCD_Write_Com(0x01);
    DelayMs(20);
    LCD_Write_Com(0x38);
    DelayMs(20);
    LCD_Write_Com(0x0C);
    DelayMs(20);
    LCD_Write_Com(0x06);
    DelayMs(20);
    LCD_FillCGRAM();
    DelayMs(20);
}

void lcd_test(void)
{
    LCD_Init();
    LCD_Clear();

    LCD_Write_String(0, 0, "set:");
    LCD_Write_String(11, 0, "mg/m");
    LCD_Write_Char(15, 0, 0);

    LCD_Write_String(0, 1, "cur:");
    LCD_Write_String(11, 1, "mg/m");
    LCD_Write_Char(15, 1, 0);
}

void lcd_display(uint8_t set_val, float val)
{
    char buf[5];
    float val_tmp;
    sprintf(buf, "%0.3f", val);
    LCD_Write_String(5, 1, buf);

    val_tmp = set_val;

    sprintf(buf, "%0.3f", val_tmp / 10);
    LCD_Write_String(5, 0, buf);
}
