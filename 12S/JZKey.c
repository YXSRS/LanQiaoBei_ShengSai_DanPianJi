#include "JZKey.h"

void JZKey_Init(){
	P32=1;
	P33=1;
	P42=0;
	P44=0;
}

unsigned char JZKey_GetKeynum(){
	unsigned char key_num=0;
	if(P32==0){
		P32=0;
		P42=1;
		P44=1;
		if(P42==0){
			Delay10ms();
			if(P42==0){
				key_num=9;
			}
			//while(P42==0);
		}
		else if(P44==0){
			Delay10ms();
			if(P44==0){
				key_num=5;
			}
			//while(P44==0);
		}
	}
	else if(P33==0){
		P33=0;
		P42=1;
		P44=1;
		if(P42==0){
			Delay10ms();
			if(P42==0){
				key_num=8;
			}
			//while(P42==0);
		}
		else if(P44==0){
			Delay10ms();
			if(P44==0){
				key_num=4;
			}
			//while(P44==0);
		}
	}
	JZKey_Init();
	return key_num;
}