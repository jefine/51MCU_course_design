#include<reg52.h>

// sbit IN1=P0^0;
// sbit IN2=P0^0;
sbit ENA=P1^0;
sbit key = P3^2;
sbit keyadd = P3^1;
sbit keysub = P3^0;
sbit keyauto = P3^3;
sbit led0 = P2^0;
sbit led1 = P2^1;
sbit led2 = P2^2;
sbit led3 = P2^3;

bit flag1s=0;//1s��ʱ��־
unsigned char T0RH=0;
unsigned char T0RL=0;
int unset = 1;
int temp;//��ȡ���ĵ�ǰ�¶�ֵ
unsigned char len;
int intT,decT;//�¶�ֵ��������С������
unsigned char str[12];
unsigned char j;
void Int0();
void Int0Init();
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
	
	Start18B20();//����DS18B20
	
	InitLcd1602();//��ʼ��Һ��
	EA=1;
	Int0Init();
	ConfigTimer0(10);//T0��ʱ10ms
	

	while(1)
	{		
		Compare();
		if(flag1s)//ÿ�����һ���¶�
		{
			flag1s=0;
			res=Get18B20Temp(&temp);//��ȡ��ǰ�¶�
			if(1)//��ȡ�ɹ�ʱ��ˢ�µ�ǰ�¶���ʾ
			{
				GetTemp();
				LcdShowStr(0,0,"Welcome to use");//��ʾ�ַ����¶�ֵ
				LcdShowStr(0,1,"Current T:");
				LcdShowStr(10,1,str);
				delay(20);
					
			}
			else //��ȡʧ��ʱ����ʾ������Ϣ
			{
				LcdShowStr(0,0,"error!");
			}
			Start18B20();//����������һ��ת��					 
		}
	}
}
/*�¶Ȼ�ȡ��������ȡ��ǰ�����¶�ֵ��������str������*/
void GetTemp()
{

	intT=temp>>4;//������¶�ֵ��������
	decT=temp &0x0F;//������¶�ֵС������
			
	len=IntToString(str,intT);//��������ת�����ַ���
			
	str[len++]='.';
	decT=(decT*10)/16;//�����Ƶ�С������ת��Ϊ1λʮ����λ
	str[len++]=decT+'0';//ʮ����С��λ��ת��ΪASCII�ַ�
	while(len<6)//�ÿո��뵽6���ַ�����
	{
		str[len++]=' ';
	}
	str[len++]='\0';
}
/*��ʱ����������PWM����*/
void delay(unsigned int z)
{
	unsigned int x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
} 
/*�ȽϺ�����ͨ���¶�ֵ�ıȽ����õ����ת��*/
void Compare()
{
	unsigned int i=0;
	
	if(unset)
	{
		if(intT<24){
			j=0;
		}
		else if((intT>= 24) && (intT<26))   //������Ϊһ���²Χ�������¶ȷ�Χ����
		{
			j=1;	
		}
		else if((intT>=26) &&(intT<28))
		{
			j=2;
		}
		else if((intT>=28) &&(intT<30))
		{
			j=3;
		}
		else if(intT>=30)
		{
			j=4;
		}
	}

	

	switch(j)		  //�����������õ��ת��
	{
		case 0:{//stop
			ENA = 0;
			break;
		}
		case 1:	{//25%
				// IN1=1;
				// IN2=0;
		  		for(i=0;i<200;i++)
	      		{
					ENA=1;
	     			delay(10);
	      		    ENA=0;
					delay(30);
				}
				break;
				}	
	
		case 2:	//50%
				// IN1=1;
				// IN2=0;
		  		for(i=0;i<200;i++)
	      		{
					ENA=1;
	     			delay(30);
	      		    ENA=0;
					delay(30);
				}
				break;	 
	
		case 3:	//75%
				// IN1=1;
				// IN2=0;
		  		for(i=0;i<200;i++)
	      		{
					ENA=1;
	     			delay(75);			 
	      		    ENA=0;
					delay(25);
				}
				break;	 
							
		case 4:	
				// IN1=1;
				// IN2=0;
		  	    ENA=1;
				break;

		default:break;	 	 
	}
}  

/*������ת��Ϊ�ַ�����str-�ַ���ָ�룬dat-��ת����������ֵ-�ַ�������*/
unsigned char IntToString(unsigned char *str,int dat)
{
	signed char i=0;
	unsigned char len=0;
	unsigned char buf[6];

	if(dat<0)//���Ϊ����������ȡ����ֵ������ָ������Ӹ���
	{
	 	dat=-dat;
		*str++='-';
		len++;
	}
	do{	   //��ת��Ϊ��λ��ǰ��ʮ��������
		buf[i++]=dat%10;
		dat /=10;
	}while(dat>0);
	len += i;//i����ֵ������Ч�ַ��ĸ���
	while(i-->0)//������ֵת��ΪASCII�뷴�򿽱�������ָ����
	{
		*str++=buf[i]+'0';
	}
	*str='\0';
	return len;
}
void ConfigTimer0(unsigned int ms)
{
	unsigned long tmp;

	tmp=11059200/12;
	tmp=(tmp*ms)/1000;
	tmp=65536-tmp;
	tmp=tmp+12;
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TH0=T0RH;
	TL0=T0RL;
	ET0=1;
	TR0=1;
}
void InterruptTimer0() interrupt 1
{
	static unsigned char tmr1s=0;
	led0 = 1;
	led1 = 1;
	led2 = 1;
	
	if(j==1)led0 = 0;
	if(j==2)led1 = 0;
	if(j==3)led2 = 0;
	TH0=T0RH;
	TL0=T0RL;
	tmr1s++;
	if(tmr1s>=100)
	{
		tmr1s=0;
		flag1s=1;
	}		 

}

void Int0Init()
{
	//����INT0
	IT0 = 0; //�����س�����ʽ���½��أ�
	EX0 = 1; //��INT0���ж�����
}

void Int0() interrupt 0 //�ⲿ�ж�0���жϺ���
{
	unset = 0;

	if(keysub==0){
		
		delay(100);
		j=1;
		while(keysub==0);
	}
	if(keyadd==0){
		
		delay(100);
		j=3;
		while(keyadd==0);
	}
	if (keyauto == 0)
	{
		delay(100);
		
		if(j!=0){
			j=0;
			ENA = 0;
			led1 = 0;
		}
		else {
			j=4;
			ENA = 1;
			led1 = 1;
		}
		while(keyauto==0);
	}
}
