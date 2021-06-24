#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include<reg51.h>
#include"delay.h"

#define uchar unsigned char
#define uint unsigned int
	
sbit dula=P2^6;
sbit wela=P2^7;	

extern uint pp;
extern uchar code table[];
extern uchar shi20,shi10,fen20,fen10,miao20,miao10;
extern void display(uchar shi2,uchar shi1,uchar fen2,uchar fen1,uchar miao2,uchar miao1);

#endif