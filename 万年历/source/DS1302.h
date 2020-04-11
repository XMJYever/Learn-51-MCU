#ifndef _DS1302_H_
#define _DS1302_H_

struct sTime{
	uint16 year; // ��
	uint8 mon;   // ��
	uint8 day;   // ��
	uint8 hour;  // ʱ
	uint8 min;   // ��
	uint8 sec;   // ��
	uint8 week;  // ����
};

void Init();  // ��ʼ��DS1302��������Ĭ��ʱ��
void GetRealTime(struct *time);	// ��ȡDS1302ʱ����������
void SetRealTime(struct *time); // ����DS1302ʱ����������

#endif