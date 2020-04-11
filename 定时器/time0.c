#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

sbit wela = P2^7; // λѡ������
sbit dula = P2^6; // ��ѡ������

// �����������
uchar table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void delay(uint m)
{
	uint i,j;
	for(i = m; i > 0; i--)
		for(j = 114; j > 0 ; j--); 		
} 

void display(uchar number)
{
	uchar x, y, z;
	x = number/100;	// ��λ
	y = number%100 /10; // ʮλ
	z = number % 10; // ��λ

	// ��λѡ������, ��λ
	P0 = 0xff; // �������
	wela = 1;
	P0 = 0xfe; // 1111 1110
	wela = 0;

	// �򿪶�ѡ������
	dula = 1;
	P0 = table[x];
	dula = 0; // �رն�ѡ������
	delay(5);

	// ��λѡ������, ʮλ
	P0 = 0xff; // ��������
	wela = 1;
	P0 = 0xfd; // 1111 1101
	wela = 0;

	// �򿪶�ѡ������
	dula = 1;
	P0 = table[y];
	dula = 0; // �رն�ѡ������
	delay(5);

	// ��λѡ������, ��λ
	P0 = 0xff; // ��������
	wela = 1;
	P0 = 0xfb; // 1111 1011
	wela = 0;

	// �򿪶�ѡ������
	dula = 1;
	P0 = table[z];
	dula = 0; // �رն�ѡ������
   	delay(5);
}

// ��ʱ��0��ʼ��
void timer0Init()
{
	TR0 = 1; // ��ʱ��0�����п���λ����1������T0��ʼ����
	TMOD = 0x01; // ��ʱ������ģʽ1��16λ��ʱ������ģʽ
	TH0 = 0x4b; // ��8λ
	TL0 = 0xfd; // ��8λ����ʱ50ms	
}

void main()
{
	uchar mSec, Sec; // ���봢�����
	timer0Init(); // ��ʱ��0��ʼ��
	while(1)
	{
		if(TF0); //	����жϱ�־
		{
			TF0 = 0; // ������������־λ
			TH0 = 0x4b; // ��8λ
			TL0 = 0xfd; // ��8λ����ʱ50ms
			mSec++; // 50ms��
			if(mSec == 20)
			{
				mSec = 0;
				Sec++; // 1sʱ�䵽
				if(Sec == 10)
					Sec = 0; // ������
			}
		}
		display(Sec);  // �������ʾ����
		
	}
}