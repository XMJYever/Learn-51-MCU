#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int

// 定义管脚
sbit YELLOW_A = P0^0;	// 东西向
sbit RED_A = P0^1;
sbit GREEN_A = P0^2;
sbit YELLOW_B = P0^3;	// 南北向
sbit RED_B = P0^4;
sbit GREEN_B = P0^5;

uchar Flash_Count = 0, Operation_Type=1; // 闪烁次数，操作类型变量

// 延时
void DelayMs(uint x)
{
	uchar i;
	while(x--)
		for(i=0; i<114; i++);
}
void Traffic_Light()
{
	switch(Operation_Type)
	{
		case 1:
			RED_A = 1;YELLOW_A = 1;GREEN_A=0;
			RED_B = 0;YELLOW_B = 1;GREEN_B=1;
			DelayMs(2000);
			Operation_Type=2;
			break;
		case 2:
			DelayMs(300);
			YELLOW_A=~YELLOW_A,GREEN_A=1;
			if(++Flash_Count!=10)
				return;
			Flash_Count=0;
			Operation_Type=3;
			break;
		case 3:
			RED_A=0;YELLOW_A=1;GREEN_A=1;
			RED_B=1;YELLOW_B=1,GREEN_B=0;
			DelayMs(2000);
			Operation_Type=4;
			break;
		case 4:
			DelayMs(300);
			YELLOW_B=~YELLOW_B;GREEN_B=1;
			if(++Flash_Count!=10)
				return;
			Flash_Count=0;
			Operation_Type=1;
	}								 
}

void main()
{
	while(1)
		Traffic_Light();
}