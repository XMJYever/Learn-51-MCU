/* 此demo重在理解和实现I2C通信 
	基本指令：1、起始信号；2、结束信号；3、应答信号
	操作函数：1、读字节；2、写字节
	步骤：1、主机发送数据；2、主机接收数据
*/
#include <reg52.h>
#include <intrins.h>
#define At24c02ADDR 0XA0   //AT24C02硬件地址
// 位定义
sbit SCL = P2^1;  // 时钟线
sbit SDA = P2^0;  // 数据线
// 数据类型定义
typedef unsigned char uchar;
typedef unsigned int uint;
bit AckFlag;   // 应答状态标志位

// 延迟函数，5us
void Delay5us()
{
	_nop_();
}

/* 基本指令-起始信号 */
void Start()
{
	SCL = 1;
	SDA = 1;
	Delay5us();
	SDA = 0;
	Delay5us();
	SCL = 0;
}

/* 基本指令-结束信号 */
void Stop()
{
	SCL = 0;
	SDA = 0;
	SCL = 1;
	Delay5us();
	SDA = 1;
	Delay5us();
}
/* 基本指令-读应答 */
bit ReadACK()
{
	SCL = 0;    // 拉低时钟总线，允许从机控制SDA
	SCL = 1;    // 拉高时钟总线，读取SDA
	Delay5us();
	if(SDA)    // 未应答
	{
		SCL = 0;
		return 1;
	}
	else
	{
		SCL = 0;
		return 0;
	}
}
/* 基本指令-写应答 */
void SendACK(i)
{
	SCL = 0;  // 拉低时钟总线，允许主机写入数据
	if (i)
	{
		SDA = 0;   // 说明有数据，需要应答
	}
	else
		SDA = 1;   // 无数据，则不需要应答
	SCL = 1;  // 拉高时钟总线，允许从机接收SDA
	Delay5us();
	SCL = 0;     // 拉低时钟总线，允许SDA释放总线
	SDA = 1;   // 释放数据总线
}
/* 读字节,从高位开始读 */
uchar Read_Byte()
{
	uchar i, dat;
	SCL = 1;  // 拉高时钟总线
	for (int i = 0; i < 8; ++i)
	{
		SCL = 0;  // 拉低时钟总线，允许从机控制SDA
		SCL = 1;  // 拉高时钟总线，保持数据
		if (SDA)
		{
			dat |= 0x01;
		}
		dat <<= 1;  // 左移一位
	}
	return dat;
}
/* 写字节，从高位开始写 */
void Write_Byte(uchar dat)
{
	uchar i;
	SCL = 1;
	for (int i = 0; i < 8; ++i)
	{
		SCL = 0; // 拉低时钟总线，允许写数据
		SDA = dat & 0x80;  // 写入最高位
		SCL = 1; // 拉高时钟总线，允许数据传给从机
		dat <<= 1;  // 左移一位
	}
	SCL = 0; //拉低时钟总线，允许SDA释放
	SDA = 1; //释放数据总线
}
/* 发送数据:1、开始信号；2、从机地址+0；3、应答；4、单元地址0-255，应答;5、数据、应答；6、结束信号 */
void SendData(uchar addr, uchar dat)
{
	Start();
	Write_Byte(At24c02ADDR + 0);  // 从机地址+方向(写)
	if(ReadACK())
		AckFlag = 1;  // 未应答
	else
		AckFlag = 0;  //应答
	Write_Byte(addr);
	if (ReadACK())
		AckFlag = 1;
	else	
		AckFlag = 0;
	Write_Byte(dat);
	if (ReadACK())
		AckFlag = 1;
	else	
		AckFlag = 0;
	Stop();
}
/* 读数据：1、开始信号；2、从机地址+1；3、应答；4、单元地址0-255，应答;5、数据、应答；6、结束信号 */
uchar ReadData(uchar addr)
{
	uchar dat;
	Start();
	Write_Byte(At24c02ADDR + 0);
	if(ReadACK())
		AckFlag = 1;  // 未应答
	else
		AckFlag = 0;  //应答
	Write_Byte(addr);
	if(ReadACK())
		AckFlag = 1;  // 未应答
	else
		AckFlag = 0;  //应答
	dat = Read_Byte();
	SendACK(1);  // 主机发送非应答
	Stop();
	return dat;
}

void main(int argc, char const *argv[])
{
	time0Init();  // 定时器0初始化
	EA = 0;       // 屏蔽中断
	SendData(3, 188);  // 给第3单元写入数据“188”
	_nop_();  // 延时等待ATC02处理
	num = ReadData(3);  // 从地址3读取数据给显示变量
	if (AckFlag)
		P1 = 0;  // P1灯亮
	else	
		P1 = 1;  // 灭P1灯
	EA = 1;    // 开中断
	while(1);
}