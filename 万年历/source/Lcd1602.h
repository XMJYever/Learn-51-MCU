#ifndef _Lcd1602_H_
#define _Lcd1602_H_

void Init_LCD1602();  // LCD1602初始化
void LCD1602_Dis_Str(uint8 x, uint8 y, uint8 *str); // LCD1602写字符串
void LCD1602_Dis_OneChar(uint8 x, uint8 y, uint8 dat); // 写一个字节
void LCD1602_Set_Cursor(); // LCD1602关闭光标显示
void LCD1602_Open_Cursor(); // LCD1602打开光标显示
void LCD1602_Clear_Screen(); // LCD1602清屏

#endif