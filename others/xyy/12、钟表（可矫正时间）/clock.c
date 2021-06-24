/***********************************************************************************
				
					  	“天祥电子”	
						  倾情奉献
						www.txmcu.com
						www.txmcu.cn

*******************************************************************

功能简介：

该例程为数码管显示可调钟表（无定时功能），用到TX-1C开发板中矩阵键盘的上三排按键。
若要更改时钟初始值（即时钟校准，因为时钟初始值全为0），可先按下矩阵键盘中的S16键
（第三排第三列），此时时钟会暂停。
然后按S6---S15中的键修改矫正当前值，S6--S15分别先后对应数字0到9。
矫正完后，按下S17（第三排最后一个），时钟可继续工作。
注意晶振需为11.0592M。
若为其他数值晶振，请合理改变TH0与TL0参数值，否则计时会有很大误差 。

********************************************************************************/
#include<reg51.h>

#define uchar unsigned char

sbit dula=P2^6;
sbit wela=P2^7;
sbit beep=P2^6;//23
unsigned char j,k,a1,a0,b1,b0,c1,c0,s,f,m,key=10,temp,qq;
uchar shi20,shi10,fen20,fen10,miao20,miao10,new,ok=1,wei;
unsigned int pp;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

sbit MOTOR1=P1^0;
sbit MOTOR2=P1^1;

unsigned int ppp;
												
uchar jj,kk,l=255;
uchar aa1,aa0;
uchar shijian,deng=2;

void delay(unsigned char i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}
void display(uchar shi2,uchar shi1,uchar fen2,uchar fen1,uchar miao2,uchar miao1)
{
   dula=0;
   P0=table[shi2];
   dula=1;
   dula=0;
   
   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[shi1]|0x80;
   dula=1;
   dula=0;
   
   P0=0xfd;
   wela=1;
   wela=0;
   delay(5);

   P0=table[fen2];
   dula=1;
   dula=0;
   
   P0=0xfb;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[fen1]|0x80;
   dula=1;
   dula=0;
   
   P0=0xf7;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[miao2];
   dula=1;
   dula=0;
   
   P0=0xef;
   wela=1;
   wela=0;
   delay(5);
   
   P0=table[miao1];
   dula=1;
   dula=0;
   
   P0=0xdf;
   wela=1;
   wela=0;
   delay(5);
}

void keyscan0()
{
    P3=0xfb;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xbb:
               ok=0;
               break;

          case 0x7b:
               ok=1;
               break;
         }
      }
      }	
}


void keyscan()
{
  {	
    P3=0xfe;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {	
        temp=P3;
        switch(temp)
        {
          case 0xee:
               key=0;
			   wei++;
               break;

          case 0xde:
               key=1;
			   wei++;
               break;

          case 0xbe:
               key=2;
			   wei++;
               break;

          case 0x7e:
               key=3;
			   wei++;
               break;
         }
         while(temp!=0xf0) 
        {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
    }
    P3=0xfd;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xed:
               key=4;
			   wei++;
               break;

          case 0xdd:
               key=5;
			   wei++;
               break;

          case 0xbd:
               key=6;
			   wei++;
               break;

          case 0x7d:
               key=7;
			   wei++;
               break;
         }
         while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
      }
    P3=0xfb;
    temp=P3;
    temp=temp&0xf0;
    if(temp!=0xf0)
    {
      delay(10);
      if(temp!=0xf0)
      {
        temp=P3;
        switch(temp)
        {
          case 0xeb:
               key=8;
			   wei++;
               break;

          case 0xdb:
               key=9;
			   wei++;
               break;
         }
        while(temp!=0xf0)
         {
           temp=P3;
           temp=temp&0xf0;
           beep=0;
         }
         beep=1;
      }
      }
}
}

void main()
{
	
	
	//TMOD=0x01;

	TH0=(65536-46080)/256;// 由于晶振为11.0592,故所记次数应为46080，计时器每隔50000微秒发起一次中断。
	TL0=(65536-46080)%256;//46080的来历，为50000*11.0592/12
	ET0=1;
//	EA=1;
	
	//TMOD|=0X10;
	TMOD = 0X11;

	TH1=(65536-46080)/256;	//
	TL1=(65536-46080)%256;	
	ET1=1;//
	EA=1;//
	//TR1=1;//
	
	while(1)
	{	keyscan0();
		
		if(shijian==0)
		{
			switch(deng)
			{
				case 0: shijian=5;		  //oìμ?
						deng=1;
						break;
//				case 1: shijian=5;		  //??μ?
//						deng=2;
//						break;
				case 2: shijian=5;		  //?ìμ?
						deng=3;
						break;
				case 3: shijian=5;		  //??μ?
						deng=0;
						break;
			
				}
		}
		if(ppp==20)
		{	
			ppp=0;
			shijian--;
		}
		aa0=shijian%10;
		aa1=shijian/10;
//		display(aa1,aa0);
		switch(deng)
		{
			case 1: 
				//P1=0X00;			 //oìμ?
					break;
			case 2: 			//??μ?
					if(shijian%2==0)
			      {
					//	  P1=0x00;
						  beep=1;
					  }
					if(shijian%2==1)
					  {
						//	 P1=0xff;
							 beep=0;
						}
					break;
			case 3: 		//?ìμ?
					if(shijian%2==0)
					  {
						//  P1=0x55;
					    MOTOR1=1;
					    MOTOR2=0;
						}
					if(shijian%2==1)
				  	{
						//  P1=0xaa;
					    MOTOR2=1;
					    MOTOR1=0;
					  }
					break;
			case 0:	if(shijian%2==0)	  //??μ?
			      {
					//		P1=0x00;
			        beep=1;
			      }
					if(shijian%2==1)
				  	{
					  // P1=0xff;
					   beep=0;
					  }
					break;	
		}
	
		if(ok==1)
		{	TR0=1;
			TR1=1;
			wei=0;
		
		if(pp==20)
		{	pp=0;
			m++;
			if(m==60)
			{
				m=0;
				f++;
				if(f==60)
				{
					f=0;
					s++;
					if(s==24)  //为24h一个循环，若要12h，只需在此改为12即可。
					{
						s=0;
					}
				}
			}
		}
		
		a0=s%10;
		a1=s/10;
		b0=f%10;
		b1=f/10;
		c0=m%10;
		c1=m/10;
		display(a1,a0,b1,b0,c1,c0);
	}
	else
	{	TR0=0;
		TR1=0;
		keyscan();
		if(key!=10)
		{
		
		switch(wei)
		{
			case 1: if(key<3)		//小时最高位为2
					a1=key;
					else
					wei--;
					break;
			case 2: if(a1==1|a1==0)
					a0=key;
					else
					if(key<5)
					a0=key;		   //当小时最高位为2时，低位最高为4
					break;
			case 3: if(key<7)		//分钟最高位为6
					b1=key;
					else
					wei--;
					break;
			case 4: b0=key; break;
			case 5: if(key<7)		//秒最高位为6
					c1=key; 
					else
					wei--;
					break;
			case 6: c0=key; break;
		}
		key=10;
		}
			m=c1*10+c0;
			f=b1*10+b0;
			s=a1*10+a0;
		display(a1,a0,b1,b0,c1,c0);
	}	
	}
}

void time0() interrupt 1
{	
	TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	pp++;
}
void Timer1() interrupt 3
{
	TH1=(65536-46080)/256;
	TL1=(65536-46080)%256;
	ppp++;
}
