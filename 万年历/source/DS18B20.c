#include "DS18B20.h"
#include "config.h"
/* 温度传感器DS18B20遵循单总线通信协议
	步骤：1、初始化；2、ROM操作指令；3、操作功能指令
 */
// 延时函数，延时10us
void Delay_us(uint8 us)   //误差 -0.234375us
{
    uint8 i,a;
	for(i=0;i<us;i++)
    	for(a=3;a>0;a--);
}

// 写时序
void Write_Byte(uchar dat)
{
	uint8 i;
	EA = 0;       // 禁止总中断
	for(i=0; i<8; i++)
	{
		DS18B20_IO = 0; // 写时序
		_nop_(); // 产生时序
		DS18B20_IO = dat & 0x01; // 取最低位
		Delay_us(65); 	// 延时60us
		DS18B20_IO = 1; // 释放总线
		_nop_();
		dat >>= 1; // 右移一位		
	}
	EA = 1;   // 使能总中断
}

// 读时序
uchar Read_Byte()
{
	uint8 i, j, dat;
	EA = 0;       // 禁止总中断
	for(i=0; i<8; i++)
	{
		DS18B20_IO = 1;
		_nop_();
		DS18B20_IO = 0;
		_nop_();  // 保持至少1us
		j = DS18B20_IO;  // 记录读取的数据
		Delay_us(70); // 延时70us
		dat = (j>>7)|(dat<<1); //从低位读到高位
	}
	DS18B20_IO = 1; // 释放总线
	return dat;
}
// 初始化DS18B20,并获取存在脉冲
bit GetDS18B20Ack()
{
	bit i;
	EA = 0;         // 禁止总中断
	DS18B20_IO = 1; // 单总线
	_nop_(); // 代表运行一个机器周期，1.08us
	// 发送复位脉冲
	DS18B20_IO = 0; // 拉低电平
	Delay_us(500);	// 延时500us，大于480us
	DS18B20_IO = 1; // 释放总线，拉高电平，发送脉冲
	Delay_us(50); // 延时50us

	// 等待从机发送存在脉冲
	i = DS18B20_IO;     // 接收到从机发来的信号
	while(!DS18B20_IO);	// 等待存在脉冲结束
	Delay_us(100); //延时100us
	_nop_();
	EA = 1;  // 使能总中断

	return (i);
}
// 启动一次18B20温度转换，返回值-表示是否启动成功。
// 1、初始化，2、ROM指令、3功能指令
bit	Start18B20()
{
	bit ack;
	// 初始化
	ack = GetDS18B20Ack();
	// ROM指令
	if(ack == 0)	// 存在脉冲为低电平
	{
		Write_Byte(0xcc);   // 跳过ROM操作
		Write_Byte()0x44;   // 启动一次温度转换
	}
	return ~ack;      // ack==0表示操作成功，所以返回值对其取反
}

// 读取DS18B20转换的温度值，返回值-表示是否读取成功
bit Get18B20Temp(int16 *temp)
{
	bit ack;
	uint8 LSB, MSB; // 16bit温度值的低字节和高字节

	ack = Get18B20Ack();   // 执行总线复位，并获取18B20应答
	if(ack == 0)
	{
		Write_Byte(0xcc);  // 跳过ROM操作
		Write_Byte(0xbe);  // 发送读暂存器指令
		LSB = Read_Byte(); // 读温度值的低字节
		MSB = Read_Byte(); // 读温度值的高字节
		*temp = ((int16)MSB << 8) + LSB; // 合成为16bit整型数
	}
	return ~ack;  // ack==0表示操作应答，所以返回值为其取反值
}