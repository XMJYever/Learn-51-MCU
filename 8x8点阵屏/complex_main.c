#include <reg52.h>
#include <intrins.h>

sbit DIO = P3^4;   // 串行数据口
sbit S_CLK = P3^5; // 移位寄存器时钟
sbit R_CLK = P3^6; // 输出锁存器时钟
unsigned char code table[2][8] = {0x58,0xAD,0x95,0xA0,0x05,0xB5,0x10,0xFF, // 班
0xFE,0x74,0xAE,0xDA,0xAA,0x06,0xDE,0xBE};
 // 0x83,0xAB,0xAB,0x00,0xAA,0xAA,0x82,0xFC//电
void Send_Byte(unsigned char dat)
{
	unsigned char i;
	S_CLK = 0;
	R_CLK = 0;
	for(i=0;i<8;i++)
	{
		if(dat & 0x01)	 // 判断第1位为1
			DIO = 1;
		else
			DIO = 0;
		S_CLK = 1;  // 产生上升沿，移位至寄存器中
		dat >>= 1;  // 右移1位，发送数据
		S_CLK = 0;  // 	
	}
}
void main()
{
	unsigned char j, k, ROW;
	unsigned int z;
	while(1)
	{
		
		for(k=0; k<2; k++)
		{
			for(z = 0; z<1000; z++)	  // 延时显示
			{
				ROW = 0x80;
				// 列选的值
				for(j = 0; j<8;j++)
				{
					Send_Byte(table[k][j]);
					Send_Byte(ROW);   //行数据
					R_CLK = 1;  // 将所有数据输出
					R_CLK = 0;  
					ROW = _cror_(ROW,1); // 循环右移
				}
			}
		}
	}
}
