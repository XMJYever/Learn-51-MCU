#include <reg52.h>

#define uint unsigned int
#define uchar unsigned char

extern void load_smg();

// ��ʱ��0��ʼ��
void time0_Init()
{
	TMOD |= 0x01; // ѡ����ʱ��0��ѡ��ģʽΪ0��1
	TH0 = 60928/256;
	TL0 = 60928%256;
	TR0 = 1; // ����ʱ��0����
}

void ISR_Init()
{
	EA = 1; //�����ж�
	ET0 = 1; //	�򿪶�ʱ��0�ж�
}

void TF0_isr() interrupt 1
{
	TH0 = 60928/256;
	TL0 = 60928%256;
	load_smg();
}

void main()
{
	time0_Init();
	ISR_Init();
	while(1)
	{
	
	}
}