#include "reg52.H" 
#include "public.h"
#include "uart.h"
#include "tempe.h"
#include "XPT2046.h"
#include "lcd.h"

void main(){
    Init_USART(); 
    LcdInit();			 //初始化LCD1602
	LcdWriteCom(0x88);	//写地址 80表示初始地址
	LcdWriteData('C');  
    //数据包 样例
    //$02_Temp025.81#
    //$02_Prad0485XX#
    while(1)
    {       
        sendtemp();//温度值传送
		//DigDisplay();//数码管显示函数
        
        sendprad();//光敏值传送
        Delay1ms(10);
    }
}
