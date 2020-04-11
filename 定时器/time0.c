#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

sbit wela = P2^7; // 位选锁存器
sbit dula = P2^6; // 段选锁存器

// 共阴极数码管
uchar table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void delay(uint m)
{
	uint i,j;
	for(i = m; i > 0; i--)
		for(j = 114; j > 0 ; j--); 		
} 

void display(uchar number)
{
	uchar x, y, z;
	x = number/100;	// 百位
	y = number%100 /10; // 十位
	z = number % 10; // 个位

	// 打开位选锁存器, 百位
	P0 = 0xff; // 清除断码
	wela = 1;
	P0 = 0xfe; // 1111 1110
	wela = 0;

	// 打开段选锁存器
	dula = 1;
	P0 = table[x];
	dula = 0; // 关闭段选锁存器
	delay(5);

	// 打开位选锁存器, 十位
	P0 = 0xff; // 清除数码管
	wela = 1;
	P0 = 0xfd; // 1111 1101
	wela = 0;

	// 打开段选锁存器
	dula = 1;
	P0 = table[y];
	dula = 0; // 关闭段选锁存器
	delay(5);

	// 打开位选锁存器, 个位
	P0 = 0xff; // 清除数码管
	wela = 1;
	P0 = 0xfb; // 1111 1011
	wela = 0;

	// 打开段选锁存器
	dula = 1;
	P0 = table[z];
	dula = 0; // 关闭段选锁存器
   	delay(5);
}

// 定时器0初始化
void timer0Init()
{
	TR0 = 1; // 定时器0的运行控制位，置1就允许T0开始计数
	TMOD = 0x01; // 定时器工作模式1，16位定时器计数模式
	TH0 = 0x4b; // 高8位
	TL0 = 0xfd; // 低8位，定时50ms	
}

void main()
{
	uchar mSec, Sec; // 毫秒储存变量
	timer0Init(); // 定时器0初始化
	while(1)
	{
		if(TF0); //	溢出中断标志
		{
			TF0 = 0; // 软件清零溢出标志位
			TH0 = 0x4b; // 高8位
			TL0 = 0xfd; // 低8位，定时50ms
			mSec++; // 50ms到
			if(mSec == 20)
			{
				mSec = 0;
				Sec++; // 1s时间到
				if(Sec == 10)
					Sec = 0; // 秒清零
			}
		}
		display(Sec);  // 数码管显示函数
		
	}
}