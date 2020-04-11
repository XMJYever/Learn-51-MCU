#include "config.h"
#include "app.h"
#include "Key_Buzzer.h"

uint8 code KeyCodeMap[4] = { //4λ������������ע������ӳ���
	0x0d, // �س���
	0x26; // �ϼ�
	0x28; // �¼�
	0x1b; // esc��
};
uint8 pdata KeySta[4] = { // 4λ����������ǰ״̬
	1, 1, 1, 1
};
void KeyDriver()  // �������
{
	uint8 i;
	static uint8 pdata backup[4] = { // 4λ������������ֵ
		1, 1, 1, 1
	}
	for(i=0; i<4; i++)	 // ѭ�����4����������
	{
		if(backup[i] != KeySta[i])	// ��ⰴ��
		{
			if(backup[i] != 0)	  // �����������
			{
				KeyAction(KeyCodeMap[i]);  // ���ð�����������
			}
			backup[i] = KeySta[i]; // ˢ�±���ֵ
		}
	}
}
/* ����ɨ�躯�������ڶ�ʱ�ж��е��ã����4ms */
void KeyScan()
{
	uint8 i;
	static uint8 flag = 0;  // ��������
	static uint8 keybuf[4] = { // 4λ��������ɨ�軺����
		0xff, 0xff, 0xff, 0xff
	};
	// ��4����������ֵ���뻺����
	keybuf[0] = (keybuf[0] << 1) | KEY_S2;
	keybuf[1] = (keybuf[1] << 1) | KEY_S3;
	keybuf[2] = (keybuf[2] << 1) | KEY_S4;
	keybuf[3] = (keybuf[3] << 1) | KEY_S5;

	flag++;   // ���5msɨ��һ��
	if(flag == 4) // 4�ξ���20ms �������
	{
		flag = 0; // ɨ���������
		for(i=0; i<4; i++)
		{
			if((keybuf[i] & 0x0f) == 0x00)
			{
				KeySta[i] = 0; // ���4��ɨ���ֵ��Ϊ0�� ������״̬
			}
			else if ((keybuf[i] & 0x0f) == 0x0f)
			{
				KeySta[i] = 1;   // ���4��ɨ���ֵ��Ϊ1��������״̬
			}
		}
	}
}
/* ���������� */
void staBuzzer(bit e)
{
	if(e == 1)
		BUZZER = 0;
	else
		BUZZER = 1;
}