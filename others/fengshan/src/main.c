#include "REG52.H"
//用不到，采用单片机上的步进电机即可，1号对应P1^0
sbit IN1=P2^7;
sbit IN2=P2^6;
sbit ENA=P2^5;
sbit in = P1^0;
bit flag1s=0;//1s定时标志
unsigned char T0RH=0;
unsigned char T0RL=0;

int temp;//读取到的当前温度值
unsigned char len;
int intT,decT;//温度值的整数和小数部分
unsigned char str[12];

void Compare();
void GetTemp();
void ConfigTimer0(unsigned int ms);
unsigned char IntToString(unsigned char *str,int dat);
extern bit Start18B20();
extern bit Get18B20Temp(int *temp);
extern void InitLcd1602();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
void delay(unsigned int z);
void main()
{
	bit res;
    int i;
	// EA=1;
	// ConfigTimer0(10);//T0定时10ms

    // IN1=1;
    // IN2=0;
    for(i=0;i<200;i++)
    {
        in=1;
        delay(20);
        in=0;
        delay(30);
    }

	// Start18B20();//启动DS18B20
	// InitLcd1602();//初始化液晶

	// while(1)
	// {		
	// 	if(flag1s)//每秒更新一次温度
	// 	{
	// 		flag1s=0;
	// 		res=Get18B20Temp(&temp);//读取当前温度
	// 		if(res)//读取成功时，刷新当前温度显示
	// 		{
	// 			GetTemp();
			
	// 			LcdShowStr(0,0,"Welcome to use");//显示字符及温度值
	// 			LcdShowStr(0,1,"Current T:");
	// 			LcdShowStr(10,1,str);
	// 				Compare();
	// 		}
	// 		else //读取失败时，提示错误信息
	// 		{
	// 			LcdShowStr(0,0,"error!");

	// 		}
	// 		Start18B20();//重新启动下一次转换					 
	// 	}
	// }
}
/*温度获取函数，获取当前环境温度值并保存在str数组中*/
void GetTemp()
{

	intT=temp>>4;//分离出温度值整数部分
	decT=temp &0x0F;//分离出温度值小数部分
			
	len=IntToString(str,intT);//整数部分转换成字符串
			
	str[len++]='.';
	decT=(decT*10)/16;//二进制的小数部分转换为1位十进制位
	str[len++]=decT+'0';//十进制小数位再转换为ASCII字符
	while(len<6)//用空格补齐到6个字符长度
	{
		str[len++]=' ';
	}
	str[len++]='\0';
}
/*延时函数，用于PWM控制*/
void delay(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
} 
/*比较函数，通过温度值的比较设置电机的转速*/
void Compare()
{
	unsigned int i=0;
	unsigned char j;

	if((intT>= 24) && (intT<26))   //以两度为一个温差范围，并设温度范围索引
	{
		j=0;	
	}
	else if((intT>=26) &&(intT<28))
	{
		j=1;
	}
	else if((intT>=28) &&(intT<30))
	{
		j=2;
	}
	else if(intT>=30)
	{
		j=3;
	}
	switch(j)		  //根据温度索引设置电机转速
	{
		case 0:	
				break;
	
		case 1:	IN1=1;
				IN2=0;
		  		for(i=0;i<200;i++)
	      		{
					ENA=1;
	     			delay(30);
	      		    ENA=0;
					delay(30);
				}
				break;	 
	
		case 2:	IN1=1;
				IN2=0;
		  		for(i=0;i<200;i++)
	      		{
					ENA=1;
	     			delay(55);			 
	      		    ENA=0;
					delay(30);
				}
				break;	 
							
		case 3:	IN1=1;
				IN2=0;
		  	    ENA=1;
				break;

		default:break;	 	 
	}
}  