#include "JZKey.h"

void JZKey_Init(){
	P32=1;
	P33=1;
	P34=0;
	P35=0;
}

unsigned char JZKey_GetKeynum(){
	unsigned char key_num=0;
	if(P32==0){
		P32=0;
		P34=1;
		P35=1;
		if(P34==0){
			Delay10ms();
			if(P34==0){
				key_num=17;
			}
			//while(P34==0);
		}
		else if(P35==0){
			Delay10ms();
			if(P35==0){
				key_num=13;
			}
			//while(P35==0);
		}
	}
	else if(P33==0){
		P33=0;
		P34=1;
		P35=1;
		if(P34==0){
			Delay10ms();
			if(P34==0){
				key_num=16;
			}
			//while(P34==0);
		}
		else if(P35==0){
			Delay10ms();
			if(P35==0){
				key_num=12;
			}
			//while(P35==0);
		}
	}
	JZKey_Init();
	return key_num;
}