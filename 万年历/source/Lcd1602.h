#ifndef _Lcd1602_H_
#define _Lcd1602_H_

void Init_LCD1602();  // LCD1602��ʼ��
void LCD1602_Dis_Str(uint8 x, uint8 y, uint8 *str); // LCD1602д�ַ���
void LCD1602_Dis_OneChar(uint8 x, uint8 y, uint8 dat); // дһ���ֽ�
void LCD1602_Set_Cursor(); // LCD1602�رչ����ʾ
void LCD1602_Open_Cursor(); // LCD1602�򿪹����ʾ
void LCD1602_Clear_Screen(); // LCD1602����

#endif