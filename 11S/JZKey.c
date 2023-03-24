#include "JZKey.h"

void JZKey_Init(){
	P30=1;
	P31=1;
	P32=1;
	P33=1;
	P34=0;
	P35=0;
	P42=0;
	P44=0;
}

unsigned char JZKey_GetKeynum(){
	unsigned char key_num=0;
	if(P30==0){
		P30=0;
		P34=1;
		P35=1;
		P42=1;
		P44=1;
		if(P34==0){
			Delay10ms();
			if(P34==0){
				key_num=19;
			}
			while(P34==0);
		}
		else if(P35==0){
			Delay10ms();
			if(P35==0){
				key_num=15;
			}
			while(P35==0);
		}
		else if(P42==0){
			Delay10ms();
			if(P42==0){
				key_num=11;
			}
			while(P42==0);
		}
		else if(P44==0){
			Delay10ms();
			if(P44==0){
				key_num=7;
			}
			while(P44==0);
		}
	}
	else if(P31==0){
		P31=0;
		P34=1;
		P35=1;
		P42=1;
		P44=1;
		if(P34==0){
			Delay10ms();
			if(P34==0){
				key_num=18;
			}
			while(P34==0);
		}
		else if(P35==0){
			Delay10ms();
			if(P35==0){
				key_num=14;
			}
			while(P35==0);
		}
		else if(P42==0){
			Delay10ms();
			if(P42==0){
				key_num=10;
			}
			while(P42==0);
		}
		else if(P44==0){
			Delay10ms();
			if(P44==0){
				key_num=6;
			}
			while(P44==0);
		}
	}
	else if(P32==0){
		P32=0;
		P34=1;
		P35=1;
		P42=1;
		P44=1;
		if(P34==0){
			Delay10ms();
			if(P34==0){
				key_num=17;
			}
			while(P34==0);
		}
		else if(P35==0){
			Delay10ms();
			if(P35==0){
				key_num=13;
			}
			while(P35==0);
		}
		else if(P42==0){
			Delay10ms();
			if(P42==0){
				key_num=9;
			}
			while(P42==0);
		}
		else if(P44==0){
			Delay10ms();
			if(P44==0){
				key_num=5;
			}
			while(P44==0);
		}
	}
	else if(P33==0){
		P33=0;
		P34=1;
		P35=1;
		P42=1;
		P44=1;
		if(P34==0){
			Delay10ms();
			if(P34==0){
				key_num=16;
			}
			while(P34==0);
		}
		else if(P35==0){
			Delay10ms();
			if(P35==0){
				key_num=12;
			}
			while(P35==0);
		}
		else if(P42==0){
			Delay10ms();
			if(P42==0){
				key_num=8;
			}
			while(P42==0);
		}
		else if(P44==0){
			Delay10ms();
			if(P44==0){
				key_num=4;
			}
			while(P44==0);
		}
	}
	JZKey_Init();
	return key_num;
}