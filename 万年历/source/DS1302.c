#include "config.h"
#include "DS1302.h"
#include "LCD1602.h"

/*
	ʵʱʱ��DS1302ģ��
	������������/���������ӿ���SCLK��IO��RST
	���ж����ݡ�д���ݵĹ���
	�����ݣ�RST���ߣ��������ȶ��Ĵ���ָ��ٴ��½��ض�����
	д���ݣ�RST���ߣ�������д�Ĵ���ָ���������д����
*/
#define uchar unsigned char
#define uint unsigned int

// �����ݶ���
#define YEAR_R 0X8D    // ��������
#define DAY_R 0X8B     // ����������
#define MONTH_R 0X89   // ��������
#define DATE_R 0X87    // ��������
#define HR_R 0X85      // ��Сʱ����
#define MIN_R 0X83     // ��������
#define SEC_R 0X81     // ��������

#define CONTROL_R 0X8F // ����������
#define TRICKLE_CHARGER_R 0X91 // ������������
#define CLOCK_BURST_R 0xBF //��ʱ����ֽ�����;

#define RAM_BURST_R 0xFF //��RAM�ַ�������;
// ��緽ʽ���ƼĴ���
#define TCS_OFF 0x00 //���ģʽ�رճ��;
#define TCS1 0xA5 //���ģʽѡ��һ��������,����2K;
#define TCS2 0xA6 //���ģʽѡ��һ��������,����4K;
#define TCS3 0xA7 //���ģʽѡ��һ��������,����8K;
#define TCS4 0xA9 //���ģʽѡ�����������,����2K;
#define TCS5 0xAA //���ģʽѡ�����������,����4K;

// д���ݶ���
#define YEAR_W (0x8D&0xFE) // д ������;
#define DAY_W (0x8B&0xFE) //д ��������;
#define MONTH_W (0x89&0xFE) //д������;
#define DATE_W (0x87&0xFE) //д������;
#define HR_W (0x85&0xFE) //дСʱ����;
#define MIN_W (0x83&0xFE) //д������;
#define SEC_W (0x81&0xFE) //д������;
#define CONTROL_W 0x8F&0xFE //д��������;
#define TRICKLE_CHARGER_W 0x91&0xFE //д����������;
#define CLOCK_BURST_W 0xBF&0xFE //дʱ����ֽ�����;
#define TCS6 0xAB //���ģʽѡ�����������,����8K;
#define RAM_BURST_W 0xFE //дRAM�ַ�������; 

// ���ֽ�
uchar Read_Byte()
{
	uint8 i, j, dat;
	TSCLK = 1;
	for(i=0; i<8; i++)
	{
		TSCLK = 0; // �����½���
		if(TIO)
			dat |= 0x80;  // ������λ�����ҽ��丳ֵ��dat
		dat >>= 1;  // ����һλ
		TSCLK = 1;  // ����ʱ������
	}
}

// д�ֽ�
void Write_Byte(uint8 dat)
{
	uint8 i;
	TSCLK = 0;
	for(i=0; i<8; i++)
	{
		TSCLK = 1; // ����������
		TIO = dat & 0x01; // ȡ���λ����������
		dat >>= 1; // ����һλ
		TSCLK = 0; // �ͷ�����
	}
}

// дDS1302����һ��д�����ֽ�
// ��д�����д����
void DS1302_W_DAT(uint8 cmd, uint8 dat)
{
	TRST = 0;
	TRST = 1;			 // TRST�����ؿ�ʼ��д����
	Write_Byte(cmd);  // д����
	Write_Byte(dat);  // д����
	TRST = 0;         // ��ֹ��д����
}

// ��DS1302���ݣ���д�������ֽں������Ӧ����
uint DS1302_R_DAT(uint8 cmd)
{
	uint8 dat;
	TRST = 0;
	TRST = 1;
	Write_Byte(cmd);      // д����
	dat = Read_Byte(); 	  // ��������
	TRST = 0;             // ��ֹ��д����
	return dat;           // ���ض�������
}

// ���д��������д������0x8e������write 0x00
void DS1302_Clear_WP()
{
	DS1302_W_DAT(0x8e, 0x00);  // ���д����������д������
}

// ����д����
void DS1302_Set_WP()
{
	DS1302_W_DAT(0x8e, 0x80);  // ����д��������ֹд������
	TRST = 0;    // ����ʹ�ܶ�
	TSCLK = 0;   // ������������
}

// ͻ��ģʽ��DS1302ʱ���������ݣ�����TimeData(���ݸ�ʽBCD��)
void DS1302_Burst_Read(uint8 *dat)  // dat��һ��ָ������
{
	uint8 i;
	DS1302_Clear_WP();  // ���д����
	TRST = 0;           // ����ʹ�ܶ�
	TSCLK = 0;          // ������������
	TRST = 1;           // ����ʹ�ܶˣ���ʼд����
	Write_Byte();       // ��ͻ��ģʽ�Ĵ���ָ��
	for(i=0; i<8; i++)
	{
		dat[i] = Read_Byte();   //��ͻ��ģʽ�¿�������������
	}
	DS1302_Set_WP();        // ����д����
}
// ͻ��ģʽдDS1302ʱ���������ݣ�����TimeData
void DS1302_Burst_Write(uint8 *dat)
{
	uint8 i;
	DS1302_Clear_WP(); // ���д����
	TRST = 0;          // ����ʹ�ܶ�
	TRST = 1;          // ����ʹ�ܶ�
	Write_Byte(0xbe);      // ����ͻ��ģʽ,д�Ĵ���
	for(i=0; i<8; i++)
	{
		Write_Byte(*dat[i]);   // ���ν�����д��
	}
	DS1302_Set_WP();          // ����д����
}
// ͻ��ģʽ��DS1302ʱ���������ݣ���ת��Ϊʱ���ʽ
void GetRealTime(struct *time)
{
	uint8 buf[8];

	DS1302_Burst_Read(buf);
	time->year = buf[] + 0x2000; // �꣬һλDS1302ֻ����0-99����ʾ2012����ͨ��2000���ַ�����ʾǧλ�Ͱ�λ
	time->mon  = buf[4];
    time->day  = buf[3];
    time->hour = buf[2];
    time->min  = buf[1];
    time->sec  = buf[0];
    time->week = buf[5];
}

// ͻ��ģʽ����DS1302ʱ����������
void SetRealTime(struct sTime *time)
{
	uint8 buf[8];

	buf[7] = 0;
	buf[6] = time->year;
    buf[5] = time->week;
    buf[4] = time->mon;
    buf[3] = time->day;
    buf[2] = time->hour;
    buf[1] = time->min;
    buf[0] = time->sec;
    DS1302_Burst_Write(buf);
} 
// ��ʼ��DS1302������Ĭ��ʱ�䣺2020-04-02 23:59:50 ������
void Init_DS1302()
{
	struct sTime code InitTime[] = { 
        0x2020 , 0x04, 0x02, 0x23, 0x59, 0x50, 0x02
    };
	SetRealTime(&InitTime);      //����DS1302ΪĬ�ϵĳ�ʼʱ��
}