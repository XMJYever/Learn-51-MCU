#include "config.h"
#include "Lcd1602.h"
/* Һ����ʾ����
	��Ҫ�����¼������ܣ�1�����æ�źţ�
	2��дָ������ݡ�д���ݣ�3��������ʾ��
	4����ʾһ���ֽ����ݣ�5����ʾһ���ֽ��ַ�
	6����ʾ�����ַ��� 
 */

// �ж�Һ��æ��æ�����æ��ȴ�
void Read_Busy()
{
	uint8 busy;
	P0 = 0xff;
	RS = 0;	 // ������
	RW = 1;  // ѡ�������
	do
	{
		EN = 1;
		busy = P0;	// ��P0�ӿڵ�״̬��
		EN = 0;
	}while(busy & 0x80);  // ���æ����ȴ�
}

// дָ��
void Write_Cmd(uint8 cmd)
{
	Read_Busy();
	RS = 0;	   // дָ��
	RW = 0;
	EN = 1;
	P0 = cmd;
	EN = 0;
}

// д����
void Write_Dat(uint8 dat)
{
	Read_Busy();
	RS = 1;
	RW = 0;   // д����
	P0 = dat;
	EN = 1;
	EN = 0;
}

// ������ʾ
void LCD1602_Set_Cursor(uint8 x,uint8 y)
{
	uint8 addr;
	if(y)
		addr = 0x40 + x;
	else
		addr = 0x00 + x;
	Write_Cmd(0x80 | addr); 
}

// ��ʾһ���ֽ�����
void LCD1602_Dis_OneChar(uint8 x, uint8 y, uint8 dat)
{
	LCD1602_Set_Cursor(x, y);	// ����ָ�ѡ��Ҫ��ʾ��λ��
	Write_Dat(dat);		// dat����д����
}

// LCD1602д�ַ���
void LCD1602_Dis_Str(uint8 x, uint8 y, uint8 *str)
{
	LCD1602_Set_Cursor(x, y);
	while(*str != '\0')
	{
		LCD1602_Write_Dat(*str++);
	}
}
// LCD1602�رչ����ʾ
void LCD1602_Close_Cursor()
{
	 Write_Cmd(0x80); // �رչ����ʾ
}
// LCD1602�򿪹����ʾ
void LCD1602_Open_Cursor()
{
	Write_Cmd(0x0f);  // �򿪹����ʾ�������˸
}
// LCD1602����
void LCD1602_Clear_Screen()
{
	Write_Cmd(0x01);  // ����
} 
// LCD1602��ʼ��
void Init_LCD1602()
{
	Write_Cmd(0x38); // ����16*2��ʾ��5*7����8λ���ݽӿ�
	Write_Cmd(0x0c); // ����ʾ���رչ��
	Write_Cmd(0x06); // ��дһ�ֽں��ַָ���1
	Write_Cmd(0x01); // ����
}