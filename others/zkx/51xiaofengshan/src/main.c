#include<reg52.h>


sbit ENA=P1^0;
sbit led=P2^0;
sbit ld=P2^1;

sbit key=P3^2;

typedef unsigned char uint8;
typedef unsigned char uint16;
uint8 count;
bit flag1s=0;//1s��ʱ��־
unsigned char T0RH=0;
unsigned char T0RL=0;

int temp;//��ȡ���ĵ�ǰ�¶�ֵ
unsigned char len;
int intT,decT;//�¶�ֵ��������С������
unsigned char str[12];

void Compare();
void GetTemp();
void ConfigTimer0(unsigned int ms);
unsigned char IntToString(unsigned char *str,int dat);
extern bit Start18B20();
extern bit Get18B20Temp(int *temp);
extern void InitLcd1602();
extern void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);
void Int0Init()
{
	//����INT0
	IT0 = 1; //�����س�����ʽ���½��أ�
	EX0 = 1; //��INT0���ж�����
	
}

void init_time0(){
TMOD=0X01;
TH0=0X4C;
TL0=0X00;
TR0=1;
}
void main()
{
   	unsigned int i=0;
	bit res;
	Int0Init();
	EA=1;
	ConfigTimer0(10);//T0��ʱ10ms
	Start18B20();//����DS18B20
	InitLcd1602();//��ʼ��Һ��
    
    init_time0();
	while(1)
	{	
	if(TF0==1){

	TF0=0;
	TH0=0X4C;
    TL0=0X00;
	count++;
	}
	
	
		if(flag1s)//ÿ�����һ���¶�
		{
			flag1s=0;
			res=Get18B20Temp(&temp);//��ȡ��ǰ�¶�
			if(res)//��ȡ�ɹ�ʱ��ˢ�µ�ǰ�¶���ʾ
			{
				GetTemp();
			
				LcdShowStr(0,0,"Welcome to use");//��ʾ�ַ����¶�ֵ
				LcdShowStr(0,1,"Current T:");
				LcdShowStr(10,1,str);
					Compare();
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
	unsigned char j;

	if((intT>= 24) && (intT<26))   //������Ϊһ���²Χ�������¶ȷ�Χ����
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
	switch(j)		  //�����¶��������õ��ת��
	{
		case 0:	
		  		for(i=0;i<200;i++)
	      		{
					ENA=1;
	     			delay(10);
	      		    ENA=0;
					delay(100);
				}
				break;
	
		case 1:	
		  		for(i=0;i<200;i++)
	      		{
					ENA=1;
	     			delay(30);
	      		    ENA=0;
					delay(100);
				}
				break;	 
	
		case 2:	
		  		for(i=0;i<200;i++)
	      		{
					ENA=1;
	     			delay(55);			 
	      		    ENA=0;
					delay(100);
				}
				break;	 
							
		case 3:	
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

	tmp=11059200/12;//��ʱ������Ƶ��
	tmp=(tmp*ms)/1000;//��������ļ���ֵ
	tmp=65536-tmp;//���㶨ʱ������ֵ
	tmp=tmp+12;//�����ж���ʱ��ɵ����
	T0RH=(unsigned char)(tmp>>8); //��ʱ������ֵ���Ϊ�ߵ��ֽ�
	T0RL=(unsigned char)tmp;
	TMOD &= 0xF0;//����T0�Ŀ���λ
	TMOD |= 0x01;//����T0Ϊģʽ1
	TH0=T0RH;//����T0����ֵ
	TL0=T0RL;
	ET0=1;//ʹ��T0�ж�
	TR0=1; //����T0
}
/* T0�жϷ����������1s��ʱ*/
void InterruptTimer0() interrupt 1
{
	static unsigned char tmr1s=0;

	TH0=T0RH;//���¼�������ֵ
	TL0=T0RL;
	tmr1s++;
	if(tmr1s>=100) //��ʱ1s
	{
		tmr1s=0;
		flag1s=1;
	}		 

}
void Int0() interrupt 0 //�ⲿ�ж�0���жϺ���
{
	delay(1000); //��ʱ����
	if (key == 0)
	{
		ENA = ~ENA;
		while(!key);
	}
}
