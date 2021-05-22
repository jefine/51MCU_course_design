#include "reg52.H" 
#include "public.h"
#include "uart.h"

char RecvString_buf[16];
//char HeartBeat[16]={datapackage_headflag,deviceID_1Bit,deviceID_2Bit,'_','B','e','a','t','X','X','X','X','X','X','#'};            //我随便定义了一个数据包用来做"心跳包"，比如单片机系统向电脑每2秒发送一次该数据包，如果电脑没有按时接收到，就认为单片机死掉了
char DataPackage_DS18B20[16]={datapackage_headflag,deviceID_1Bit,deviceID_2Bit,'_','T','e','m','p','X','X','X','X','X','X','#'};    //这个是曾经用来控制温度传感模块（DS18B20）的数据包
char DataPackage_XPT2046[16]={datapackage_headflag,deviceID_1Bit,deviceID_2Bit,'_','P','r','a','d','X','X','X','X','X','X','#'};    //这个是用来控制光敏电阻的数据包

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
bit ReceiveString()
{
    char * RecStr=RecvString_buf;
    char num=0;
    unsigned char count=0;
    loop:
    *RecStr=SBUF;
    count=0;
    RI=0;
    if(num<14)  //数据包长度为15个字符,尝试连续接收15个字符
    {
        num++;
        RecStr++;
        while(!RI)
        {
            count++;
            if(count>130)return 0;    //接收数据等待延迟，等待时间太久会导致CPU运算闲置，太短会出现"数据包被分割",默认count=130
        }
        goto loop;
    }
    return 1;
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
    //PutString(RecvString_buf);
    if(RecvString_buf[0]==datapackage_headflag&&RecvString_buf[14]=='#')  //进行数据包头尾标记验证
    {
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
                                            if(CompareCMD_tail(10,3,"Off"))   //判断整个数据包是否为：A03_Ligt01Off_#
                                            {
                                                //如果是则执行以下代码
                                                return 1;
                                            }
                                            if(CompareCMD_tail(10,3,"On_"))    //判断整个数据包是否为：A03_Ligt01On__#
                                            {
                                                PutString("received right led5 on\n");
                                                led5 = ~led5;
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
        Deal_UART_RecData();
    }
    else
    {
        //数据包长度错误则执行以下代码
        led=~led;
    }
    RI=0;  //接收并处理一次数据后把接收中断标志清除一下，拒绝响应在中断接收忙的时候发来的请求
}