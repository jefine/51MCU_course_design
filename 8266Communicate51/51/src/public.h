#ifndef PUBLIC_H
#define PUBLIC_H

#include <reg52.H>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

extern uchar DisplayData[8];
extern char num;
extern uchar code smgduan[10];
//led
sbit led = P2 ^ 0; 
sbit led5 = P2 ^ 5;

//138 
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;


/*******************************************************************************
* �� �� ��         : delay
* ��������		   : ��ʱ������i=1ʱ����Լ��ʱ10us
*******************************************************************************/
void delay(unsigned int i);

/*******************************************************************************
* �� �� ��         : datapros()
* ��������		   : �¶ȶ�ȡ����ת������
* ��    ��         : temp
* ��    ��         : ��
*******************************************************************************/
int dataprosTemp(int temp);

/*******************************************************************************
* ������         :DigDisplay()
* ��������		 :�������ʾ����
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void DigDisplay();

void sendtemp( );

void sendprad();

#endif