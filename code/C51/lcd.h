#ifndef __LCD_H__
#define __LCD_H__
#include "stdint.h"
void LCD_Write_String(unsigned char x, unsigned char y, unsigned char *s);
void lcd_display(uint8_t set_val, float val);
void lcd_test(void);

#endif