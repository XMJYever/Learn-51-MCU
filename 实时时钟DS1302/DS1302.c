#include <reg52.h>
#include <intrins.h>
#define MAIN_Fosc		11059200UL	//�궨����ʱ��HZ
/*====================================
 �Զ���������
====================================*/
typedef unsigned char INT8U;
typedef unsigned char uchar;

typedef unsigned int INT16U;
typedef unsigned int uint;

/*====================================
 Ӳ���ӿ�λ����
====================================*/
sbit TSCLK = P1^0;  // ʱ���ߣ��ӵ�P10��
sbit TIO   = P1^1;	// ������
sbit TRST  = P1^2;	// ʹ�ܶ�
sbit DU  = P2^6;   //����ܶ�ѡ
sbit WE  = P2^7;   //�����λѡ
/*====================================
����������ܶ�ѡ��
====================================*/
uchar code table[]={ 
//0		1	 2     3     4     5     6     7     8
0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
//9     A     B	   C	 D	   E	 F		-	 .	  ����ʾ
0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x40, 0x80, 0x00
                   };

/*====================================
�����λѡ��
====================================*/
				  //��1λ	2λ	  3λ	 4λ   5λ	6λ	  7λ	8λ
uchar code T_COM[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};//�����λ��

/*====================================
������void Delay_Ms(INT16U ms)
������ms��������ʱ�β�
������12T 51��Ƭ������Ӧ��ʱ�Ӻ��뼶��ʱ����
====================================*/
void Delay_Ms(INT16U ms)
{
     INT16U i;
	 do{
	      i = MAIN_Fosc / 96000; 	  // //96��ʱ�����ڣ�8����������
		  while(--i);   //96T per loop
     }while(--ms);
}
/*====================================
������void Display(INT16U Value)
������Value,��ʾֵ ȡֵ0-65535
�������������������ʾ��������ʾһ���ֽڵ���
====================================*/
void Display(uchar Hour, Min, Sec)			//ע��������Ҫ��ʾ��������һ���ֽ������β���Ϊint��
{	
//------------------------------
	DU = 0;							//�رն�ѡ
	P0 = table[Hour/10];		//�������ʾСʱ
	DU = 1;							//�򿪶�ѡ
	DU = 0;							//�رն�ѡ

	WE = 0;						//�ر�λѡ
	P0 = T_COM[0];				   //��һλ�����
	WE = 1;						//��λѡ
	WE = 0;						//�ر�λѡ
	Delay_Ms(3);
//-------------------------------
	DU = 0;
	P0 = table[Hour%10]|0x80; //��ʾ�ڶ�λ
	DU = 1;
	DU = 0;

	WE = 0;
	P0 = T_COM[1];			  //�ڶ�λ�����
	WE = 1;
	WE = 0;
	Delay_Ms(3);
//-------------------------------
	DU = 0;
	P0 = table[Min/10];		//��ʾ��λ
	DU = 1;
	DU = 0;

	WE = 0;
	P0 = T_COM[2];				//����λ�����
	WE = 1;
	WE = 0;
	Delay_Ms(3);
//-------------------------------
	DU = 0;
	P0 = table[Min%10]|0x80;		//��ʾ��
	DU = 1;
	DU = 0;

	WE = 0;
	P0 = T_COM[3];				//����λ�����
	WE = 1;
	WE = 0;
	Delay_Ms(3);
//-------------------------------
	DU = 0;
	P0 = table[Sec/10];		//��ʾ��
	DU = 1;
	DU = 0;

	WE = 0;
	P0 = T_COM[4];				//����λ�����
	WE = 1;
	WE = 0;
	Delay_Ms(3);
//-------------------------------
	DU = 0;
	P0 = table[Sec%10];		//��ʾ
	DU = 1;
	DU = 0;

	WE = 0;
	P0 = T_COM[5];				//����λ�����
	WE = 1;
	 WE = 0;
	Delay_Ms(3);
}
// дDS1302����
void Write_DS1302_DAT(uchar cmd, uchar dat)
{	// ����DS1302���ʱ��ͼ��ʵ�ֵ�
	 uchar i;
	 TRST = 0;	 // ����ʹ�ܶ�
	 TSCLK = 0;	 // ����������
	 TRST = 1;	 // ����ʹ�ܶˣ����������ؿ�ʼд����
	 // ����ָ��
	 for(i=0;i<8; i++)
	 {
	 	 TSCLK = 0;
		 TIO = cmd & 0x01;
		 TSCLK = 1;	 // ����ʱ�����ߣ��������������ݱ�DS1302����
		 cmd >>= 1; // ����һλ
	 }
	 // д����
	 for(i=0; i<8; i++)
	 {
	 	TSCLK = 0;
		TIO = dat & 0x01;
		TSCLK = 1;   // ����ʱ�����ߣ����������ر�DS1302����
		dat	>>= 1; //����һλ
	 }
}
// ��DS1302����
uchar Read_DS1302_DAT(uchar cmd)
{
	uchar i, dat;
	TRST = 0;
	TSCLK = 0;
	TRST = 1;
	// TSCLKΪ������ʱ���ж�ָ��
	for(i=0; i<8; i++)
	{
		TSCLK = 0;
		TIO = cmd & 0x01;
		TSCLK = 1;		// ���ߣ�����������
		cmd >>= 1; // ����һλ
	}
	// ������
	for(i=0; i<8; i++)
	{
		TSCLK = 0;  // �����½��أ�������
		dat >>= 1;	// ����һλ
		if(TIO) dat |= 0x80;  // ��ȡ���ݣ������λ��ʼ
		TSCLK = 1;	   // ����ʱ�����ߣ��Ա���һ�β����½���
	} 
	return dat;
}

// ����תBCD��
uchar Dat_Chg_BCD(uchar dat)
{
	uchar dat1, dat2;
	// ʮ����תʮ������
	dat1 = dat/10;
	dat2 = dat%10;
	dat2 = dat2 + dat1*16;
	return dat2;
}

// BCDת��Ϊ����
uchar BCD_Chg_Dat(uchar dat)
{
	uchar dat1, dat2;
	dat1 = dat/16;
	dat2 = dat%16;
	dat2 = dat2 + dat1*10;
	return dat2;
}

void main()
{
	uchar i;
	uchar Sec,Min,Hour;
	Write_DS1302_DAT(0x8e,0); // ���д����
	Write_DS1302_DAT(0x80,Dat_Chg_BCD(30)); // д�룬30s
	Write_DS1302_DAT(0x82,Dat_Chg_BCD(17));	// д�֣�25��
	Write_DS1302_DAT(0x84,Dat_Chg_BCD(16));	// 16ʱ
	Write_DS1302_DAT(0x8e,0x80);  // ��д����
	while(1)
	{
		 Write_DS1302_DAT(0x8e,0);	//���д����
		 Sec = BCD_Chg_Dat(Read_DS1302_DAT(0x81)); // ����Ĵ��������ҽ���BCD��ת����
		 Min = BCD_Chg_Dat(Read_DS1302_DAT(0x83)); // ���ּĴ���
		 Hour = BCD_Chg_Dat(Read_DS1302 _DAT(0x85));// ��ʱ�Ĵ���
		 Write_DS1302_DAT(0x8e,0x80); // ��д����
		 for(i=0; i<50; i++)	//ѭ����ʾʱ��
		 	Display(Hour,Min,Sec);
	}
}