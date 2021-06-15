#include "reg52.H" 
#include "public.h"
#include "uart.h"
#include "lcd.h"
#include <string.h>
char RecvString_buf[16];
int number=0;
//char HeartBeat[16]={datapackage_headflag,deviceID_1Bit,deviceID_2Bit,'_','B','e','a','t','X','X','X','X','X','X','#'};            //心跳包
char DataPackage_DS18B20[16]={datapackage_headflag,deviceID_1Bit,deviceID_2Bit,'_','T','e','m','p','X','X','X','X','X','X','#'};    //控制温度传感模块（DS18B20）的数据包
char DataPackage_XPT2046[16]={datapackage_headflag,deviceID_1Bit,deviceID_2Bit,'_','P','r','a','d','X','X','X','X','X','X','#'};    //控制光敏电阻的数据包


//串口发送函数
void PutString(unsigned char *TXStr)
{
    ES=0;
     while(*TXStr!=0)
    {
        SBUF=*TXStr;
        while(TI==0);
        TI=0;
        TXStr++;
    }
    ES=1;
}
//串口接收函数
// 51接收8266发送来的数据（目的是点灯）
// 8266 所发送数据包样例
// $01_Ligt01Off_#
// $01_Ligt01On__#
bit ReceiveString()
{
    char * RecStr=RecvString_buf;
    char num=0;
    unsigned char count=0;
    loop:
    *RecStr=SBUF;
    if(RecvString_buf[0]!=datapackage_headflag)return 0;
    count=0;
    RI=0;
    if(num<14)  //数据包长度为15个字符,尝试连续接收15个字符
    {
        num++;
        RecStr++;
        while(!RI)
        {
            // count++;
            // if(count>130)return 0;    //接收数据等待延迟，等待时间太久会导致CPU运算闲置，太短会出现"数据包被分割",默认count=130
        }
        goto loop;
    }
    if(RecvString_buf[14]=='#'){
     PutString("\n it really has the String reveiced \n");    
     PutString(RecvString_buf);
     return 1;
    }
    
    return 0;
    
    
    
}
//定时器1用作波特率发生器
void Init_USART()
{
    SCON=0x50;  //串口方式1，使能接收
    TMOD|=0x20;  //定时器1工作方式2（8位自动重装初值）
    TMOD&=~0x10;
    TH1=0xfa;   //9600bps
    TL1=0xfa;
    PCON|=0x80;  //SMOD=1
    TR1=1;
    TI=0;
    RI=0;
    //PS=1;   //提高串口中断优先级
    ES=1;  //开启串口中断使能
    EA=1;  //开中断
}
//比较指令头部
bit CompareCMD_head(char CMD_head[])
{
    unsigned char CharNum;
    for(CharNum=0;CharNum<4;CharNum++)  //指令长度为10个字符
    {
        if(!(RecvString_buf[CharNum+4]==CMD_head[CharNum]))
        {
            return 0;  //指令头部匹配失败
        }
    }
    return 1;        //指令头部匹配成功
}
//比较指令尾部(start:从哪里开始比较，quality:比较多少个字符，CMD_tail[]：要比较的字符串)
bit CompareCMD_tail(unsigned char start,unsigned char quality,char CMD_tail[])
{
    unsigned char CharNum;
    for(CharNum=0;CharNum<quality;CharNum++)
    {
        if(!(RecvString_buf[start+CharNum]==CMD_tail[CharNum]))
        {
            return 0;
        }
    }
    return 1;
}
bit Deal_UART_RecData()   //处理串口接收数据包函数（成功处理数据包则返回1，否则返回0）
{
    if(RecvString_buf[0]==datapackage_headflag && RecvString_buf[14]=='#')  //进行数据包头尾标记验证
    {
        PutString("the package is right");
        switch(RecvString_buf[1])        //识别发送者设备ID的第1位数字
        {
            case '0':
                switch(RecvString_buf[2])        //识别发送者设备ID的第2位数字
                {
                    case '1':
                        if(CompareCMD_head("Ligt"))    //判断指令头部是否为"Ligt"
                        {
                            //下面是指令尾部分析
                            switch(RecvString_buf[8])
                            {
                                case '0':
                                    switch(RecvString_buf[9])
                                    {
                                        case '0':
                                            return 0;
                                        case '1':
                                            if(CompareCMD_tail(10,4,"Off_"))   //判断整个数据包是否为：$01_Ligt01Off_#
                                            {
                                                delay(100);
                                                PutString("\nreceived right led off\n");
                                                led = 1;
                                                return 1;
                                            }
                                            if(CompareCMD_tail(10,4,"On__"))    //判断整个数据包是否为：$01_Ligt01On__#
                                            {
                                                delay(100);
                                                PutString("\nreceived right led on\n");
                                                led = 0;
                                                return 1;
                                            }
                                            return 0;
                                        default:
                                            return 0;
                                    }
                                default:
                                    return 0;
                            }
                        }
                        return 0;

                    default:
                        return 0;
                }
            default:
                return 0;
        }
    }
    return 0;
}


//串口中断服务函数
void USART() interrupt 4   //标志位TI和RI需要手动复位，TI和RI置位共用一个中断入口
{
    if(ReceiveString())
    {
        //数据包长度正确则执行以下代码
        if(Deal_UART_RecData());
        else
        {
            PutString("\n error package \n");
        }
    }
    RI=0;  //接收并处理一次数据后把接收中断标志清除一下，拒绝响应在中断接收忙的时候发来的请求
}