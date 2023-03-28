#ifndef __DS1302_H
#define __DS1302_H

#include <STC15F2K60S2.H>
#include <intrins.h>

void Write_Ds1302(unsigned char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte( unsigned char address );

void Ds1302_XieData(unsigned char shi,fen,miao);
unsigned char Ds1302_GetShi();
unsigned char Ds1302_GetFen();
unsigned char Ds1302_GetMiao();

#endif
