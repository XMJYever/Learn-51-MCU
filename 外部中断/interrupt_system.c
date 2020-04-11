		#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2^6;//����ܶ�ѡ
sbit WE = P2^7;//����ܶ�ѡ
sbit key_s2 = P3^0;//��������S2
sbit flag = P3^7;//�ⲿ�ж��źŲ�����
uchar num;//�������ʾ��ֵ

//���뼶��ʱ��������
void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--); 		
} 

//�ⲿ�ж�1��ʼ��
void int1Init()
{
	EA = 1;	//�����ж�
	EX1 = 1; // ���ⲿ�ж�1
	IT1 = 1; // �ⲿ�ж�1�½��ش���
}

// �ⲿ�ж�1�жϷ������
void int1() interrupt 2
{
	P1 = ~P1;  // ȡ��
}

void main()//main���������ѭ��
{
	int1Init(); // ��ʼ��
	while(1)
	{
		if(key_s2 == 0)//�ж�S2�Ƿ񱻰���
		{
			delay(20);//��������
			if(key_s2 == 0)
			{
				flag = 1;
				flag = 0;  // �������½���
				while(!key_s2);//���ּ��
			}	
		}
	}	
}  