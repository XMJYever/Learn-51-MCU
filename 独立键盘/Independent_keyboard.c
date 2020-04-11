#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2^6; // 数码管段选
sbit WE = P2^7; // 数码管位选

sbit key_s2 = P3^0;
sbit key_s3 = P3^1;

uchar number = 0;
// 共阴极数码管编码表
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

// 延时15ms
void delay(void)   //误差 -0.000000000003us
{
    unsigned char a,b;
    for(b=51;b>0;b--)
        for(a=134;a>0;a--);
}

void main()
{
	WE = 1; // 打开位选锁存器
	P0 = 0xfe; // 1111 1110
	WE = 0;

	while(1)
	{
		if(key_s2 == 0)	// 键盘2被按下
		{
			delay(); //按键消抖
			number++;
			if(number == 10)
				number = 0;
			while(!key_s2); // 松手检测
		}
		DU = 1; // 打开段选锁存器
		P0 = table[number]; // 0000 0110
		DU = 0; // 锁存段选数据
		
		if(key_s3 == 0)	// 键盘3被按下
		{
			delay(); //按键消抖
			if(number > 0)
				number--;
			while(!key_s3); // 松手检测
		}
		DU = 1; // 打开段选锁存器
		P0 = table[number]; // 0000 0110
		DU = 0; // 锁存段选数据
	}
}