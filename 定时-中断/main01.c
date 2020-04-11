#include <reg52.h>

#define uint unsigned int
#define uchar unsigned char

extern void load_smg();

// 定时器0初始化
void time0_Init()
{
	TMOD |= 0x01; // 选定定时器0，选择模式为0，1
	TH0 = 60928/256;
	TL0 = 60928%256;
	TR0 = 1; // 允许定时器0工作
}

void ISR_Init()
{
	EA = 1; //打开总中断
	ET0 = 1; //	打开定时器0中断
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