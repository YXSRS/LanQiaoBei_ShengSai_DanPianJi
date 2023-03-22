#include "LN555.h"

void LN555_DSQ_0_Init(){
	TMOD=(TMOD&0xf0)|0x05;
	TH0=0;
	TL0=0;
	ET0=0;
	TR0=1;
}