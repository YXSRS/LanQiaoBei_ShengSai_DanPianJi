#include "DLKey.h"

void DLKey_Init(){
	P30=1;
	P31=1;
	P32=1;
	P33=1;
}

unsigned char DLKey_GetKeynum(){
	unsigned char key_num=0;
	if(P30==0){
		Delay10ms();
		if(P30==0){
			key_num=7;
		}
		while(P30==0);
	}
	else if(P31==0){
		Delay10ms();
		if(P31==0){
			key_num=6;
		}
		while(P31==0);
	}
	else if(P32==0){
		Delay10ms();
		if(P32==0){
			key_num=5;
		}
		while(P32==0);
	}
	else if(P33==0){
		Delay10ms();
		if(P33==0){
			key_num=4;
		}
		while(P33==0);
	}
	DLKey_Init();
	return key_num;
}