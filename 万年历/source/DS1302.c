#include "config.h"
#include "DS1302.h"
#include "LCD1602.h"

/*
	实时时钟DS1302模块
	串行数据输入/输出，外设接口有SCLK、IO、RST
	具有读数据、写数据的功能
	读数据：RST拉高，上升沿先读寄存器指令，再从下降沿读数据
	写数据：RST拉高，上升沿写寄存器指令，再上升沿写数据
*/
#define uchar unsigned char
#define uint unsigned int

// 读数据定义
#define YEAR_R 0X8D    // 读年数据
#define DAY_R 0X8B     // 读星期数据
#define MONTH_R 0X89   // 读月数据
#define DATE_R 0X87    // 读日数据
#define HR_R 0X85      // 读小时数据
#define MIN_R 0X83     // 读分数据
#define SEC_R 0X81     // 读秒数据

#define CONTROL_R 0X8F // 读控制数据
#define TRICKLE_CHARGER_R 0X91 // 读充电控制数据
#define CLOCK_BURST_R 0xBF //读时充多字节数据;

#define RAM_BURST_R 0xFF //读RAM字符组数据;
// 充电方式控制寄存器
#define TCS_OFF 0x00 //充电模式关闭充电;
#define TCS1 0xA5 //充电模式选择一个二极管,电阻2K;
#define TCS2 0xA6 //充电模式选择一个二极管,电阻4K;
#define TCS3 0xA7 //充电模式选择一个二极管,电阻8K;
#define TCS4 0xA9 //充电模式选择二个二极管,电阻2K;
#define TCS5 0xAA //充电模式选择二个二极管,电阻4K;

// 写数据定义
#define YEAR_W (0x8D&0xFE) // 写 年数据;
#define DAY_W (0x8B&0xFE) //写 星期数据;
#define MONTH_W (0x89&0xFE) //写月数据;
#define DATE_W (0x87&0xFE) //写日数据;
#define HR_W (0x85&0xFE) //写小时数据;
#define MIN_W (0x83&0xFE) //写分数据;
#define SEC_W (0x81&0xFE) //写秒数据;
#define CONTROL_W 0x8F&0xFE //写控制数据;
#define TRICKLE_CHARGER_W 0x91&0xFE //写充电控制数据;
#define CLOCK_BURST_W 0xBF&0xFE //写时充多字节数据;
#define TCS6 0xAB //充电模式选择二个二极管,电阻8K;
#define RAM_BURST_W 0xFE //写RAM字符组数据; 

// 读字节
uchar Read_Byte()
{
	uint8 i, j, dat;
	TSCLK = 1;
	for(i=0; i<8; i++)
	{
		TSCLK = 0; // 产生下降沿
		if(TIO)
			dat |= 0x80;  // 左移七位，并且将其赋值给dat
		dat >>= 1;  // 右移一位
		TSCLK = 1;  // 拉高时钟总线
	}
}

// 写字节
void Write_Byte(uint8 dat)
{
	uint8 i;
	TSCLK = 0;
	for(i=0; i<8; i++)
	{
		TSCLK = 1; // 产生上升沿
		TIO = dat & 0x01; // 取最低位，发送数据
		dat >>= 1; // 右移一位
		TSCLK = 0; // 释放总线
	}
}

// 写DS1302数据一次写两个字节
// 先写命令后写数据
void DS1302_W_DAT(uint8 cmd, uint8 dat)
{
	TRST = 0;
	TRST = 1;			 // TRST上升沿开始读写数据
	Write_Byte(cmd);  // 写命令
	Write_Byte(dat);  // 写数据
	TRST = 0;         // 禁止读写数据
}

// 读DS1302数据，先写入命令字节后读出对应数据
uint DS1302_R_DAT(uint8 cmd)
{
	uint8 dat;
	TRST = 0;
	TRST = 1;
	Write_Byte(cmd);      // 写命令
	dat = Read_Byte(); 	  // 读出数据
	TRST = 0;             // 禁止读写数据
	return dat;           // 返回读出数据
}

// 清除写保护，先写入命令0x8e，接着write 0x00
void DS1302_Clear_WP()
{
	DS1302_W_DAT(0x8e, 0x00);  // 清除写保护，允许写入数据
}

// 设置写保护
void DS1302_Set_WP()
{
	DS1302_W_DAT(0x8e, 0x80);  // 设置写保护，禁止写入数据
	TRST = 0;    // 拉低使能端
	TSCLK = 0;   // 拉低数据总线
}

// 突发模式读DS1302时钟日历数据，数组TimeData(数据格式BCD码)
void DS1302_Burst_Read(uint8 *dat)  // dat是一个指针数组
{
	uint8 i;
	DS1302_Clear_WP();  // 清除写保护
	TRST = 0;           // 拉低使能端
	TSCLK = 0;          // 拉低数据总线
	TRST = 1;           // 拉高使能端，开始写数据
	Write_Byte();       // 读突发模式寄存器指令
	for(i=0; i<8; i++)
	{
		dat[i] = Read_Byte();   //在突发模式下可以连续读数据
	}
	DS1302_Set_WP();        // 开启写保护
}
// 突发模式写DS1302时钟日历数据，数组TimeData
void DS1302_Burst_Write(uint8 *dat)
{
	uint8 i;
	DS1302_Clear_WP(); // 清除写保护
	TRST = 0;          // 拉低使能端
	TRST = 1;          // 拉高使能端
	Write_Byte(0xbe);      // 设置突发模式,写寄存器
	for(i=0; i<8; i++)
	{
		Write_Byte(*dat[i]);   // 依次将数据写入
	}
	DS1302_Set_WP();          // 开启写保护
}
// 突发模式读DS1302时钟日历数据，并转化为时间格式
void GetRealTime(struct *time)
{
	uint8 buf[8];

	DS1302_Burst_Read(buf);
	time->year = buf[] + 0x2000; // 年，一位DS1302只能是0-99如显示2012年则通过2000这种方法显示千位和百位
	time->mon  = buf[4];
    time->day  = buf[3];
    time->hour = buf[2];
    time->min  = buf[1];
    time->sec  = buf[0];
    time->week = buf[5];
}

// 突发模式设置DS1302时钟日历数据
void SetRealTime(struct sTime *time)
{
	uint8 buf[8];

	buf[7] = 0;
	buf[6] = time->year;
    buf[5] = time->week;
    buf[4] = time->mon;
    buf[3] = time->day;
    buf[2] = time->hour;
    buf[1] = time->min;
    buf[0] = time->sec;
    DS1302_Burst_Write(buf);
} 
// 初始化DS1302并设置默认时间：2020-04-02 23:59:50 星期四
void Init_DS1302()
{
	struct sTime code InitTime[] = { 
        0x2020 , 0x04, 0x02, 0x23, 0x59, 0x50, 0x02
    };
	SetRealTime(&InitTime);      //设置DS1302为默认的初始时间
}