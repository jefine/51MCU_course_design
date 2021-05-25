#include "reg52.H" 
#include "public.h"
#include "uart.h"
#include "tempe.h"
#include "XPT2046.h"
#include "lcd.h"

void main(){
    Init_USART(); 
    LcdInit();			 //��ʼ��LCD1602
	LcdWriteCom(0x88);	//д��ַ 80��ʾ��ʼ��ַ
	LcdWriteData('C');  
    //���ݰ� ����
    //$02_Temp025.81#
    //$02_Prad0485XX#
    while(1)
    {       
        sendtemp();//�¶�ֵ����
		//DigDisplay();//�������ʾ����
        
        sendprad();//����ֵ����
        Delay1ms(10);
    }
}
