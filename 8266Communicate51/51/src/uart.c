#include "reg52.H" 
#include "public.h"
#include "uart.h"

char RecvString_buf[16];
//char HeartBeat[16]={datapackage_headflag,deviceID_1Bit,deviceID_2Bit,'_','B','e','a','t','X','X','X','X','X','X','#'};            //����㶨����һ�����ݰ�������"������"�����絥Ƭ��ϵͳ�����ÿ2�뷢��һ�θ����ݰ����������û�а�ʱ���յ�������Ϊ��Ƭ��������
char DataPackage_DS18B20[16]={datapackage_headflag,deviceID_1Bit,deviceID_2Bit,'_','T','e','m','p','X','X','X','X','X','X','#'};    //������������������¶ȴ���ģ�飨DS18B20�������ݰ�
char DataPackage_XPT2046[16]={datapackage_headflag,deviceID_1Bit,deviceID_2Bit,'_','P','r','a','d','X','X','X','X','X','X','#'};    //������������ƹ�����������ݰ�

//----------------------------------------------//
/*******************************
            ����ͨ��
    MCU:89C52RC        11.0592MHz

//11.0592MHz 0xd0 1200bps
//12MHz 0xcc 1200bps
//11.0592MHz 0xfa 9600bps
//0xf4 11.0592MHz  0xf3 12MHz 4800bps
//����SMOD=1������£������ʱ���ģʽ��
*******************************/

//���ڷ��ͺ���
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
//���ڽ��պ���
bit ReceiveString()
{
    char * RecStr=RecvString_buf;
    char num=0;
    unsigned char count=0;
    loop:
    *RecStr=SBUF;
    count=0;
    RI=0;
    if(num<14)  //���ݰ�����Ϊ15���ַ�,������������15���ַ�
    {
        num++;
        RecStr++;
        while(!RI)
        {
            count++;
            if(count>130)return 0;    //�������ݵȴ��ӳ٣��ȴ�ʱ��̫�ûᵼ��CPU�������ã�̫�̻����"���ݰ����ָ�",Ĭ��count=130
        }
        goto loop;
    }
    return 1;
}
//��ʱ��1���������ʷ�����
void Init_USART()
{
    SCON=0x50;  //���ڷ�ʽ1��ʹ�ܽ���
    TMOD|=0x20;  //��ʱ��1������ʽ2��8λ�Զ���װ��ֵ��
    TMOD&=~0x10;
    TH1=0xfa;   //9600bps
    TL1=0xfa;
    PCON|=0x80;  //SMOD=1
    TR1=1;
    TI=0;
    RI=0;
    //PS=1;   //��ߴ����ж����ȼ�
    ES=1;  //���������ж�ʹ��
    EA=1;  //���ж�
}
//�Ƚ�ָ��ͷ��
bit CompareCMD_head(char CMD_head[])
{
    unsigned char CharNum;
    for(CharNum=0;CharNum<4;CharNum++)  //ָ���Ϊ10���ַ�
    {
        if(!(RecvString_buf[CharNum+4]==CMD_head[CharNum]))
        {
            return 0;  //ָ��ͷ��ƥ��ʧ��
        }
    }
    return 1;        //ָ��ͷ��ƥ��ɹ�
}
//�Ƚ�ָ��β��(start:�����￪ʼ�Ƚϣ�quality:�Ƚ϶��ٸ��ַ���CMD_tail[]��Ҫ�Ƚϵ��ַ���)
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
bit Deal_UART_RecData()   //�����ڽ������ݰ��������ɹ��������ݰ��򷵻�1�����򷵻�0��
{
    //PutString(RecvString_buf);
    if(RecvString_buf[0]==datapackage_headflag&&RecvString_buf[14]=='#')  //�������ݰ�ͷβ�����֤
    {
        switch(RecvString_buf[1])        //ʶ�������豸ID�ĵ�1λ����
        {
            case '0':
                switch(RecvString_buf[2])        //ʶ�������豸ID�ĵ�2λ����
                {
                    case '1':
                        if(CompareCMD_head("Ligt"))    //�ж�ָ��ͷ���Ƿ�Ϊ"Ligt"
                        {
                            //������ָ��β������
                            switch(RecvString_buf[8])
                            {
                                case '0':
                                    switch(RecvString_buf[9])
                                    {
                                        case '0':

                                            return 0;
                                        case '1':
                                            if(CompareCMD_tail(10,3,"Off"))   //�ж��������ݰ��Ƿ�Ϊ��A03_Ligt01Off_#
                                            {
                                                //�������ִ�����´���
                                                return 1;
                                            }
                                            if(CompareCMD_tail(10,3,"On_"))    //�ж��������ݰ��Ƿ�Ϊ��A03_Ligt01On__#
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


//�����жϷ�����
void USART() interrupt 4   //��־λTI��RI��Ҫ�ֶ���λ��TI��RI��λ����һ���ж����
{
    if(ReceiveString())
    {
        //���ݰ�������ȷ��ִ�����´���
        Deal_UART_RecData();
    }
    else
    {
        //���ݰ����ȴ�����ִ�����´���
        led=~led;
    }
    RI=0;  //���ղ�����һ�����ݺ�ѽ����жϱ�־���һ�£��ܾ���Ӧ���жϽ���æ��ʱ����������
}