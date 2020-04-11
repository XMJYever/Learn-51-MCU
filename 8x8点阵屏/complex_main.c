#include <reg52.h>
#include <intrins.h>

sbit DIO = P3^4;   // �������ݿ�
sbit S_CLK = P3^5; // ��λ�Ĵ���ʱ��
sbit R_CLK = P3^6; // ���������ʱ��
unsigned char code table[2][8] = {0x58,0xAD,0x95,0xA0,0x05,0xB5,0x10,0xFF, // ��
0xFE,0x74,0xAE,0xDA,0xAA,0x06,0xDE,0xBE};
 // 0x83,0xAB,0xAB,0x00,0xAA,0xAA,0x82,0xFC//��
void Send_Byte(unsigned char dat)
{
	unsigned char i;
	S_CLK = 0;
	R_CLK = 0;
	for(i=0;i<8;i++)
	{
		if(dat & 0x01)	 // �жϵ�1λΪ1
			DIO = 1;
		else
			DIO = 0;
		S_CLK = 1;  // ���������أ���λ���Ĵ�����
		dat >>= 1;  // ����1λ����������
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
			for(z = 0; z<1000; z++)	  // ��ʱ��ʾ
			{
				ROW = 0x80;
				// ��ѡ��ֵ
				for(j = 0; j<8;j++)
				{
					Send_Byte(table[k][j]);
					Send_Byte(ROW);   //������
					R_CLK = 1;  // �������������
					R_CLK = 0;  
					ROW = _cror_(ROW,1); // ѭ������
				}
			}
		}
	}
}
