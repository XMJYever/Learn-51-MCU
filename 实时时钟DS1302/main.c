#include <reg52.h>
#include <intrins.h>
#define MAIN_Fosc		11059200UL	//宏定义主时钟HZ
/*====================================
 自定义类型名
====================================*/
typedef unsigned char INT8U;
typedef unsigned char uchar;

typedef unsigned int INT16U;
typedef unsigned int uint;

/*====================================
 硬件接口位声明
====================================*/
sbit TSCLK = P1^0;//时钟线 接到P10上用杜邦线
sbit TIO   = P1^1;//数据线，接到P11上
sbit TRST  = P1^2;//使能端，接到P12上
sbit DU  = P2^6;   //数码管段选
sbit WE  = P2^7;   //数码管位选
/*====================================
共阴极数码管段选码
====================================*/
uchar code table[]={ 
//0		1	 2     3     4     5     6     7     8
0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
//9     A     B	   C	 D	   E	 F		-	 .	  关显示
0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x40, 0x80, 0x00
                   };

/*====================================
数码管位选码
====================================*/
				  //第1位	2位	  3位	 4位   5位	6位	  7位	8位
uchar code T_COM[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};//数码管位码

/*====================================
函数：void Delay_Ms(INT16U ms)
参数：ms，毫秒延时形参
描述：12T 51单片机自适应主时钟毫秒级延时函数
====================================*/
void Delay_Ms(INT16U ms)
{
     INT16U i;
	 do{
	      i = MAIN_Fosc / 96000; 
		  while(--i);   //96T per loop
     }while(--ms);
}

void Display(uchar Hour, Min, Sec)
{		
//------------------------------
	DU = 0;							
	P0 = table[Hour/10];		
	DU = 1;						
	DU = 0;							

	WE = 0;						
	P0 = T_COM[0];				   
	WE = 1;						
	WE = 0;						
	Delay_Ms(3);
//-------------------------------
	DU = 0;
	P0 = table[Hour%10]|0x80; 
	DU = 1;
	DU = 0;

	WE = 0;
	P0 = T_COM[1];			  
	WE = 1;
	WE = 0;
	Delay_Ms(3);
//------------------------------
	DU = 0;							
	P0 = table[Min/10];		
	DU = 1;						
	DU = 0;							

	WE = 0;						
	P0 = T_COM[2];				   
	WE = 1;						
	WE = 0;						
	Delay_Ms(3);
//-------------------------------
	DU = 0;
	P0 = table[Min%10]|0x80; 
	DU = 1;
	DU = 0;

	WE = 0;
	P0 = T_COM[3];			 
	WE = 1;
	WE = 0;
	Delay_Ms(3);
//------------------------------
	DU = 0;							
	P0 = table[Sec/10];		
	DU = 1;						
	DU = 0;							

	WE = 0;						
	P0 = T_COM[4];				   
	WE = 1;						
	WE = 0;						
	Delay_Ms(3);
//-------------------------------
	DU = 0;
	P0 = table[Sec%10]; 
	DU = 1;
	DU = 0;

	WE = 0;
	P0 = T_COM[5];			  
	WE = 1;
	WE = 0;
	Delay_Ms(3);

}

//写DS1302数据
void Write_DS1302_DAT(uchar cmd, uchar dat)
{
	uchar i;
	TRST = 0; //拉低使能端
	TSCLK = 0;//拉低数据总线
	TRST = 1; //拉高使能端，产生上升沿开始写数据
	for(i = 0; i < 8; i++)//每次写1位，写8次
	{
		TSCLK = 0;		  //拉低时钟总线
		TIO = cmd & 0x01; //写1位数据，从最低位开始写
		TSCLK = 1;		  //拉高时钟总线，产生上升沿数据被DS1302读走
		cmd >>=1;		  //右移一位
	}
	for(i = 0; i < 8; i++)//每次写1位，写8次
	{
		TSCLK = 0;		  //拉低时钟总线
		TIO = dat & 0x01; //写1位数据，从最低位开始写
		TSCLK = 1;		  //拉高时钟总线，产生上升沿数据被DS1302读走
		dat >>= 1;		  //右移一位
	}
}
//读DS1302数据
uchar Read_DS1302_DAT(uchar cmd)
{
	uchar i, dat;
	TRST = 0;  //拉低使能端
	TSCLK = 0; //拉低数据总线
	TRST = 1;  //拉高使能端，产生上升沿开始写数据
	for(i = 0; i < 8; i++)//每次写1位，写8次
	{
		TSCLK = 0;		 //拉低时钟总线
		TIO = cmd & 0x01;//写1位数据，从最低位开始写
		TSCLK = 1;		 //拉高时钟总线，产生上升沿数据被DS1302读走
		cmd >>=1;		 //右移一位
	}
	for(i = 0; i < 8; i++)//每次读1位，读8次
	{
		TSCLK = 0;		  //拉低时钟总线,产生下降沿，DS1302把数据放到TIO上
		dat >>= 1;		  //右移一位
		if(TIO)	dat |= 0x80;//读取数据，从最低位开始
		TSCLK = 1;			//拉高时钟总线，以备下一次产生下降沿
	}
	return dat;	//返回读出数据
}

//数据转BCD码
uchar Dat_Chg_BCD(uchar dat)
{
	uchar dat1, dat2;
	dat1 = dat / 10;
	dat2 = dat % 10;
	dat2 = dat2 + dat1 * 16;
	return dat2;
}

//BCD码转换为数据
uchar BCD_Chg_Dat(uchar dat)
{
	uchar dat1, dat2;
	dat1 = dat / 16;
	dat2 = dat % 16;
	dat2 = dat2 + dat1 * 10;
	return dat2;
}

void main()
{
	uchar i;
	uchar Sec, Min, Hour;
	Write_DS1302_DAT(0x8e, 0);//清除写保护
	Write_DS1302_DAT(0x80, Dat_Chg_BCD(30));//30秒（并且进行BCD码转换）
	Write_DS1302_DAT(0x82, Dat_Chg_BCD(15));//15分
	Write_DS1302_DAT(0x84, Dat_Chg_BCD(19));//19时
	Write_DS1302_DAT(0x8e, 0x80);//开写保护
	while(1)
	{
		Write_DS1302_DAT(0x8e, 0); //清除写保护
		Sec = BCD_Chg_Dat(Read_DS1302_DAT(0x81));//读秒寄存器（并且进行BCD码转换）
		Min	= BCD_Chg_Dat(Read_DS1302_DAT(0x83));//读分寄存器
		Hour = BCD_Chg_Dat(Read_DS1302_DAT(0x85));//读时寄存器
		Write_DS1302_DAT(0x8e, 0x80);//开写保护
		for(i = 0; i < 50; i++)	//循环显示时钟
			Display(Hour, Min, Sec);

	}
}