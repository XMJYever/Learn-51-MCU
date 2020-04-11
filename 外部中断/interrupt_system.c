		#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2^6;//数码管段选
sbit WE = P2^7;//数码管段选
sbit key_s2 = P3^0;//独立按键S2
sbit flag = P3^7;//外部中断信号产生脚
uchar num;//数码管显示的值

//毫秒级延时函数定义
void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--); 		
} 

//外部中断1初始化
void int1Init()
{
	EA = 1;	//开总中断
	EX1 = 1; // 开外部中断1
	IT1 = 1; // 外部中断1下降沿触发
}

// 外部中断1中断服务程序
void int1() interrupt 2
{
	P1 = ~P1;  // 取反
}

void main()//main函数自身会循环
{
	int1Init(); // 初始化
	while(1)
	{
		if(key_s2 == 0)//判断S2是否被按下
		{
			delay(20);//按键消抖
			if(key_s2 == 0)
			{
				flag = 1;
				flag = 0;  // 产生了下降沿
				while(!key_s2);//松手检测
			}	
		}
	}	
}  