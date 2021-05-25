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
* 函 数 名         : delay
* 函数功能		   : 延时函数，i=1时，大约延时10us
*******************************************************************************/
void delay(unsigned int i);

/*******************************************************************************
* 函 数 名         : datapros()
* 函数功能		   : 温度读取处理转换函数
* 输    入         : temp
* 输    出         : 无
*******************************************************************************/
int dataprosTemp(int temp);

/*******************************************************************************
* 函数名         :DigDisplay()
* 函数功能		 :数码管显示函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void DigDisplay();

void sendtemp( );

void sendprad();

#endif