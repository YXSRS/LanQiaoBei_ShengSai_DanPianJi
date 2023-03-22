#include "main.h"

uint Hz;
uint Hz_time=0;
uchar key_num;
uchar XianShi_JieMian=0;//显示界面。0：电压测量；1：频率测量
uchar ShuChu_MoShi=0;//输出模式。0：2V：1：RB2
uchar LED_GongNeng=1;//LED功能。1：LED功能启用；0：LED功能禁用
uchar SMG_XianShi_GongNeng=1;//数码管显示功能。1：数码管显示功能启用。0：数码管显示功能禁用。
uchar RB2_data;
uint RB2_DianYa;
uchar RB2_time;
uchar DAC_data;

void main(){
	Init_BZ();
	SMG_Init();
	DSQ_2_Init();
	DLKey_Init();
	LN555_DSQ_0_Init();
	while(1){
		key_num=DLKey_GetKeynum();
		if(key_num==4){
			XianShi_JieMian++;
			XianShi_JieMian=XianShi_JieMian%2;
		}
		else if(key_num==5){
			ShuChu_MoShi++;
			ShuChu_MoShi=ShuChu_MoShi%2;
		}
		else if(key_num==6){
			LED_GongNeng++;
			LED_GongNeng=LED_GongNeng%2;
		}
		else if(key_num==7){
			SMG_XianShi_GongNeng++;
			SMG_XianShi_GongNeng=SMG_XianShi_GongNeng%2;
		}
		
		if(RB2_time>=20){
			RB2_time=0;
			RB2_data=IIC_ADC_GetData(3);
			RB2_DianYa=(uint)(RB2_data*1.0*(500.0/255.0));
		}
		
		if(ShuChu_MoShi){
			IIC_DAC_XieData(RB2_data);
		}
		else{
			IIC_DAC_XieData(101);
		}
	}
}

void DSQ_2_ZD() interrupt 12{
	RB2_time++;
	
	Hz_time++;
	if(Hz_time>=1000){
		Hz_time=0;
		Hz=((uint)TH0<<8)|(uint)TL0;
		TH0=0;
		TL0=0;
	}
	
	if(SMG_XianShi_GongNeng){
		if(XianShi_JieMian){
			if(Hz>=100000){
				SMG_Show(15,16,Hz/100000,(Hz/10000)%10,(Hz/1000)%10,(Hz/100)%10,(Hz/10)%10,Hz%10);
			}
			else if(Hz>=10000){
				SMG_Show(15,16,16,Hz/10000,(Hz/1000)%10,(Hz/100)%10,(Hz/10)%10,Hz%10);
			}
			else if(Hz>=1000){
				SMG_Show(15,16,16,16,Hz/1000,(Hz/100)%10,(Hz/10)%10,Hz%10);
			}
			else if(Hz>=100){
				SMG_Show(15,16,16,16,16,Hz/100,(Hz/10)%10,Hz%10);
			}
			else if(Hz>=10){
				SMG_Show(15,16,16,16,16,16,Hz/10,Hz%10);
			}
			else if(Hz>=0){
				SMG_Show(15,16,16,16,16,16,16,Hz);
			}
		}
		else{
			SMG_Show(25,16,16,16,16,(RB2_DianYa/100)+32,(RB2_DianYa/10)%10,RB2_DianYa%10);
		}
	}
	else{
		SMG_Show(16,16,16,16,16,16,16,16);
	}
	
	LED_Show();
}

void LED_Show(){
	uchar L1_2=0xff;
	uchar L3=0xff;
	uchar L4=0xff;
	uchar L5=0xff;
	if(LED_GongNeng){
		if(XianShi_JieMian){
			L1_2=0xfd;
		}
		else{
			L1_2=0xfe;
		}
		if((RB2_DianYa>=150&&RB2_DianYa<=250)||(RB2_DianYa>=350)){
			L3=0xfb;
		}
		if((Hz>=1000&&Hz<5000)||(Hz>=10000)){
			L4=0xf7;
		}
		if(ShuChu_MoShi){
			L5=0xef;
		}
	}
	P0=L1_2&L3&L4;
	P2=(P2&0x1f)|0x80;
	P2=(P2&0x1f)|0x00;
}