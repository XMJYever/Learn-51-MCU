#include <reg52.h>
sbit DIO = P3^4;   // 串行数据口
sbit S_CLK = P3^5; // 移位寄存器时钟
sbit R_CLK = P3^6; // 输出锁存器时钟

void main()
{
	// row 0x80 ，COL 0xfe。点亮第一个点
	// 串行输入
	unsigned char i, dat;
	dat = 0xfe;	 // 列项的值
	for(i=0;i<8;i++)
	{
		S_CLK = 0;
		R_CLK = 0;
		if(dat & 0x01)	 // 判断第1位为1
			DIO = 1;
		else
			DIO = 0;
		S_CLK = 1;  // 产生上升沿，移位至寄存器中
		dat >>= 1;  // 右移1位，发送数据	
	}		

	dat = 0x80;
	for(i=0;i<8;i++)
	{
		S_CLK = 0;
		R_CLK = 0;
		if(dat & 0x01)	 // 判断是否为第8位
			DIO = 1;
		else
			DIO = 0;
		S_CLK = 1;  // 产生上升沿，移位至寄存器中
		dat >>= 1;  // 右移1位，发送数据	
	}
	R_CLK = 1;  // 将所有数据输出
	while(1);   // 让程序保持
}
