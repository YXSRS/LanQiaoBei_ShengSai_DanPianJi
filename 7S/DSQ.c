#include "DSQ.h"

void DSQ_2_Init(){
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0x20;		//���ö�ʱ��ֵ
	T2H = 0xD1;		//���ö�ʱ��ֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	AUXR=AUXR&0xf7;
	IE2=IE2|0x04;
	EA=1;
}