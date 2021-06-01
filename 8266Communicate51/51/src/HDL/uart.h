#ifndef UART_H
#define UART_H

extern char RecvString_buf[16];            //定义数据包长度为15个字符
#define deviceID_1Bit '0'                //用于串口通信时，定义本地设备ID的第1位
#define deviceID_2Bit '2'                //用于串口通信时，定义本地设备ID的第2位
#define datapackage_headflag '$'        //用于串口通信时，定义数据包头部的验证标记
extern char DataPackage_DS18B20[16];
extern char DataPackage_XPT2046[16];
extern int number;
//------------------串口通信的数据包协议-----------------//
/*
    此程序的串口字符串通信使用到下面的一个自定义协议，每次通信都是发送或接收一个数据包，数据包格式解释如下(长度恒为15):
    例如:A01_fmq_01Off___#
    A--------数据包的开始标记(可以为A到Z,意味着数据包可以有26种)
    01-----设备代号
    fmq_01Off___--------指令(长度恒为10)，指令的前4个人字符是指令头部，指令的后6个字符是指令尾部
    #---------数据包的结束标记

    例如:A02_SenT010250#
    A--------数据包的开始标记(可以为A到Z,意味着数据包可以有26种)
    02-----设备代号
    SenT010250--------指令(长度恒为10)，指令的前4个人字符是指令头部，指令的后6个字符是指令尾部
    #---------数据包的结束标记
*/

//----------------------------------------------//
/*******************************
            串口通信
    MCU:89C52RC        11.0592MHz

//11.0592MHz 0xd0 1200bps
//12MHz 0xcc 1200bps
//11.0592MHz 0xfa 9600bps
//0xf4 11.0592MHz  0xf3 12MHz 4800bps
//均在SMOD=1的情况下（波特率倍增模式）
*******************************/

//串口发送函数
void PutString(unsigned char *TXStr);
//串口接收函数
bit ReceiveString();
//定时器1用作波特率发生器
void Init_USART();
//比较指令头部
bit CompareCMD_head(char CMD_head[]);
//比较指令尾部(start:从哪里开始比较，quality:比较多少个字符，CMD_tail[]：要比较的字符串)
bit CompareCMD_tail(unsigned char start,unsigned char quality,char CMD_tail[]);

bit Deal_UART_RecData();   //处理串口接收数据包函数（成功处理数据包则返回1，否则返回0）
//串口中断服务函数
//void USART() interrupt 4;   //标志位TI和RI需要手动复位，TI和RI置位共用一个中断入口

#endif