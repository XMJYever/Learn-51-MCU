#include <reg52.h>

unsigned int i; // 0~65535

void main()
{
	while(1)
	{
		P1 = 0x5e;	 // ����P1������led��
		i = 65535;
		while(i--);
		P1 = 0xff;  // 1111 1111  (255)	 Ϩ��P1��8��LED	
		i = 65535;
		while(i--);
	}
}
