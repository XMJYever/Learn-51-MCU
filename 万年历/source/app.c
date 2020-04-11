#include "config.h"
#include "Lcd1602.h"
#include "DS1302.h"
#include "Key_Buzzer.h"
#include "DS18B20.h"
#include "lunar.h"
#include "main.h"
#include "app.h"

struct sTime CurTime;  // ��ǰ����ʱ��
uint8 SetIndex = 0;    // ����λ����
bit AlarmOn = 0;       // ���ӿ��ر�־λ
uint8 AlarmHour = 0x06;     // ����ʱ���Сʱ��������Ĭ��ֵ6��30
uint8 AlarmMin = 0x30;     // ����ʱ��ķ�����
uint8 pdata SetDat;        // ����ʱ���������ݻ���

uint8 code *week[8] = {
		"    ", "Mon ", "Tues", "Wed ", "Thur", "Fri ", "Sat ", "Sun "
};
uint8 code *AlarmSta[2] = {	// ���ӿ���ϣ�����ַ���
		"ON ", "OFF"
}
/* BCD��תʮ���� */
uint8 BCD_DEC(uint8 BCD)
{
	uint8 DEC;
	DEC = BCD / 16;
	BCD %= 16;
	BCD += DEC * 10;
	
	return BCD; 
}
/* ʮ����תBCD */
uint8 DEC_BCD(uint8 DEC)
{
	uint8 BCD;
	BCD = DEC/10;
	DEC %= DEC%10;
	DEC += BCD*16;

	return DEC;
}
/* ��ȡ��ǰʱ�䣬��ˢ����ʾ */
void RefreshTime()
{
	uint8 pdata time[10];
	GetRealTime(&CurTime);  // ��ȡ��ǰ����/ʱ��

	time[0] = (CurTime.hour >> 4) + '0';  // Сʱʮλ�� ������λ
	time[1] = (CurTime.hour & 0x0f) + '0';  // Сʱ��λ
	time[2] = ':';
	time[3] = (CurTime.min >> 4) + '0';  // ����ʮλ
	time[4]	= (CurTime.min & 0x0f) + '0'; // ���Ӹ�λ
	time[5] = ':';
	time[6]	= (CurTime.sec >> 4) + '0';	  // ����ʮλ
	time[7] = (CurTime.sec & 0x0f) + '0'; // ���Ӹ�λ
	time[8] = '\0';                       // �ַ���������
	LCD1602_Dis_Str(0, 1, time);		  // ��ʾҺ����
}
/* ����ˢ�º�����ops-ˢ��ѡ�Ϊ0ʱֻ�����ڱ仯��ˢ�£���0������ˢ�� */
void RefreshTime()
{
	uint8 pdata time[12];
	static uint8 backup = 0;
	if((backup!=CurTime.day) || (ops!=0)) // ������ڱ仯��ˢ�±�־λ��1������ˢ��
	{
		date[0] = ((CurTime.year>>12) & 0x0f) + '0'; // ���ǧλ
		date[1] = ((CurTime.year>>8) & 0x0f) + '0';	 // ��ݰ�λ
		date[2] = ((CurTime.year>>4) & 0x0f) + '0';	 // ���ʮλ
		date[3] = (CurTime.year & 0x0f)	+ '0';		// ��ݸ�λ
		date[4] = '-';                              // �ָ���
		date[5] = (CurTime.mon >> 4)+ '0';          // �·�ʮλ
		date[6]	= (CurTime.mon & 0x0f) + '0';       // �·ݸ�λ
		date[7] = '-';                              // �ָ���
		date[8] = (CurTime.day >> 4) + '0';         // ����ʮλ
		date[9] = (CurTime.day & 0x0f) + '0';       // ���ڸ�λ
		date[10] = '\0';                            // �ַ���������
		LCD1602_Dis_Str(0, 0, date);				// ��ʾ��Һ����
		LCD1602_Dis_Str(12, 0, week[CurTime.week]); // ��ʾ����
		backup = CurTime.day;                       // ˢ���ϴ�����ֵ
	}
}
/* ˢ������ */
void RefreshLunar()
{
	uint8 pdata date[12];
	GetRealTime(&CurTime);	// ��ȡ��ǰ����/ʱ��
	Conversion(0, CurTime.year, CurTime.mon, CurTime.day);  // ���ݹ����������
	date[0] = 0x02 + '0';  // ���ǧλ
	date[1]	= 0x00 + '0';  // ��ݰ�λ
	date[2]	= ((SpDate.year>>4) & 0x0f) + '0'; // ���ʮλ
	date[3] = (SpDate.year & 0x0f) + '0';	   // ��ݸ�λ
	date[4] = '-';                             // �ָ���
	date[5] = (SpDate.mon>>4) + '0';           // �·�ʮλ
	date[6] = (SpDate.mon & 0x0f) + '0';	   // �·ݸ�λ
	date[7] = '-';							   // �ָ���
	date[8] = (SpDate.day>>4) + '0';		   // ����ʮλ
	date[9] = (SpDate.day & 0x0f) + '0';	   // ���ڸ�λ
	date[10] = '\0';							// �ַ���������
	LCD1602_Dis_Str(0, 0, "Lunar");             // �����ַ�
	LCD1602_Dis_Str(6, 0, date);			    // ��ʾ��Һ����
}
/* ˢ�����ӵ���ʾ */
void RefreshAlarm()
{
	uint8 pdata str[6];

	str[0] = (AlarmHour>>4) + '0';				 // ����Сʱ��
	str[1] = (AlarmHour&0x0f)+'0';
	str[2] = ':';
	str[3] = (AlarmMin>>4) + '0';				 // ���ӷ�����
	str[4] = (AlarmMin&0x0f)+'0';
	str[5] = '\0';
	LCD1602_Dis_Str(0, 1, "Alarm ");             // ��ʾ��ʾ����
	LCD1602_Dis_Str(13, 1, AlarmSta[AlarmOn]); 	 // ��ʾ����״̬
	LCD1602_Dis_Str(6, 1, str);                  // ��ʾ��Һ����
}
/* ���Ӽ�غ������ִ��趨������ʱ��ʱִ������ */
void AlarmMonitor()
{
	if((CurTime.hour == AlarmHour) && (CurTime.min == AlarmMin))  //���ʱ��ƥ��
	{
		if(!AlarmOn)
			staBuzzer(1);  // ����������
		else
			staBuzzer(0);  // ������ֹͣ����
	}
	else
	{
		staBuzzer(0);	  // ʱ�䲻ƥ��ʱ��������ֹͣ����
	}
}
/* S2�������ܺ�����ʱ�����������ʱ������ƶ�����ͷ���ۻ� */
void SetShift()
{
	if(staSystem == E_SET_TIME)
	{
		switch (SetIndex)
		{
			case 0: SetIndex=1; LCD1602_Set_Cursor(7, 1); break; // ���λ�����Ӹ�λ��
			case 1: SetIndex=2; LCD1602_Set_Cursor(4, 1); break; // ���λ�����Ӹ�λ��
			case 2: SetIndex=3; LCD1602_Set_Cursor(1, 1); break; // ���λ�����Ӹ�λ��
			case 3: SetIndex=4; LCD1602_Set_Cursor(12, 1); break; // ���λ�����Ӹ�λ��
			case 4: SetIndex=5; LCD1602_Set_Cursor(9, 0); break; // ���λ�����Ӹ�λ��
			case 5: SetIndex=6; LCD1602_Set_Cursor(6, 0); break; // ���λ�����Ӹ�λ��
			default: SetIndex=0; LCD1602_Set_Cursor(3, 0);break; // ���λ���괦
		}
	}
	else if(staSystem == E_SET_ALARM)  // �������ӹ��
	{
		switch(SetIndex)
		{
			case 0:SetIndex=1; LCD1602_Set_Cursor(13, 1); break;  // ���λ�����ӿ��ش�
			case 1:SetIndex=2; LCD1602_Set_Cursor(10, 1); break;  // ���λ�����ӷ��Ӵ�
			default: SetIndex=0; LCD1602_Set_Cursor(7, 1); break; // ���λ������Сʱ��
		}
	}
	else if(staSystem == E_NORMAL) // ��������״̬�£��л�Ϊ��ʾ����/����
	{
		staSystem = Dis_Alarm;       // ��ϵͳ״̬����Ϊ��ʾ����/����
		LCD1602_Clear_Screen();      // LCD1602����
		RefreshLunar();				 // ��ʾ����
		RefreshAlarm();              // ��ʾ����
	}
	else if(staSystem == Dis_Alarm)
	{
		staSystem = E_NORMAL;          // ��ϵͳ״̬����Ϊ������ʾ
		LCD1602_Clear_Screen();        // LCD1602����
		RefreshTime();                 // ˢ��ʱ��
		RefreshDate(1);				   // ����ˢ������
		RefreshTemp(1);				   // ����ˢ���¶�
	}
}
/* C5�������ܺ������л�ϵͳ����״̬ */
void SwitchSystemSta()
{
	if (staSystem = E_NORMAL)
	{
		staSystem = E_SET_TIME;   // ϵͳ״̬����Ϊ����ʱ��
		SetIndex = 0;             // ����������õ�������
		SetShift();               // ��ʾ���λ��
		LCD1602_Open_Cursor();    // LCD1602�򿪹����ʾ
	}
	else if(staSystem == Dis_Alarm)
	{
		staSystem = E_SET_ALARM;    // ϵͳ״̬����Ϊ����
		SetIndex = 0;               // ����������õ�������
		SetShift();                 // ��ʾ���λ��
		LCD1602_Open_Cursor();      // LCD1602�򿪹����ʾ
	}
	else if(staSystem == E_SET_TIME) // ���ù���/ʱ�ӵ�״̬��
	{
		staSystem = E_NORMAL;       //ϵͳ״̬����Ϊ�������У���ʾ����/ʱ�ӡ��¶ȡ�
		SetRealTime(&CurTime);		// ���趨ʱ��д��ʵʱʱ��
		LCD1602_Close_Cursor();     // LCD1602�رչ����ʾ
		LCD1602_Clear_Screen();     // LCD1602����
		RefreshTime();              // ˢ��ʱ��
		RefreshDate(1);				// ����ˢ������
		RefreshTemp(1);				// ����ˢ���¶�
	}
	else							 //�������ӵ�״̬��
	{
		staSystem = E_NORMAL;		 //ϵͳ״̬����Ϊ�������У���ʾ����/ʱ�ӡ��¶ȡ�
	    RefreshAlarm();  			 //д�������趨ֵ
        LCD1602_Close_Cursor();		 //LCD1602�رչ����ʾ
        LCD1602_Clear_Screen();		 //LCD1602����
		RefreshTime();			  	 //ˢ��ʱ��
		RefreshDate(1);				 //����ˢ������
		RefreshTemp(1);				 //����ˢ���¶�
	}
}
/*S3�������ܺ��������ݼ�*/
void SetDtate_Alarm_S3()
{
	if (staSystem == E_SET_TIME) //���ù���/ʱ��
    {
		switch (SetIndex)//�����λ��
        {				
			case 1:	SetDat = BCD_DEC(CurTime.sec);//��BCD��תΪʮ����
					if (SetDat < 59) SetDat++; else SetDat = 0;//�����������Ӳ�����59�룬������ص�0
					CurTime.sec = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(6, 1, (CurTime.sec >> 4) + '0');
					LCD1602_Dis_OneChar(7, 1, (CurTime.sec & 0x0f) + '0');
					LCD1602_Set_Cursor(7, 1);//��걣��ԭλ
					break;

			case 2:	SetDat = BCD_DEC(CurTime.min);//��BCD��תΪʮ����
					if (SetDat < 59) SetDat++; else SetDat = 0;//�������÷��Ӳ�����59�֣�������ص�0
					CurTime.min = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(3, 1, (CurTime.min >> 4) + '0');
					LCD1602_Dis_OneChar(4, 1, (CurTime.min & 0x0f) + '0');
					LCD1602_Set_Cursor(4, 1);//��걣��ԭλ
					break;

			case 3:	SetDat = BCD_DEC(CurTime.hour);//��BCD��תΪʮ����
					if (SetDat < 23) SetDat++; else SetDat = 0;//��������Сʱ������23ʱ��������ص�0
					CurTime.hour = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(0, 1, (CurTime.hour >> 4) + '0');
					LCD1602_Dis_OneChar(1, 1, (CurTime.hour & 0x0f) + '0');
					LCD1602_Set_Cursor(1, 1);//��걣��ԭλ
					break;

			case 4: //���Ʋ����������գ����������ص���һ
					if (CurTime.week < 7) CurTime.week++; else CurTime.week = 1;
					LCD1602_Dis_Str(12, 0, week[CurTime.week]);  //��ʾ��Һ����;
					LCD1602_Set_Cursor(12, 0);//��걣��ԭλ
					break;

			case 5:	SetDat = BCD_DEC(CurTime.day);//��BCD��תΪʮ����
					if (SetDat < 31) SetDat++; else SetDat = 1;//�����������ڲ�����31�գ�������ص�1
					CurTime.day = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(8, 0, (CurTime.day >> 4) + '0');
					LCD1602_Dis_OneChar(9, 0, (CurTime.day & 0x0f) + '0');
					LCD1602_Set_Cursor(9, 0);//��걣��ԭλ
					break;

			case 6:	SetDat = BCD_DEC(CurTime.mon);//��BCD��תΪʮ����
					if (SetDat < 12) SetDat++; else SetDat = 1;//���������·ݲ�����12�£�������ص�1
					CurTime.mon = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(5, 0, (CurTime.mon >> 4) + '0');
					LCD1602_Dis_OneChar(6, 0, (CurTime.mon & 0x0f) + '0');
					LCD1602_Set_Cursor(6, 0);//��걣��ԭλ
					break;

			case 0:	SetDat = BCD_DEC(CurTime.year);//��BCD��תΪʮ����
					if (SetDat < 99) SetDat++; else SetDat = 0;//���������겻����99�꣬������ص�0
					CurTime.year = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(2, 0, (CurTime.year >> 4) + '0');
					LCD1602_Dis_OneChar(3, 0, (CurTime.year & 0x0f) + '0');
					LCD1602_Set_Cursor(3, 0);//��걣��ԭλ
					break;
		}	
	}
	else if (staSystem == E_SET_ALARM)//��������
	{
		switch (SetIndex)//�����λ��
        {
			case 1:	AlarmOn = ~AlarmOn;
					LCD1602_Dis_Str(13, 1, AlarmSta[AlarmOn]);//��ʾ����״̬ 
					LCD1602_Set_Cursor(13, 1);//��걣��ԭλ
					break;
								
			case 2:	SetDat = BCD_DEC(AlarmMin);//��BCD��תΪʮ����
					if (SetDat < 59) SetDat++; else SetDat = 0;//�������÷��Ӳ�����59�֣�������ص�0
					AlarmMin = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(9, 1, (AlarmMin >> 4) + '0');
					LCD1602_Dis_OneChar(10, 1, (AlarmMin & 0x0f) + '0');
					LCD1602_Set_Cursor(10, 1);//��걣��ԭλ
					break;

			case 0: SetDat = BCD_DEC(AlarmHour);//��BCD��תΪʮ����
					if (SetDat < 23) SetDat++; else SetDat = 0;//��������Сʱ������23ʱ��������ص�0
					AlarmHour = DEC_BCD(SetDat);//��ʮ����תΪBCD�� 
					LCD1602_Dis_OneChar(6, 1, (AlarmHour >> 4) + '0');
					LCD1602_Dis_OneChar(7, 1, (AlarmHour & 0x0f) + '0');
					LCD1602_Set_Cursor(7, 1);//��걣��ԭλ
					break;
		}		
	}	
}
/*S4�������ܺ��������ݼ�*/
void SetDtate_Alarm_S4()
{
	if (staSystem == E_SET_TIME) //���ù���/ʱ��
    {
		switch (SetIndex) //�����λ��
        {				
			case 1:	SetDat = BCD_DEC(CurTime.sec);//��BCD��תΪʮ����
					if (SetDat != 0) SetDat--; else SetDat = 59;//�����������Ӳ�Ϊ0ʱ��1��Ϊ0ʱ�ص�59
					CurTime.sec = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(6, 1, (CurTime.sec >> 4) + '0');
					LCD1602_Dis_OneChar(7, 1, (CurTime.sec & 0x0f) + '0');
					LCD1602_Set_Cursor(7, 1);//��걣��ԭλ
					break;

			case 2:	SetDat = BCD_DEC(CurTime.min);//��BCD��תΪʮ����
					if (SetDat != 0) SetDat--; else SetDat = 59;//�������÷��Ӳ�Ϊ0ʱ��1��Ϊ0ʱ�ص�59
					CurTime.min = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(3, 1, (CurTime.min >> 4) + '0');
					LCD1602_Dis_OneChar(4, 1, (CurTime.min & 0x0f) + '0');
					LCD1602_Set_Cursor(4, 1);//��걣��ԭλ
					break;

			case 3:	SetDat = BCD_DEC(CurTime.hour);//��BCD��תΪʮ����
					if (SetDat != 0) SetDat--; else SetDat = 23;//��������Сʱ��Ϊ0ʱ��1��Ϊ0ʱ�ص�23
					CurTime.hour = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(0, 1, (CurTime.hour >> 4) + '0');
					LCD1602_Dis_OneChar(1, 1, (CurTime.hour & 0x0f) + '0');
					LCD1602_Set_Cursor(1, 1);//��걣��ԭλ
					break;

			case 4: //���Ϊ��ǰΪ����һʱ���ص�������
					if (CurTime.week != 1) CurTime.week--; else CurTime.week = 7;
					LCD1602_Dis_Str(12, 0, week[CurTime.week]);  //��ʾ��Һ����;
					LCD1602_Set_Cursor(12, 0);//��걣��ԭλ
					break;

			case 5:	SetDat = BCD_DEC(CurTime.day);//��BCD��תΪʮ����
					if (SetDat != 1) SetDat--; else SetDat = 31;//�����������ڲ�Ϊ1ʱ��1��Ϊ0ʱ�ص�31
					CurTime.day = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(8, 0, (CurTime.day >> 4) + '0');
					LCD1602_Dis_OneChar(9, 0, (CurTime.day & 0x0f) + '0');
					LCD1602_Set_Cursor(9, 0);//��걣��ԭλ
					break;

			case 6:	SetDat = BCD_DEC(CurTime.mon);//��BCD��תΪʮ����
					if (SetDat != 1) SetDat--; else SetDat = 12;//���������·ݲ�Ϊ1ʱ��1��Ϊ0ʱ�ص�12
					CurTime.mon = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(5, 0, (CurTime.mon >> 4) + '0');
					LCD1602_Dis_OneChar(6, 0, (CurTime.mon & 0x0f) + '0');
					LCD1602_Set_Cursor(6, 0);//��걣��ԭλ
					break;

			case 0:	SetDat = BCD_DEC(CurTime.year);//��BCD��תΪʮ����
					if (SetDat != 0) SetDat--; else SetDat = 99;//���������겻Ϊ0ʱ��1��Ϊ0ʱ�ص�99
					CurTime.year = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(2, 0, (CurTime.year >> 4) + '0');
					LCD1602_Dis_OneChar(3, 0, (CurTime.year & 0x0f) + '0');
					LCD1602_Set_Cursor(3, 0);//��걣��ԭλ
					break;
		}		
	}
	else if (staSystem == E_SET_ALARM) //��������
	{
		switch (SetIndex) //�����λ��
        {
			case 1:	AlarmOn = ~AlarmOn;
					LCD1602_Dis_Str(13, 1, AlarmSta[AlarmOn]);//��ʾ����״̬ 
					LCD1602_Set_Cursor(13, 1);//��걣��ԭλ
					break;

			case 2:	SetDat = BCD_DEC(AlarmMin);//��BCD��תΪʮ����
					if (SetDat != 0) SetDat--; else SetDat = 59;//�������÷��Ӳ�Ϊ0ʱ��1��Ϊ0ʱ�ص�59
					AlarmMin = DEC_BCD(SetDat);//��ʮ����תΪBCD�� 
					LCD1602_Dis_OneChar(9, 1, (AlarmMin >> 4) + '0');
					LCD1602_Dis_OneChar(10, 1, (AlarmMin & 0x0f) + '0');
					LCD1602_Set_Cursor(10, 1);//��걣��ԭλ
					break;

			case 0: SetDat = BCD_DEC(AlarmHour);//��BCD��תΪʮ����
					if (SetDat != 0) SetDat--; else SetDat = 23;//��������Сʱ��Ϊ0ʱ��1��Ϊ0ʱ�ص�59
					AlarmHour = DEC_BCD(SetDat);//��ʮ����תΪBCD��
					LCD1602_Dis_OneChar(6, 1, (AlarmHour >> 4) + '0');
					LCD1602_Dis_OneChar(7, 1, (AlarmHour & 0x0f) + '0');
					LCD1602_Set_Cursor(7, 1);//��걣��ԭλ
					break;
		}		
	}	
}


/*��������*/
void KeyAction(uint8 keycode)
{
	switch (keycode)
	{
		case 0x0d: SetShift();          break;	//����S2,��ʾ�л�����������״̬��Ϊ�������Ƽ�
        case 0x26: SetDtate_Alarm_S3(); break;	//����S3,���ݼ�
        case 0x28: SetDtate_Alarm_S4(); break;	//����S4,���ݼ�
        case 0x1b: SwitchSystemSta();   break;	//����S5,���ü�
		default: break;
	}
}