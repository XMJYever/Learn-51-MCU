#include <reg52.h>

void time0_Init()
{
	TMOD |= 0x01;
	TR0 = 1; // 打开定时器
	TH0 = 56320/256; // 计数起点为10ms溢出一次
	TL0 = 56320%256;
}

void time1_Init()
{

}

void ISR_Init()
{
	EA = 1; //总中断开启
	ET0 = 1; // 开启定时器中断，TF0控制位置1，表名TF0=1时，中断系统介入
	EX0 = 1; // 外部中断0
	EX1 = 0;
	ET1 = 0;
	ES = 0;  // UART中断没有开启
}

// 中断服务子程序，希望中断系统来调用，而不是在main函数中调用
void IE0_isr() interrupt 0
{

}

void TF0_isr() interrupt 1 // 由于上面进入一次，由于设置了计数起点，所以10ms溢出一次
{
	static char c;
	TH0 = 56320/256;  // 重装初值
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