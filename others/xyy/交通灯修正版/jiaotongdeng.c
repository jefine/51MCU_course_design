#include<reg51.h>

#define uchar unsigned char
#define uint unsigned int

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^3;
//23
sbit MOTOR1=P1^0;
sbit MOTOR2=P1^1;

sbit key_green = P3^6;
sbit key_yellow = P3^7;

bit alwaysyellow=0;

unsigned int ppp;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
uchar jj,kk,l=255;
uchar aa1,aa0;
uchar shijian,deng=1;
void delay(unsigned int i)
{
  for(jj=i;jj>0;jj--)
    for(kk=125;kk>0;kk--);
}

void display(unsigned char sh_c,unsigned char g_c)
{
   dula=0;
   P0=table[sh_c];
   dula=1;
   dula=0;
   
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[g_c];
   dula=1;
   dula=0;
   
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);
}

void main()
{
	TMOD=0x01;
	TR0=1;
	TH0=(65536-46080)/256; 
	TL0=(65536-46080)%256;//50000*11.0592/12
	ET0=1;

//	IT1=1;//
//	EX1=1;//	
//	IT0=1;//
//	EX0=1;//
	
	EA=1;
	while(1)
	{	
		//green switchs
		if(key_green == 0)
		{
			delay(100);
			shijian = 0;
			deng = 1;
		while(!key_green);
		}
		
		if(key_yellow == 0)
		{
		delay(100);
		shijian = 0;
		deng = 2;
		while(!key_yellow);
		alwaysyellow = !alwaysyellow;
		}
		
		if(shijian==0 && !alwaysyellow)
		{
			deng++;
			deng%=3;
			switch(deng)
			{
				case 0: shijian=5;		  //ºìµÆ
						deng=0;
						break;
				case 1: shijian=5;		  //ÂÌµÆ
						deng=1;
						break;
				case 2: shijian=5;		  //»ÆµÆ
						deng=2;
						break;
//				case 3: shijian=5;		  //»ÆµÆ
//						deng=0;
//						break;
				}
		}
		if(ppp==20)
		{	
			ppp=0;
			shijian--;
		}
		aa0=shijian%10;
		aa1=shijian/10;
		
		if(!alwaysyellow)
		display(aa1,aa0);
		
		
		switch(deng)
		{
			case 0: P1=0X00;			 //ºìµÆ
					MOTOR2=0;
					MOTOR1=0;
					break;
//			case 3: 			//»ÆµÆ
//					if(shijian%2==0)
//			      {
//						  P1=0x00;
//						  beep=1;
//					  }
//					if(shijian%2==1)
//					  {
//							 P1=0xff;
//							 beep=0;
//						}
//					break;
			case 1: 		//ÂÌµÆ
					    MOTOR1=1;
					    MOTOR2=0;
					break;
			case 2:	 //yellow
					if(shijian%2==0)
			      {
						  P1=0x00;
						  beep=1;
					  }
					if(shijian%2==1)
					  {
							 P1=0xff;
							 beep=0;
						}
					break;	
		}
	}
}

void time0() interrupt 1
{
	TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	ppp++;
}
//void green() interrupt 0
//{
//	delay(100);
//	led_green = ~led_green;
//	if(key_green == 0){
//		shijian = 0;
//		deng = 1;
//		while(!key_green);
//	}
//}

//void yellow() interrupt 2
//{
//	
//	delay(100);
//	if(key_yellow == 0){
//		shijian = 0;
//		deng = 2;
//		while(!key_yellow);
//		delay(200);
//	}
//	while(!key_yellow);//night mode, always yellow unless retouch the key_yellow button;
//}
