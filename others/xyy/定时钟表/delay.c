#include"delay.h"

uchar j,k;
void delay(uchar i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}