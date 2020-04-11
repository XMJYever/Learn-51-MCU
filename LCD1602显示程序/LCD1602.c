#include <reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint;

// 管脚命名
sbit EN = P3^4;	   // 使能信号
sbit RW = P3^6;	   // 读/写选择端
sbit RS = P3^5;	   // 数据/命令选择段(H/L)
// 判断液晶忙不忙，如果忙则等待
void Read_Busy()
{
	uchar busy;
	P0 = 0xff;
	RS = 0;
	RW = 1;
	do
	{
		EN = 1;
		busy = P0; // 读P0接口的状态字，D7位读写操作使能
		EN = 0;	
	}while(busy & 0x80);
}
// 写LCD1602命令
void Write_Cmd(uchar cmd)
{
	Read_Busy();
	RS = 0;
	RW = 0;
	P0 = cmd;
	EN = 1;  //	传入指令给LCD
	EN = 0;  //释放
}
// 写一个字节数据
void Write_Dat(uchar dat)
{
	Read_Busy();
	RS = 1;
	RW = 0;
	P0 = dat;
	EN = 1;
	EN = 0;
}

void main()
{
	uchar i;
	uint k = 1000;
	uchar *a[] = {"Monday", "Tuesday", "Wednesday"};
	uchar *p;
	Write_Cmd(0x38);  // 设置16*2显示
	Write_Cmd(0x0f);  // 开显示
	Write_Cmd(0x01);  // 清屏
	Write_Cmd(0x07);  // 地址指针移位命令
	Write_Cmd(0x80 | 0x10);  // 显示地址
	p = a;
	for(i=0; i<3; i++)
	{
		p = a[i];
		while(*p != '\0')
			{
				 Write_Dat(*p++);  // 写数据	 当作acsii码来显示
				 while(k--);
			}
	}
	while(1);
}