/*************************************************
����51��Ƭ���������ע΢�Ź��ںţ�����Ӧ��ѧϰ��
**************************************************
*****************������Ӧ��ѧϰ�ݡ�***************
***************************************************/
#include<reg52.h>
#include<intrins.h>
#include "temp.h"

sbit IO_18B20=P3^2;

/*������ʱ��������ʱʱ�䣨t*10��us*/
void DelayX10us(unsigned char t)
{
	do{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}while(--t);
}
/*��λ���ߣ���ȡ�������壬������һ�ζ�д����*/
bit Get18B20Ack()
{
	bit ack;
	
	EA=0;	   			//��ֹ���ж�
	IO_18B20=0;			//����500us��λ����
	DelayX10us(50);
	IO_18B20=1;
	DelayX10us(6);		//��ʱ60us
	ack=IO_18B20;		//��ȡ��������
	while(!IO_18B20);   //�ȴ������������
	EA=1;				//����ʹ�����ж�
	return ack;
}
/*��DS18B20д��һ���ֽڣ�dat-��д���ֽ�*/
void Write18B20(unsigned char dat)
{
	unsigned char mask;
	EA=0;
	for(mask=0x01;mask!=0;mask<<=1)//��λ���ȣ������Ƴ�8��bit
	{
		IO_18B20=0;//����2us�͵�ƽ����
		_nop_();
		_nop_();
		if((mask&dat)==0)//�����bitֵ
			IO_18B20=0;
		else
			IO_18B20=1;
		DelayX10us(6);//��ʱ60us
		IO_18B20=1;//����ͨ������
	}
	EA=1;
}
/*��DS18B20��ȡһ���ֽڣ�����ֵ-�������ֽ�*/
unsigned char Read18B20()
{
	unsigned char dat;
	unsigned char mask;

	EA=0;
	for(mask=0x01;mask!=0;mask<<=1)//��λ���ȣ����βɼ�8��bit
	{
		IO_18B20=0;//����2us�͵�ƽ����
		_nop_();
		_nop_();
		IO_18B20=1;//�����͵�ƽ���壬�ȴ�18B20�������
		_nop_(); //��ʱ2us
		_nop_();
		if(!IO_18B20)//��ȡͨ�������ϵ�ֵ
			dat &= ~mask;
		else
			dat |= mask;
		DelayX10us(6);//����ʱ60us
	}
	EA=1;
	return dat;
}
/*����һ��18B20�¶�ת��������ֵ-��ʾ�Ƿ������ɹ�*/
bit Start18B20()
{
	bit ack;
	ack=Get18B20Ack();//ִ�����߸�λ������ȡ18B20Ӧ��
	if(ack==0)
	{
		Write18B20(0xCC);
		Write18B20(0x44);
	}
	return ~ack;
}
/*��ȡDS18B20ת�����¶�ֵ������ֵ-��ʾ�Ƿ��ȡ�ɹ�*/
bit Get18B20Temp(int *temp)
{
	bit ack;
	unsigned char LSB,MSB;//16bit�¶�ֵ�ĵ��ֽں͸��ֽ�

	ack=Get18B20Ack();//ִ�����߸�λ������ȡ18B20Ӧ��
	if(ack==0)
	{
		Write18B20(0xCC);//����ROM����
		Write18B20(0xBE);//���Ͷ�����
		LSB=Read18B20();//���¶�ֵ�ĵ��ֽ�
		MSB=Read18B20();//���¶�ֵ�ĸ��ֽ�
		*temp=((int)MSB<<8)+LSB;//�ϳ�Ϊ16bit������
	}
	return ~ack;
}