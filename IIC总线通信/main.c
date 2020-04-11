#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char
#define At24c02ADDR 0xa0
#define I2cRead 1
#define I2cWrite 0

sbit DU = P2^6;  //����ܶ�ѡ
sbit WE = P2^7;  //����ܶ�ѡ
sbit SCL = P2^1;
sbit SDA = P2^0;
bit AckFlag;
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
	TMOD |= 0X01; //��ʱ������ģʽ1��16λ��ʱ������ģʽ
	TH0 = 0xed;
	TL0 = 0xff; //��ʱ5ms
}

/*************************************
IICͨ�Ŵ���
*************************************/
void delay5us()
{
	_nop_(); // �⺯������ʱһ����������
}
// ��ʼ�ź�
void I2cStart()
{
	 SCL = 1;
	 SDA = 1;
	 delay5us();
	 SDA = 0;
	 delay5us();
}
// ��ֹ�ź�
void I2cStop()
{
	SCL = 0;
	SDA = 0;
	SCL = 1;
	delay5us();
	SDA = 1;
	delay5us();
}
// ���߶��ӻ�Ӧ���ź�
bit ReadACK()
{
	SCL= 0;  // ����ʱ�����ߣ�����ӻ�����SDA
	SCL = 1; // ���ߣ���SDA
	delay5us();
	if(SDA)	 // NOACK
	{
		SCL = 0;
		return(1);
	}
	else  // ACK
	{
		SCL = 0;
		return(0);
	}
}
// ��������֮�������Դӻ�����Ӧ�𣬴ӻ��ͷ�SDA��
void SendACK(bit i)
{
	SCL = 0;
	if(i)   // ��������ݣ����Ӧ��
		SDA = 1;
	else
		SDA = 0;
	SCL = 1;
	delay5us();
	SCL = 0;
	SDA = 1;  // �ͷ���������
}

// I2c����һ���ֽ�����
void I2cSendByte(uchar DAT)
{
	uchar i;
	for(i=0; i<8; i++)
	{
		SCL = 0;
		if(DAT & 0x80)
			SDA = 1;
		else
			SDA = 0;
		SCL = 1;
		DAT <<= 1;	// ����
	}
	SCL = 0;
	SDA = 1;  // �ͷ���������
}

// AT24c02ָ����Ԫд��һ���ֽ�����
void At24c02Write(uchar ADDR, DAT)
{
	I2cStart();
	I2cSendByte(At24c02ADDR + I2cWrite);// I2C����һ���ֽ�
	if(ReadACK())  // ���ӻ�Ӧ��
	{
		AckFlag = 1; // NO ACK
	}
	else
		AckFlag = 0;  // ACK
	I2cSendByte(ADDR);// I2C�����׵�ַ
	if(ReadACK())
	{
		AckFlag = 1; // NO ACK
	}
	else
		AckFlag = 0;  // ACK
	I2cSendByte(DAT);// I2C��������
	if(ReadACK())	// ��Ӧ��
	{
		AckFlag = 1; // NO ACK
	}
	else
		AckFlag = 0;  // ACK
	I2cStop();
}

// I2c���߶�һ�ֽ�����
uchar I2cReadByte()
{
	uchar i, DAT;
	for(i=0; i<8; i++)
	{
		DAT <<= 1;	// ��������1λ��ÿ�ζ�һλ
		SCL = 0;
		SCL = 1;
		if(SDA)
			DAT |= 0x01;
	}
	return(DAT);
}
// ��AT24C02ָ����Ԫ������
uchar At24C02Read(uchar ADDR)
{
	uchar DAT;
	I2cStart();
	I2cSendByte(At24c02ADDR + I2cWrite);// I2C����һ���ֽ�
	if(ReadACK()) // ���ӻ�Ӧ��
	{
		AckFlag = 1; // NO ACK
	}
	else
		AckFlag = 0;  // ACK
	I2cSendByte(ADDR);// I2C�����׵�ַ
	ReadACK();
	I2cStart();
	I2cSendByte(At24c02ADDR + I2cRead);// ����������ַ�Ӷ�д����λ ��
	if(ReadACK())	// ���ӻ�Ӧ��
	{
		AckFlag = 1; // NO ACK
	}
	else
		AckFlag = 0;  // ACK
	DAT = I2cReadByte(); //
	SendACK(1);	// �������ͷ�Ӧ��
	I2cStop();
	return(DAT);

}

void main()//main���������ѭ��
{
	timer0Init();//��ʱ��0��ʼ��
	EA = 0; // �����ж�
	At24c02Write(3, 188);   // �ڵڶ�����ַ��д��9
	delay(10);	  // �ȴ�����
	num = At24C02Read(3);
	if(AckFlag)
		P1 = 0;
	else
		P1 = 0xff;
	EA = 1; // ���ж�
	while(1);	
}  

// �жϷ�ʽ��Ӳ����ʽ��0��������ʽ���������
void timer0() interrupt 1  // ��ʱ��0���ж����1
{
	TH0 = 0Xed;
	TL0 = 0Xff;	
	display(num); //�������ʾ����
}