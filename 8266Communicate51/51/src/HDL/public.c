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


//温度读取处理转换函数

int dataprosTemp(int temp) 	 
{
   	float tp;  
	if(temp< 0)				//当温度值为负数
  	{
		DisplayData[0] = 0x40; 	  //   -
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	DisplayData[1] = smgduan[temp / 10000];
	DisplayData[2] = smgduan[temp % 10000 / 1000];
	DisplayData[3] = smgduan[temp % 1000 / 100] | 0x80;
	DisplayData[4] = smgduan[temp % 100 / 10];
	DisplayData[5] = smgduan[temp % 10];

	return temp;
}


//数码管
void DigDisplay()
{
	uchar i;
	
	for(i=0;i<6;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=0; break;//显示第1位
			case(2):
				LSA=0;LSB=1;LSC=0; break;//显示第2位
			case(3):
				LSA=1;LSB=1;LSC=0; break;//显示第3位
			case(4):
				LSA=0;LSB=0;LSC=1; break;//显示第4位
			case(5):
				LSA=1;LSB=0;LSC=1; break;//显示第5位	
		}
		P0=DisplayData[5-i];//发送数据
		Delay1ms(1);
		//delay(100); //间隔一段时间扫描	
		P0=0x00;//消隐
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
	temp = Read_AD_Data(0xA4);		// 光敏电阻
	DataPackage_XPT2046[8] = (char)(temp/1000)+'0';//千位
	DataPackage_XPT2046[9] = (char)(temp%1000/100)+'0';//百位
	DataPackage_XPT2046[10] = (char)(temp%1000%100/10)+'0';//十位
	DataPackage_XPT2046[11] = (char)(temp%1000%100%10)+'0';//个位
	PutString(DataPackage_XPT2046);
	LcdDisplayPrad(DataPackage_XPT2046);
}