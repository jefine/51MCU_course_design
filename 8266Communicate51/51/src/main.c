#include "reg52.H" 
#include "public.h"
#include "uart.h"
#include "tempe.h"
#include "XPT2046.h"
#include "lcd.h"

void main(){
    Init_USART(); 
    LcdInit();			 //Init LCD1602
	LcdWriteCom(0x88);	//80 is the first address
	LcdWriteData('C');  
    //Example to Esp8266
    //$02_Temp025.81#
    //$02_Prad0485XX#
    while(1)
    {       
        sendtemp();
		//DigDisplay();//smg send
        
        sendprad();//
        Delay1ms(100);
    }
}
