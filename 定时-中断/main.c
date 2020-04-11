#include <reg52.h>

void time0_Init()
{
	TMOD |= 0x01;
	TR0 = 1; // �򿪶�ʱ��
	TH0 = 56320/256; // �������Ϊ10ms���һ��
	TL0 = 56320%256;
}

void time1_Init()
{

}

void ISR_Init()
{
	EA = 1; //���жϿ���
	ET0 = 1; // ������ʱ���жϣ�TF0����λ��1������TF0=1ʱ���ж�ϵͳ����
	EX0 = 1; // �ⲿ�ж�0
	EX1 = 0;
	ET1 = 0;
	ES = 0;  // UART�ж�û�п���
}

// �жϷ����ӳ���ϣ���ж�ϵͳ�����ã���������main�����е���
void IE0_isr() interrupt 0
{

}

void TF0_isr() interrupt 1 // �����������һ�Σ����������˼�����㣬����10ms���һ��
{
	static char c;
	TH0 = 56320/256;  // ��װ��ֵ
	TL0 = 56320%256;
	c++;
	if(c==50)
	{
		P1^=(1<<0);
		c = 0;
	}
}

void IE1_isr() interrupt 2
{

}

void TF1_isr() interrupt 3
{

}

void RI_TI_isr() interrupt 4
{

}

void main()
{
	time0_Init();
	time1_Init();
	ISR_Init();

	while(1)
	{
		
	}
}