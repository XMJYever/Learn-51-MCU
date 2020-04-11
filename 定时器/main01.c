#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char 

sbit led1 = P1^0;
uchar c;

void delay(void)   //误差 -0.000000000001us
{
    unsigned char a,b;
    for(b=15;b>0;b--)
        for(a=152;a>0;a--);
}

void timer0_Init()
{
	TMOD = 0X01;  // 选定相关模式
	TR0 = 1;  // 允许T0开始计数
	TH0 = 0;
	TL0 = 0;
}

void main()
{
	timer0_Init();	// 
	while(1)
	{
		if(TF0 == 1) // 检测定时器0是否溢出，每到65535次
		{
			TF0 = 0;
			c++;
			if(c == 7)
			{
				c = 0;
				led1 = ~led1;
			}
		}
	}
}