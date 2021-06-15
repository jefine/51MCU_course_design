#include "public.h"
#include "REG52.H"
#include "uart.h"
#include "XPT2046.h"
#include "tempe.h"
#include "lcd.h"
char num=0;
uchar DisplayData[8];
uchar code smgduan[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
//10us
void delay(uint i)
{
	while(i--);	
}


//�¶ȶ�ȡ����ת������

int dataprosTemp(int temp) 	 
{
   	float tp;  
	if(temp< 0)				//���¶�ֵΪ����
  	{
		DisplayData[0] = 0x40; 	  //   -
		//��Ϊ��ȡ���¶���ʵ���¶ȵĲ��룬���Լ�1����ȡ�����ԭ��
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//��Ϊ���ݴ�����С�������Խ��¶ȸ���һ�������ͱ���
		//����¶���������ô����ô������ԭ����ǲ���������
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
	}
	DisplayData[1] = smgduan[temp / 10000];
	DisplayData[2] = smgduan[temp % 10000 / 1000];
	DisplayData[3] = smgduan[temp % 1000 / 100] | 0x80;
	DisplayData[4] = smgduan[temp % 100 / 10];
	DisplayData[5] = smgduan[temp % 10];

	return temp;
}


//�����
void DigDisplay()
{
	uchar i;
	
	for(i=0;i<6;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ
			case(1):
				LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ
			case(2):
				LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ
			case(3):
				LSA=1;LSB=1;LSC=0; break;//��ʾ��3λ
			case(4):
				LSA=0;LSB=0;LSC=1; break;//��ʾ��4λ
			case(5):
				LSA=1;LSB=0;LSC=1; break;//��ʾ��5λ	
		}
		P0=DisplayData[5-i];//��������
		Delay1ms(1);
		//delay(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}		
}

void sendtemp(){
		int temp = dataprosTemp(Ds18b20ReadTemp());
		DataPackage_DS18B20[8] = (char)(temp / 10000)+'0';
        DataPackage_DS18B20[9] = (char)(temp % 10000 / 1000)+'0';
        DataPackage_DS18B20[10] = (char)(temp % 1000 / 100)+'0';
		DataPackage_DS18B20[11] = '.';
        DataPackage_DS18B20[12] = (char)(temp % 100 / 10)+'0';
        DataPackage_DS18B20[13] = (char)(temp % 10)+'0';
        PutString(DataPackage_DS18B20);
		LcdDisplayTemp(DataPackage_DS18B20);
}

void sendprad(){
	int temp;
	temp = Read_AD_Data(0xA4);		// ��������
	DataPackage_XPT2046[8] = (char)(temp/1000)+'0';//ǧλ
	DataPackage_XPT2046[9] = (char)(temp%1000/100)+'0';//��λ
	DataPackage_XPT2046[10] = (char)(temp%1000%100/10)+'0';//ʮλ
	DataPackage_XPT2046[11] = (char)(temp%1000%100%10)+'0';//��λ
	PutString(DataPackage_XPT2046);
	LcdDisplayPrad(DataPackage_XPT2046);
}