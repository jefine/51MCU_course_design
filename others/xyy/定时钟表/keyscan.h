#ifndef __KEYSCAN_H__
#define __KEYSCAN_H__

#include<reg51.h>
#include"delay.h"
sbit beep=P2^3;
#define uchar unsigned char
#define uint unsigned int
	
extern uchar temp,key,wei;
extern uchar ok,dingshi,change,yidingshi;

extern void keyscan();
extern void keyscan0();

#endif