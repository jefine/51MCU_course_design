#ifndef TEMPE_H
#define TEMPE_H

#include<reg52.h>

//--����ʹ�õ�IO��--//
sbit DSPORT=P3^7;

//--����ȫ�ֺ���--//
void Delay1ms(unsigned int );
unsigned char Ds18b20Init();
void Ds18b20WriteByte(unsigned char com);
unsigned char Ds18b20ReadByte();
void  Ds18b20ChangTemp();
void  Ds18b20ReadTempCom();
int Ds18b20ReadTemp();

#endif
