#include <reg52.h>

unsigned int i; // 0~65535

void main()
{
	while(1)
	{
		P1 = 0x5e;	 // 点亮P1口所有led灯
		i = 65535;
		while(i--);
		P1 = 0xff;  // 1111 1111  (255)	 熄灭P1口8个LED	
		i = 65535;
		while(i--);
	}
}
