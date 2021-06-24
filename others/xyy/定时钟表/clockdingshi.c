#include<reg51.h>
#include"display.h"
#include"delay.h"
#include"keyscan.h"

uchar a1,a0,b1,b0,c1,c0,s,f,m,qq;
uchar baoshi,baoshijieshu;

void main()
{
	TMOD=0x01;
	TH0=(65536-46080)/256;// ���ھ���Ϊ11.0592,�����Ǵ���ӦΪ46080����ʱ��ÿ��50000΢�뷢��һ���жϡ�
	TL0=(65536-46080)%256;//46080  ����50000*11.0592/12
	ET0=1;
	EA=1;
	
	while(1){
		keyscan0();
		
		if(ok==1)
		{	TR0=1;
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
					if(s==24)  //24hһ��ѭ��
					{
						s=0;
					}
				}
			}
		}
		
		a0=s%10;//��ģ
		a1=s/10; //����
		b0=f%10;
		b1=f/10;
		c0=m%10;
		c1=m/10;
		display(a1,a0,b1,b0,c1,c0);
	}
	else
	{
	if(change==1)
	{	TR0=0;
		keyscan();
		if(key!=10)
		{
		
		switch(wei)
		{
			case 1: if(key<3)		//Сʱ���λΪ2
					a1=key;
					else
					wei--;
					break;
			case 2: if(a1==1|a1==0)
					a0=key;
					else
					if(key<5)
					a0=key;		   //��Сʱ���λΪ2ʱ����λ���Ϊ4
					break;
			case 3: if(key<6)		//�������λΪ5
					b1=key;
					else
					wei--;
					break;
			case 4: b0=key; break;
			case 5: if(key<6)		//�����λΪ5
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
	if(dingshi==1)
	{
		TR0=0;
		display(shi20,shi10,fen20,fen10,miao20,miao10);	
		keyscan();
		if(key!=10)
		{
		switch(wei)
		{
			case 1: if(key<3)		//Сʱ���λΪ2
					shi20=key;
					else
					wei--;
					break;
			case 2: if(a1==1|a1==0)
					shi10=key;
					else
					if(key<5)
					a0=key;		   //��Сʱ���λΪ2ʱ����λ���Ϊ4
					break;
			case 3: if(key<6)		//�������λΪ5
					fen20=key;
					else
					wei--;
					break;
			case 4: fen10=key; break;
			case 5: if(key<6)		//�����λΪ5
					miao20=key; 
					else
					wei--;
					break;
			case 6: miao10=key; break;
		}
		key=10;
		}
		yidingshi=1;
		display(shi20,shi10,fen20,fen10,miao20,miao10);	
	}	
	}
	if(yidingshi==1)
	{
	if((a1==shi20)&&(a0==shi10)&&(b1==fen20)&&(b0==fen10)&&(c1==miao20)&&(c0==miao10))
		beep=0;
	}
	if(((a1==shi20)&&(a0==shi10)&&(b1==fen20)&&(b0==fen10+1)&&(c1==miao20)&&(c0==miao10))||(yidingshi==0))//һ���ӱ�ʱ��ʾ
		beep=1;
	}
}

void time0() interrupt 1   //50ms
{	TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	pp++;
}
