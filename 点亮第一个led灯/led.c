#include <reg52.h>  // 用来引用51头文件

//sbit: 位定义	egs: sbit 标识符 = 地址值；
sbit LED1 = P0^2;  // 将LED1连接到芯片的P1.0 I/O口
//sbit LED5 = P1^4;

void main()
{
	LED1 = 0;
//	LED5 = 0;
}