#include "reg52.H"

sbit led = P2 ^ 0; //LED接口为P1.0

bit flag = 0; //LED使能标志位，初始化为0

void main() //主程序
{
	//初始化UART
	SCON = 0x50; //设置串口为工作方式1，8位/帧，REN=1，允许接收
	TCON = 0x20; //设置T1为方式2 定时器
	PCON = 0x80; //PCON.SMOD=1,波特率倍增
	EA = 1; //总中断使能
	ES = 1; //串口中断使能
	TH1 = 0xF3; //T1初值，串口波特率4800bps@12MHz
	TL1 = 0xF3;
	TR1 = 1; //启动定时器T1
	led = 1; //初始化LED灭
	while (1); //主循环
}

//串口中断处理函数
//中断号为4
void ISR_Serial() interrupt 4
{
	if (RI == 1) //由接收引起串口中断，取数
	{
		//led = 0;
		flag = SBUF; //取flag
		led = ~flag; // flag为1时，输出0，点亮LED；flag为0时，输出1，熄灭LED
		RI = 0; //软件清零
	}
}