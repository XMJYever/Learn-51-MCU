#include <reg52.h>
#include <intrins.h>

void delay(unsigned int);
sbit beep = P2^3;

void main()
{
	P1 = 0xF0;	  // 第一个灯被点亮，初始化
	while(1)
	{
		delay(100);
		beep = 1;
		P1 = _crol_(P1, 1);
	}
}

void delay(unsigned int z)
{
	unsigned int x, y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0; y--);
}