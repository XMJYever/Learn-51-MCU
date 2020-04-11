#include "DS18B20.h"
#include "config.h"
/* �¶ȴ�����DS18B20��ѭ������ͨ��Э��
	���裺1����ʼ����2��ROM����ָ�3����������ָ��
 */
// ��ʱ��������ʱ10us
void Delay_us(uint8 us)   //��� -0.234375us
{
    uint8 i,a;
	for(i=0;i<us;i++)
    	for(a=3;a>0;a--);
}

// дʱ��
void Write_Byte(uchar dat)
{
	uint8 i;
	EA = 0;       // ��ֹ���ж�
	for(i=0; i<8; i++)
	{
		DS18B20_IO = 0; // дʱ��
		_nop_(); // ����ʱ��
		DS18B20_IO = dat & 0x01; // ȡ���λ
		Delay_us(65); 	// ��ʱ60us
		DS18B20_IO = 1; // �ͷ�����
		_nop_();
		dat >>= 1; // ����һλ		
	}
	EA = 1;   // ʹ�����ж�
}

// ��ʱ��
uchar Read_Byte()
{
	uint8 i, j, dat;
	EA = 0;       // ��ֹ���ж�
	for(i=0; i<8; i++)
	{
		DS18B20_IO = 1;
		_nop_();
		DS18B20_IO = 0;
		_nop_();  // ��������1us
		j = DS18B20_IO;  // ��¼��ȡ������
		Delay_us(70); // ��ʱ70us
		dat = (j>>7)|(dat<<1); //�ӵ�λ������λ
	}
	DS18B20_IO = 1; // �ͷ�����
	return dat;
}
// ��ʼ��DS18B20,����ȡ��������
bit GetDS18B20Ack()
{
	bit i;
	EA = 0;         // ��ֹ���ж�
	DS18B20_IO = 1; // ������
	_nop_(); // ��������һ���������ڣ�1.08us
	// ���͸�λ����
	DS18B20_IO = 0; // ���͵�ƽ
	Delay_us(500);	// ��ʱ500us������480us
	DS18B20_IO = 1; // �ͷ����ߣ����ߵ�ƽ����������
	Delay_us(50); // ��ʱ50us

	// �ȴ��ӻ����ʹ�������
	i = DS18B20_IO;     // ���յ��ӻ��������ź�
	while(!DS18B20_IO);	// �ȴ������������
	Delay_us(100); //��ʱ100us
	_nop_();
	EA = 1;  // ʹ�����ж�

	return (i);
}
// ����һ��18B20�¶�ת��������ֵ-��ʾ�Ƿ������ɹ���
// 1����ʼ����2��ROMָ�3����ָ��
bit	Start18B20()
{
	bit ack;
	// ��ʼ��
	ack = GetDS18B20Ack();
	// ROMָ��
	if(ack == 0)	// ��������Ϊ�͵�ƽ
	{
		Write_Byte(0xcc);   // ����ROM����
		Write_Byte()0x44;   // ����һ���¶�ת��
	}
	return ~ack;      // ack==0��ʾ�����ɹ������Է���ֵ����ȡ��
}

// ��ȡDS18B20ת�����¶�ֵ������ֵ-��ʾ�Ƿ��ȡ�ɹ�
bit Get18B20Temp(int16 *temp)
{
	bit ack;
	uint8 LSB, MSB; // 16bit�¶�ֵ�ĵ��ֽں͸��ֽ�

	ack = Get18B20Ack();   // ִ�����߸�λ������ȡ18B20Ӧ��
	if(ack == 0)
	{
		Write_Byte(0xcc);  // ����ROM����
		Write_Byte(0xbe);  // ���Ͷ��ݴ���ָ��
		LSB = Read_Byte(); // ���¶�ֵ�ĵ��ֽ�
		MSB = Read_Byte(); // ���¶�ֵ�ĸ��ֽ�
		*temp = ((int16)MSB << 8) + LSB; // �ϳ�Ϊ16bit������
	}
	return ~ack;  // ack==0��ʾ����Ӧ�����Է���ֵΪ��ȡ��ֵ
}