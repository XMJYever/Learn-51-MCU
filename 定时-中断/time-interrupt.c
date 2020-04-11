#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2^6;//����ܶ�ѡ
sbit WE = P2^7;//����ܶ�ѡ
sbit key_s2 = P3^0;//��������S2
sbit key_s3 = P3^1;//��������S3
uchar mSec, Sec;//������봢�����
uchar num; // �������ʾ��ֵ

//��������ܶ�ѡ��0-9
uchar  code SMGduan[]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,};
// �����λѡ��
uchar code SMGwei[] = {0xfe, 0xfd, 0xfb};
/*====================================
����	�� delay(uint z)
����	��z ��ʱ�����趨��ȡֵ��Χ0-65535
����ֵ	����
����	��12T/Fosc11.0592M���뼶��ʱ
====================================*/
void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--); 		
} 

/*====================================
����	��display(uchar i)
����	��i ��ʾ��ֵ��ȡֵ��Χ0-255
����ֵ	����
����	����λ��������ܶ�̬��ʾ
====================================*/
void display(uchar i)
{
	static uchar wei; // �ֲ���̬����
	//��һλ�����  		
	P0 = 0XFF;//�������
	WE = 1;//��λѡ������
	P0 = SMGwei[wei]; // 1111 1110
	WE = 0;//����λѡ����
	switch(wei)
	{
		case 0:	DU = 1; P0 = SMGduan[i/100]; DU = 0; break;
		case 1:	DU = 1; P0 = SMGduan[i%100/10]; DU = 0; break;
		case 2:	DU = 1; P0 = SMGduan[i%10]; DU = 0; break;
	}
	wei++;
	if(wei == 3)
		wei = 0;
}
//��ʱ��0��ʼ��
void timer0Init()
{
	EA = 1;	// �����ж�
	ET0 = 1; // �򿪶�ʱ��0�ж�
	TR0 = 1;	 //������ʱ��0
	TMOD = 0X01; //��ʱ������ģʽ1��16λ��ʱ������ģʽ
	TH0 = 0xed;
	TL0 = 0xff; //��ʱ5ms
}

void main()//main���������ѭ��
{	
	timer0Init();//��ʱ��0��ʼ��
	while(1)
	{
		if(key_s2 == 0)//�ж�S2�Ƿ񱻰���
		{
			delay(20);//��������
			if(key_s2 == 0)
			{
				if(num != 9)//���ֵ������9��+1�����ܰ�ֵ�޶�ΪС��9
				num++;
				while(!key_s2);//���ּ��
			}	
		}
		if(key_s3 == 0)//�ж�S3�Ƿ񱻰���
		{
			delay(20);//��������
			if(key_s3 == 0)
			{
				if(num > 0)	//�������0��ִ�м�һ
					num--;
				while(!key_s3);//���ּ��
			}	
		}
//		//����֮��ˢ����ʾ
//		DU = 1;//�򿪶�ѡ������
//		P0 = SMGduan[num];//
//		DU = 0;//�����ѡ����
	}	
}  

// �жϷ�ʽ��Ӳ����ʽ��0��������ʽ���������
void timer0() interrupt 1  // ��ʱ��0���ж����1
{
	TH0 = 0Xed;
	TL0 = 0Xff;	
	display(num); //�������ʾ����
}