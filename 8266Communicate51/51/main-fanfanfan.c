#include "reg52.H"

sbit led = P2 ^ 0; //LED�ӿ�ΪP1.0

bit flag = 0; //LEDʹ�ܱ�־λ����ʼ��Ϊ0

void main() //������
{
	//��ʼ��UART
	SCON = 0x50; //���ô���Ϊ������ʽ1��8λ/֡��REN=1���������
	TCON = 0x20; //����T1Ϊ��ʽ2 ��ʱ��
	PCON = 0x80; //PCON.SMOD=1,�����ʱ���
	EA = 1; //���ж�ʹ��
	ES = 1; //�����ж�ʹ��
	TH1 = 0xF3; //T1��ֵ�����ڲ�����4800bps@12MHz
	TL1 = 0xF3;
	TR1 = 1; //������ʱ��T1
	led = 1; //��ʼ��LED��
	while (1); //��ѭ��
}

//�����жϴ�����
//�жϺ�Ϊ4
void ISR_Serial() interrupt 4
{
	if (RI == 1) //�ɽ������𴮿��жϣ�ȡ��
	{
		//led = 0;
		flag = SBUF; //ȡflag
		led = ~flag; // flagΪ1ʱ�����0������LED��flagΪ0ʱ�����1��Ϩ��LED
		RI = 0; //�������
	}
}