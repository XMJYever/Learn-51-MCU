#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <reg52.h>
#include <intrins.h>

#define MAIN_Fosc 11059200UL  // �궨����ʱ��HZ

// ���������������¶���
typedef signed char int8;  //8λ�з�����
typedef signed int  int16; //16λ�з�����
typedef unsigned char uint8;  //8λ�޷�����
typedef unsigned char uchar;  //8λ�޷�����
typedef unsigned int  uint16; //16λ�޷�����
typedef unsigned int  uint; //16λ�޷�����
typedef unsigned long uint32; //32λ�޷�����
/****************************************************************
 Ӳ���ӿڶ���
****************************************************************/
/*DS1302IO�ӿ� ����3P�Ű������Ӧ��P1������*/
sbit TSCLK = P1^0;   //DS1302ʱ������
sbit TIO   = P1^1;   //DS1302��������
sbit TRST  = P1^2;   //DS1302ʹ������
/*4λ������������*/
sbit KEY_S2 = P3^0;	//S2��������
sbit KEY_S3 = P3^1;	//S3��������
sbit KEY_S4 = P3^2;	//S4��������
sbit KEY_S5 = P3^3;	//S5��������
/*LCD1602Һ������*/
#define LCD1602_DB  P0      //data bus ��������
sbit LCD1602_RS = P3^5;		//LCD1602Һ������/����ѡ������
sbit LCD1602_RW = P3^6;		//LCD1602Һ����/дѡ������
sbit LCD1602_EN = P3^4;	 	//LCD1602Һ��ʹ������
/*DS18B20�¶ȴ���������*/
sbit DS18B20_IO = P2^2;   //DS18B20ͨ������
/*5V��Դ����������*/
sbit BUZZER = P2^3;	//����������

#endif