#ifndef _DS1302_H_
#define _DS1302_H_

struct sTime{
	uint16 year; // 年
	uint8 mon;   // 月
	uint8 day;   // 日
	uint8 hour;  // 时
	uint8 min;   // 分
	uint8 sec;   // 秒
	uint8 week;  // 星期
};

void Init();  // 初始化DS1302，并设置默认时间
void GetRealTime(struct *time);	// 获取DS1302时钟日历数据
void SetRealTime(struct *time); // 设置DS1302时钟日历数据

#endif