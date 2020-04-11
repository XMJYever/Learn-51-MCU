#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2^6; // 数码管段选
sbit WE = P2^7; // 数码管位选

uchar number = 20;
uchar KeyValue;//按键值
// 共阴极数码管编码表
uchar code table[]={
//0   1     2    3    4    5    6    7    8
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,
//9   A     B    C    D    E   F	 H      L
0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71, 0x76, 0x38,
//n     u     -    熄灭  
0X37, 0X3e, 0X40, 0X00};

// 延时15ms
void delay(void)   //误差 -0.000000000003us
{
    unsigned char a,b;
    for(b=51;b>0;b--)
        for(a=134;a>0;a--);
}

void KeyScan()
{
	P3 = 0xf0;  // 列扫描
	if(P3 != 0xf0)	 // 判断按键是否被按下
	{
		delay(); // 按键消抖
		if(P3!=0xf0)
		{
			switch(P3) // 判断哪一列被按下
			{
				case 0xe0: KeyValue = 0; break;	//第一列被按下
				case 0xd0: KeyValue = 1; break;
				case 0xb0: KeyValue = 2; break;
				case 0x70: KeyValue = 3; break;
			}
			P3 = 0x0f;  // 行扫描
			switch(P3) // 判断哪一行被按下
			{
				case 0x0e: KeyValue = KeyValue; break;	//第一行被按下
				case 0x0d: KeyValue = KeyValue + 4; break;
				case 0x0b: KeyValue = KeyValue + 8; break;
				case 0x07: KeyValue = KeyValue + 12; break;
			}
			while(P3!=0x0f); //松手检测
		}

	   	//独立键盘
		P3 = 0xff;
		if(P3!=0xff) //被按下
		{
			delay();
			if(P3!=0xff)
			{
				switch(P3) // 判断哪一行被按下
			{
				case 0xfe: KeyValue = 16; break;  //s2被按下
				case 0xfd: KeyValue = 17; break; //s2被按下
				case 0xfb: KeyValue = 18; break;  //s2被按下
				case 0xf7: KeyValue = 19; break;  //s2被按下
			}
			while(P3!=0xff); //松手检测
			}	
		}
	}
}

void main()
{
	WE = 1; // 打开位选锁存器
	P0 = 0xfe; // 1111 1110
	WE = 0;
	while(1)
	{

		KeyScan();
		DU = 1;
		P0 = table[KeyValue];
		DU = 0;
	}
}