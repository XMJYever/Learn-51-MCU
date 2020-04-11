#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char
#define At24c02ADDR 0xa0
#define I2cRead 1
#define I2cWrite 0

sbit DU = P2^6;  //数码管段选
sbit WE = P2^7;  //数码管段选
sbit SCL = P2^1;
sbit SDA = P2^0;
bit AckFlag;
uchar num; // 数码管显示的值

//共阴数码管段选表0-9
uchar  code SMGduan[]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,};
// 数码管位选码
uchar code SMGwei[] = {0xfe, 0xfd, 0xfb};
/*====================================
函数	： delay(uint z)
参数	：z 延时毫秒设定，取值范围0-65535
返回值	：无
描述	：12T/Fosc11.0592M毫秒级延时
====================================*/
void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--); 		
} 

/*====================================
函数	：display(uchar i)
参数	：i 显示数值，取值范围0-255
返回值	：无
描述	：三位共阴数码管动态显示
====================================*/
void display(uchar i)
{
	static uchar wei; // 局部静态变量
	//第一位数码管  		
	P0 = 0XFF;//清除断码
	WE = 1;//打开位选锁存器
	P0 = SMGwei[wei]; // 1111 1110
	WE = 0;//锁存位选数据
	switch(wei)
	{
		case 0:	DU = 1; P0 = SMGduan[i/100]; DU = 0; break;
		case 1:	DU = 1; P0 = SMGduan[i%100/10]; DU = 0; break;
		case 2:	DU = 1; P0 = SMGduan[i%10]; DU = 0; break;
	}
	wei++;
	if(wei == 3)
		wei = 0;
}
//定时器0初始化
void timer0Init()
{
	EA = 1;	// 打开总中断
	ET0 = 1; // 打开定时器0中断
	TR0 = 1;	 //启动定时器0
	TMOD |= 0X01; //定时器工作模式1，16位定时器计数模式
	TH0 = 0xed;
	TL0 = 0xff; //定时5ms
}

/*************************************
IIC通信代码
*************************************/
void delay5us()
{
	_nop_(); // 库函数，延时一个机器周期
}
// 起始信号
void I2cStart()
{
	 SCL = 1;
	 SDA = 1;
	 delay5us();
	 SDA = 0;
	 delay5us();
}
// 终止信号
void I2cStop()
{
	SCL = 0;
	SDA = 0;
	SCL = 1;
	delay5us();
	SDA = 1;
	delay5us();
}
// 总线读从机应答信号
bit ReadACK()
{
	SCL= 0;  // 拉低时钟总线，允许从机控制SDA
	SCL = 1; // 拉高，读SDA
	delay5us();
	if(SDA)	 // NOACK
	{
		SCL = 0;
		return(1);
	}
	else  // ACK
	{
		SCL = 0;
		return(0);
	}
}
// 接收数据之后，主机对从机发送应答，从机释放SDA线
void SendACK(bit i)
{
	SCL = 0;
	if(i)   // 如果有数据，则非应答
		SDA = 1;
	else
		SDA = 0;
	SCL = 1;
	delay5us();
	SCL = 0;
	SDA = 1;  // 释放数据总线
}

// I2c发送一个字节数据
void I2cSendByte(uchar DAT)
{
	uchar i;
	for(i=0; i<8; i++)
	{
		SCL = 0;
		if(DAT & 0x80)
			SDA = 1;
		else
			SDA = 0;
		SCL = 1;
		DAT <<= 1;	// 左移
	}
	SCL = 0;
	SDA = 1;  // 释放数据总线
}

// AT24c02指定单元写入一个字节数据
void At24c02Write(uchar ADDR, DAT)
{
	I2cStart();
	I2cSendByte(At24c02ADDR + I2cWrite);// I2C发送一个字节
	if(ReadACK())  // 读从机应答
	{
		AckFlag = 1; // NO ACK
	}
	else
		AckFlag = 0;  // ACK
	I2cSendByte(ADDR);// I2C发送首地址
	if(ReadACK())
	{
		AckFlag = 1; // NO ACK
	}
	else
		AckFlag = 0;  // ACK
	I2cSendByte(DAT);// I2C发送数据
	if(ReadACK())	// 读应答
	{
		AckFlag = 1; // NO ACK
	}
	else
		AckFlag = 0;  // ACK
	I2cStop();
}

// I2c总线读一字节数据
uchar I2cReadByte()
{
	uchar i, DAT;
	for(i=0; i<8; i++)
	{
		DAT <<= 1;	// 数据左移1位，每次读一位
		SCL = 0;
		SCL = 1;
		if(SDA)
			DAT |= 0x01;
	}
	return(DAT);
}
// 读AT24C02指定单元内数据
uchar At24C02Read(uchar ADDR)
{
	uchar DAT;
	I2cStart();
	I2cSendByte(At24c02ADDR + I2cWrite);// I2C发送一个字节
	if(ReadACK()) // 读从机应答
	{
		AckFlag = 1; // NO ACK
	}
	else
		AckFlag = 0;  // ACK
	I2cSendByte(ADDR);// I2C发送首地址
	ReadACK();
	I2cStart();
	I2cSendByte(At24c02ADDR + I2cRead);// 发送器件地址加读写方向位 读
	if(ReadACK())	// 读从机应答
	{
		AckFlag = 1; // NO ACK
	}
	else
		AckFlag = 0;  // ACK
	DAT = I2cReadByte(); //
	SendACK(1);	// 主机发送非应答
	I2cStop();
	return(DAT);

}

void main()//main函数自身会循环
{
	timer0Init();//定时器0初始化
	EA = 0; // 屏蔽中断
	At24c02Write(3, 188);   // 在第二个地址中写入9
	delay(10);	  // 等待处理
	num = At24C02Read(3);
	if(AckFlag)
		P1 = 0;
	else
		P1 = 0xff;
	EA = 1; // 开中断
	while(1);	
}  

// 中断方式是硬件方式清0，计数方式是软件清零
void timer0() interrupt 1  // 定时器0的中断入口1
{
	TH0 = 0Xed;
	TL0 = 0Xff;	
	display(num); //数码管显示函数
}