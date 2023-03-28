#include "main.h"

uchar key_num;
uchar key_num_old;
uchar JieMian=0;
uchar MoShi=0;
int WenDu=0;
uchar shi;
uchar fen;
uchar miao;
uchar WenDu_CanShu=23;//温度参数
uchar L1_flag=0;//L1亮灭标志位
uint L1_time=0;
uchar JDQ=0;//继电器标志位
uchar L3_time_flag=0;
uchar L3_time=0;
uchar time=0;

void main(){
	Init_BZ();
	SMG_Init();
	DSQ_2_Init();
	JZKey_Init();
	DS18B20_GetWenDu();
	Ds1302_XieData(23,59,50);
	while(1){
		key_num_old=key_num;
		key_num=JZKey_GetKeynum();
		if(key_num_old!=key_num){
			if(key_num==12){
				JieMian++;
				JieMian=JieMian%3;
			}
			else if(key_num==13){
				MoShi++;
				MoShi=MoShi%2;
				if(MoShi){
					L3_time_flag=0;
				}
			}
			else if(key_num==16&&JieMian==2){
				WenDu_CanShu++;
				if(WenDu_CanShu>=99){
					WenDu_CanShu=99;
				}
			}
			else if(key_num==17&&JieMian==2){
				WenDu_CanShu--;
				if(WenDu_CanShu<=10){
					WenDu_CanShu=10;
				}
			}
		}
		
		if(time>=10){
			time=0;
			WenDu=DS18B20_GetWenDu()*10;
			
			shi=Ds1302_GetShi();
			fen=Ds1302_GetFen();
			miao=Ds1302_GetMiao();
			if(fen==0&&miao==0){
				L1_flag=1;
				L1_time=0;
				if(MoShi){
					P0=0x10;
					P2=(P2&0x1f)|0xa0;
					P2=(P2&0x1f)|0x00;
					L3_time_flag=1;
				}
			}
			
			if(MoShi==0){
				if(WenDu>WenDu_CanShu*10){
					P0=0x10;
					L3_time_flag=1;
				}
				else{
					P0=0x00;
					L3_time_flag=0;
				}
				P2=(P2&0x1f)|0xa0;
				P2=(P2&0x1f)|0x00;
			}
		}
	}
}

void DSQ_2_ZD() interrupt 12{
	time++;
	
	if(L1_flag){
		L1_time++;
		if(L1_time>=5000){
			L1_time=0;
			L1_flag=0;
			if(MoShi){
				P0=0x00;
				P2=(P2&0x1f)|0xa0;
				P2=(P2&0x1f)|0x00;
				L3_time_flag=0;
				L3_time=0;
			}
		}
	}
	
	if(L3_time_flag){
		L3_time++;
		if(L3_time>=200){
			L3_time=0;
		}
	}
	
	if(JieMian==0){
		if(WenDu>=100){
			SMG_Show(25,1,16,16,16,WenDu/100,((WenDu/10)%10)+32,WenDu%10);
		}
		else if(WenDu>=0){
			SMG_Show(25,1,16,16,16,16,(WenDu/10)+32,WenDu%10);
		}
		else if(WenDu<0){
			SMG_Show(25,1,16,16,16,17,(((-WenDu)/10)%10)+32,(-WenDu)%10);
		}
	}
	else if(JieMian==1){
		if(key_num==17){
			SMG_Show(25,2,16,fen/10,fen%10,17,miao/10,miao%10);
		}
		else{
			SMG_Show(25,2,16,shi/10,shi%10,17,fen/10,fen%10);
		}
	}
	else if(JieMian==2){
		SMG_Show(25,3,16,16,16,16,WenDu_CanShu/10,WenDu_CanShu%10);
	}
	
	LED_Show();
}

void LED_Show(){
	uchar L1=0xff;
	uchar L2=0xff;
	uchar L3=0xff;
	if(L1_flag){
		L1=0xfe;
	}
	if(MoShi==0){
		L2=0xfd;
	}
	if(L3_time_flag&&L3_time<=100){
		L3=0xfb;
	}
	P0=L1&L2&L3;
	P2=(P2&0x1f)|0x80;
	P2=(P2&0x1f)|0x00;
}