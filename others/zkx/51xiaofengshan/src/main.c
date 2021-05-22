#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit DQ = P1 ^ 0;
sbit key2 = P3 ^ 2;
sbit key3 = P3 ^ 3;
sbit P3_4 = P3 ^ 4;
sbit P3_5 = P3 ^ 5;
float ff;
uint y3, last;
uchar shi, ge, xiaoshu, yushe = 25;
uchar code dispcode[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71}; //段码
uchar code tablel[] = {
    0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0x87, 0xff, 0xef}; //带小数点的段码
uchar dispbitcode[] = {0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};
uchar dispbuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void Delay(uint num) //延时函数
{
    while (--num)
        ;
}
void digitalshow(uchar a4, uchar a3, uchar a2, uchar a1, uchar a0)
{
    dispbuf[0] = a0;
    dispbuf[1] = a1;
    dispbuf[2] = a2;
    dispbuf[3] = a3;
    dispbuf[4] = a4;
    P2 = 0xff;
    P0 = dispcode[dispbuf[0]];
    P2 = dispbitcode[5];
    Delay(1);
    P2 = 0xff;
    P0 = dispcode[dispbuf[1]];
    P2 = dispbitcode[4];
    Delay(1);
    P2 = 0xff;
    P0 = dispcode[dispbuf[2]];
    P2 = dispbitcode[2];
    Delay(1);
    P2 = 0xff;
    P0 = tablel[dispbuf[3]];
    P2 = dispbitcode[1];
    Delay(1);
    P2 = 0xff;
    P0 = dispcode[dispbuf[4]];
    P2 = dispbitcode[0];
    Delay(1);
}
void dmsec(uint count)
{
    uint i; //1ms延时
    while (count--)
    {
        for (i = 0; i < 125; i++)
        {
        }
    }
}
void tmreset(void)
{
    DQ = 0;
    Delay(90); //精确延时 大于480微秒
    DQ = 1;
    Delay(4); //90,4可以小范围变化
}
void tmpre(void)
{
    while (DQ)
        ;
    while (~DQ)
        ;
    Delay(4);
}
bit tmrbit(void)
{
    uint i;
    bit dat;
    DQ = 0;
    i++; //i++;大概1微秒
    DQ = 1;
    i++;
    i++;
    dat = DQ;
    Delay(8);
    return (dat);
}
uchar tmrbyte(void) //读一个比特
{
    uchar i, j, dat;
    dat = 0;
    for (i = 1; i <= 8; i++)
    {
        j = tmrbit();
        dat = (j << 7) | (dat >> 1);
        ;
    }
    return (dat);
}
void tmwbyte(uchar dat) //写一个比特
{
    uint i;
    uchar j;
    bit testb;
    for (j = 1; j <= 8; j++)
    {
        testb = dat & 0x01;
        dat = dat >> 1; //从低位开始
        if (testb)      //write 1
        {
            DQ = 0; //先拉低
            i++;
            i++; //>1微秒
            DQ = 1;
            Delay(4);
        }
        else //write 0
        {
            DQ = 0;
            Delay(4);
            DQ = 1;
            i++;
            i++; //再拉高
        }
    }
}
void tmstart(void) //ds1820开始转换
{
    dmsec(1);
    tmreset();
    tmpre();
    dmsec(1);
    tmwbyte(0xcc);
    // rom
            tmwbyte(0x44); //转换
}
uchar tmrtemp(void) //读取温度
{
    uchar a, b;
    tmreset();
    tmpre();
    dmsec(1);
    tmwbyte(0xcc);
    // rom
            tmwbyte(0xbe); //转换
    a = tmrbyte();         //LSB低八位
    b = tmrbyte();         //MSB高8位
    y3 = b;
    y3 <<= 8;
    y3 = y3 | a;
    ff = y3 * 0.0625;
    y3 = ff * 10 + 0.5;
    return (y3);
}
void keyscan(void)
{
    if (key2 == 0)
    {
        dmsec(5);
        if (key2 == 0)
        {
            yushe++;
            if (yushe == 100)
                yushe = 25;
        }
        while (!key2)
            ;
    }
    else if (key3 == 0)
    {
        dmsec(5);
        if (key3 == 0)
        {
            yushe--;
            if (yushe == 0)
                yushe = 25;
        }
        while (!key3)
            ;
    }
}
void time0_int(void) interrupt 1 //定时器T0产生不同占空比的PWM
{
    uint tmp;
    static unsigned char j;
    TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;
    j = j + 1;
    if (tmp <= yushe)
    {
        P3_4 = 0;
        j = 0; //占空比00%
    }
    else if ((tmp > yushe) && (tmp <= (yushe + 5)))
    {
        if (j == 4)
            P3_4 = 0;
        else if (j == 20)
            P3_4 = 1;
        j = 0;
    } //占空比为20%
    else if ((tmp > (yushe + 5)) && (tmp <= (yushe + 10)))
    {
        if (j == 8)
            P3_4 = 0;
        else if (j == 20)
            P3_4 = 0;
        else if (j == 20)
            P3_4 = 1;
        j = 0;
    } //占空比为40%
    else if ((tmp > (yushe + 10)) && (tmp <= (yushe + 15)))
    {
        if (j == 12)
            P3_4 = 0;
        else if (j == 20)
            P3_4 = 1;
        j = 0;
    } //占空比为60%
    else if ((tmp > (yushe + 15)) && (tmp <= (yushe + 20)))
    {
        if (j == 16)
            P3_4 = 0;
        else if (j == 20)
            P3_4 = 1;
        j = 0;
    } //占空比为80%
    else if (tmp > (yushe + 20))
        P3_4 = 1;
    j = 0; //占空比为100%
}
/******
void time1_int(void)interrupt 1//定时器T1产生频率为22KHz的驱蚊超声波{
P3^5 != P3_5;
TH0=(65536-2500)/256;
TL0=(65536-2500)%256;
}**/
void main(void)
{
    TMOD = 0101; //定时器初始化
    TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;
    TH1 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;
    EA = 1;
    ET0 = 1;
    ET1 = 1;
    TR0 = 1;
    TR1 = 1;
    //uint last;
    tmstart();
    dmsec(450); //初始化ds18b20
    while (1)
    {
        tmstart(); //ds1820开始转换
        dmsec(2);
        last = tmrtemp() + 256; //读取温度
        shi = last / 100;
        ge = (last % 100) / 10;
        xiaoshu = (last % 100) % 10;
        digitalshow(shi, ge, xiaoshu, yushe / 10, yushe % 10);
        keyscan();
        dmsec(2);
        //deal(last/10);
    }
}
