#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2^6; // 数码管段选
sbit WE = P2^7; // 数码管位选

//共阴极数码管段选表0-9, 存放在rom区
uchar code table[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
/*void main()
{
	while(1)
	{
	WE = 1;	   // 位选处于高电平，输出端可以修改值
	P0 = 0xf5; // 第一个数码管为1  1111 0000
	delay();
	_crol_(P0, 1); // 循环左移， 1110 0001
	WE = 0;   // 锁存位选锁存器


	DU = 1;	 // 打开段选锁存器
	P0 = 0x06; // 0000 0110 
	DU = 0;  // 关闭锁存器；	
	}
	

//	while(1)
//	{
//		// P0 = 0x01;
//	}
}

void delay(void)   //误差 -0.000000000227us
{
    unsigned char a,b,c;
    for(c=13;c>0;c--)
        for(b=247;b>0;b--)
            for(a=142;a>0;a--);
    _nop_;  //if Keil,require use intrins.h
}
*/


// 数码管动态显示
void delay(uint z)
{
	uint x, y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0; y--);
}


void display(uchar i)
{
	uchar bai, shi, ge;
	bai = i/100;
	shi = i%100 / 10;
	ge  = i%10;

	//第一位数码管  		
	P0 = 0XFF;//清除断码
	WE = 1;//打开位选锁存器
	P0 = 0XFE; //1111 1110
	WE = 0;//锁存位选数据
	
	DU = 1;//打开段选锁存器
	P0 = table[bai];//
	DU = 0;//锁存段选数据
	delay(5);

	//第二位数码管
	P0 = 0XFF;//清除断码
	WE = 1;//打开位选锁存器
	P0 = 0XFD; //1111 1101
	WE = 0;//锁存位选数据
	
	DU = 1;//打开段选锁存器
	P0 = table[shi];//
	DU = 0;//锁存段选数据
	delay(5);

	//第三位数码管
	P0 = 0XFF;//清除断码
	WE = 1;//打开位选锁存器
	P0 = 0XFB; //1111 1011
	WE = 0;//锁存位选数据
	
	DU = 1;//打开段选锁存器
	P0 = table[ge];//
	DU = 0;//锁存段选数据
	delay(5);

/*
	P0 = 0Xff; // 清除断码
	// 第一个数码管显示1
	WE = 1; // 打开位选锁存器
	P0 = 0xfe; // 1111 1110
	WE = 0; // 锁存位选数据

	DU = 1;
	P0 = table[bai]; // 0000 0110
	DU = 0;    // 锁存段选数据
	delay(5);  // 延时5ms

	P0 = 0Xff; // 清除断码
	//第二个数码管显示2
	WE = 1; // 打开位选锁存器
	P0 = 0xfd; // 1111 1101
	WE = 0; // 锁存位选数据

	DU = 1;
	P0 = table[shi]; // 0101 1011
	DU = 0;    // 锁存段选数据
	delay(5);  // 延时5ms

	P0 = 0Xff; // 清除断码
	// 第三个数码管显示3
	WE = 1; // 打开位选锁存器
	P0 = 0xfb; // 1111 1011
	WE = 0; // 锁存位选数据

	DU = 1;
	P0 = table[ge]; // 0100 1111
	DU = 0;    // 锁存段选数据
	delay(5);  // 延时5ms

	/*P0 = 0Xff; // 清除断码
	//第四个数码管显示4
	WE = 1; // 打开位选锁存器
	P0 = 0Xf7; // 0100 1111
	WE = 0; // 锁存位选数据

	DU = 1;
	P0 = 0X66; // 0110 0110
	DU = 0;    // 锁存段选数据
	delay(5);  // 延时5ms

	P0 = 0Xff; // 清除断码
	// 第五个数码管显示5
	WE = 1; // 打开位选锁存器
	P0 = 0Xef; // 1110 1111
	WE = 0; // 锁存位选数据

	DU = 1;
	P0 = 0X6d; // 0110 1101
	DU = 0;    // 锁存段选数据
	delay(5);  // 延时5ms

	P0 = 0Xff; // 清除断码
	//第六个数码管显示6
	WE = 1; // 打开位选锁存器
	P0 = 0Xdf; // 1101 1111
	WE = 0; // 锁存位选数据

	DU = 1;
	P0 = 0X7d; // 0111 1101
	DU = 0;    // 锁存段选数据
	delay(5);  // 延时5ms

	P0 = 0Xff; // 清除断码
	// 第七个数码管显示7
	WE = 1; // 打开位选锁存器
	P0 = 0Xbf; // 1011 1111
	WE = 0; // 锁存位选数据

	DU = 1;
	P0 = 0X07; // 0000 0111
	DU = 0;    // 锁存段选数据
	delay(5);  // 延时5ms

	P0 = 0Xff; // 清除断码
	//第八个数码管显示8
	WE = 1; // 打开位选锁存器
	P0 = 0X7f; // 0111 1111
	WE = 0; // 锁存位选数据

	DU = 1;
	P0 = 0X7f; // 0111 1111
	DU = 0;    // 锁存段选数据
	delay(5);  // 延时5ms
	*/
}

void main()
{
	while(1)
	{	
		display(452);
	}
}