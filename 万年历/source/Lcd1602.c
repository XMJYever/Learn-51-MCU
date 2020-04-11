#include "config.h"
#include "Lcd1602.h"
/* 液晶显示程序
	主要有如下几个功能：1、检测忙信号；
	2、写指令、读数据、写数据；3、坐标显示；
	4、显示一个字节数据；5、显示一个字节字符
	6、显示整个字符串 
 */

// 判断液晶忙不忙，如果忙则等待
void Read_Busy()
{
	uint8 busy;
	P0 = 0xff;
	RS = 0;	 // 读数据
	RW = 1;  // 选择读操作
	do
	{
		EN = 1;
		busy = P0;	// 读P0接口的状态字
		EN = 0;
	}while(busy & 0x80);  // 如果忙，则等待
}

// 写指令
void Write_Cmd(uint8 cmd)
{
	Read_Busy();
	RS = 0;	   // 写指令
	RW = 0;
	EN = 1;
	P0 = cmd;
	EN = 0;
}

// 写数据
void Write_Dat(uint8 dat)
{
	Read_Busy();
	RS = 1;
	RW = 0;   // 写操作
	P0 = dat;
	EN = 1;
	EN = 0;
}

// 坐标显示
void LCD1602_Set_Cursor(uint8 x,uint8 y)
{
	uint8 addr;
	if(y)
		addr = 0x40 + x;
	else
		addr = 0x00 + x;
	Write_Cmd(0x80 | addr); 
}

// 显示一个字节数据
void LCD1602_Dis_OneChar(uint8 x, uint8 y, uint8 dat)
{
	LCD1602_Set_Cursor(x, y);	// 发送指令，选定要显示的位置
	Write_Dat(dat);		// dat用来写数据
}

// LCD1602写字符串
void LCD1602_Dis_Str(uint8 x, uint8 y, uint8 *str)
{
	LCD1602_Set_Cursor(x, y);
	while(*str != '\0')
	{
		LCD1602_Write_Dat(*str++);
	}
}
// LCD1602关闭光标显示
void LCD1602_Close_Cursor()
{
	 Write_Cmd(0x80); // 关闭光标显示
}
// LCD1602打开光标显示
void LCD1602_Open_Cursor()
{
	Write_Cmd(0x0f);  // 打开光标显示，光标闪烁
}
// LCD1602清屏
void LCD1602_Clear_Screen()
{
	Write_Cmd(0x01);  // 清屏
} 
// LCD1602初始化
void Init_LCD1602()
{
	Write_Cmd(0x38); // 设置16*2显示，5*7点阵，8位数据接口
	Write_Cmd(0x0c); // 开显示，关闭光标
	Write_Cmd(0x06); // 读写一字节后地址指针加1
	Write_Cmd(0x01); // 清屏
}