#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2^6; // ����ܶ�ѡ
sbit WE = P2^7; // �����λѡ

sbit key_s2 = P3^0;
sbit key_s3 = P3^1;

uchar number = 0;
// ����������ܱ����
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

// ��ʱ15ms
void delay(void)   //��� -0.000000000003us
{
    unsigned char a,b;
    for(b=51;b>0;b--)
        for(a=134;a>0;a--);
}

void main()
{
	WE = 1; // ��λѡ������
	P0 = 0xfe; // 1111 1110
	WE = 0;

	while(1)
	{
		if(key_s2 == 0)	// ����2������
		{
			delay(); //��������
			number++;
			if(number == 10)
				number = 0;
			while(!key_s2); // ���ּ��
		}
		DU = 1; // �򿪶�ѡ������
		P0 = table[number]; // 0000 0110
		DU = 0; // �����ѡ����
		
		if(key_s3 == 0)	// ����3������
		{
			delay(); //��������
			if(number > 0)
				number--;
			while(!key_s3); // ���ּ��
		}
		DU = 1; // �򿪶�ѡ������
		P0 = table[number]; // 0000 0110
		DU = 0; // �����ѡ����
	}
}