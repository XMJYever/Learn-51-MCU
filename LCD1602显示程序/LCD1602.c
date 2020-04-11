#include <reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint;

// �ܽ�����
sbit EN = P3^4;	   // ʹ���ź�
sbit RW = P3^6;	   // ��/дѡ���
sbit RS = P3^5;	   // ����/����ѡ���(H/L)
// �ж�Һ��æ��æ�����æ��ȴ�
void Read_Busy()
{
	uchar busy;
	P0 = 0xff;
	RS = 0;
	RW = 1;
	do
	{
		EN = 1;
		busy = P0; // ��P0�ӿڵ�״̬�֣�D7λ��д����ʹ��
		EN = 0;	
	}while(busy & 0x80);
}
// дLCD1602����
void Write_Cmd(uchar cmd)
{
	Read_Busy();
	RS = 0;
	RW = 0;
	P0 = cmd;
	EN = 1;  //	����ָ���LCD
	EN = 0;  //�ͷ�
}
// дһ���ֽ�����
void Write_Dat(uchar dat)
{
	Read_Busy();
	RS = 1;
	RW = 0;
	P0 = dat;
	EN = 1;
	EN = 0;
}

void main()
{
	uchar i;
	uint k = 1000;
	uchar *a[] = {"Monday", "Tuesday", "Wednesday"};
	uchar *p;
	Write_Cmd(0x38);  // ����16*2��ʾ
	Write_Cmd(0x0f);  // ����ʾ
	Write_Cmd(0x01);  // ����
	Write_Cmd(0x07);  // ��ַָ����λ����
	Write_Cmd(0x80 | 0x10);  // ��ʾ��ַ
	p = a;
	for(i=0; i<3; i++)
	{
		p = a[i];
		while(*p != '\0')
			{
				 Write_Dat(*p++);  // д����	 ����acsii������ʾ
				 while(k--);
			}
	}
	while(1);
}