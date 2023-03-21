#include "DSQ.h"

void DSQ_2_Init(){
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0x20;		//设置定时初值
	T2H = 0xD1;		//设置定时初值
	AUXR |= 0x10;		//定时器2开始计时
	AUXR=AUXR&0xf7;
	IE2=IE2|0x04;
	EA=1;
}