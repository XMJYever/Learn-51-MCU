#include <reg52.h>
sbit DIO = P3^4;   // �������ݿ�
sbit S_CLK = P3^5; // ��λ�Ĵ���ʱ��
sbit R_CLK = P3^6; // ���������ʱ��

void main()
{
	// row 0x80 ��COL 0xfe��������һ����
	// ��������
	unsigned char i, dat;
	dat = 0xfe;	 // �����ֵ
	for(i=0;i<8;i++)
	{
		S_CLK = 0;
		R_CLK = 0;
		if(dat & 0x01)	 // �жϵ�1λΪ1
			DIO = 1;
		else
			DIO = 0;
		S_CLK = 1;  // ���������أ���λ���Ĵ�����
		dat >>= 1;  // ����1λ����������	
	}		

	dat = 0x80;
	for(i=0;i<8;i++)
	{
		S_CLK = 0;
		R_CLK = 0;
		if(dat & 0x01)	 // �ж��Ƿ�Ϊ��8λ
			DIO = 1;
		else
			DIO = 0;
		S_CLK = 1;  // ���������أ���λ���Ĵ�����
		dat >>= 1;  // ����1λ����������	
	}
	R_CLK = 1;  // �������������
	while(1);   // �ó��򱣳�
}
