#include "main.h"

uchar key_num=0;
uchar key_num_old=0;
uchar JieMian=0;//界面。0：温度显示界面；1：参数设置界面；2：DAC输出界面
uchar MoShi=0;//模式。0：模式1；1：模式2
char CanShu=25;
uint WenDu;
uchar DAC_Data;
uint DAC_DianYa;
uchar time=0;

void main(){
	Init_BZ();
	SMG_Init();
	DSQ_2_Init();
	JZKey_Init();
	DS18B20_GetWenDu();
	while(1){
		key_num_old=key_num;
		key_num=JZKey_GetKeynum();
		if(key_num_old!=key_num){
			if(key_num==4){
				JieMian++;
				JieMian=JieMian%3;
			}
			else if(key_num==5){
				MoShi++;
				MoShi=MoShi%2;
			}
			else if(key_num==8&&JieMian==1){
				CanShu--;
				if(CanShu<-9){
					CanShu=-9;
				}
			}
			else if(key_num==9&&JieMian==1){
				CanShu++;
				if(CanShu>99){
					CanShu=99;
				}
			}
		}
		
		
		if(time>=20){
			time=0;
			WenDu=DS18B20_GetWenDu()*100;
		}
		
		if(MoShi){
			if(WenDu<2000){
				IIC_DAC_XieData(51);
				DAC_DianYa=100;
			}
			else if(WenDu>=2000&&WenDu<=4000){
				DAC_DianYa=0.15*WenDu-200;
				IIC_DAC_XieData(DAC_DianYa/100.0*51);
			}
			else if(WenDu>4000){
				IIC_DAC_XieData(204);
				DAC_DianYa=400;
			}
		}
		else{
			if(WenDu<CanShu*100){
				IIC_DAC_XieData(0);
				DAC_DianYa=0;
			}
			else{
				IIC_DAC_XieData(255);
				DAC_DianYa=500;
			}
		}
	}
}

void DSQ_2_ZD() interrupt 12{
	time++;
	
	switch(JieMian){
		case 0:{
			if(WenDu>=1000){
				SMG_Show(12,16,16,16,WenDu/1000,((WenDu/100)%10)+32,(WenDu/10)%10,WenDu%10);
			}
			else if(WenDu>=0){
				SMG_Show(12,16,16,16,16,(WenDu/100)+32,(WenDu/10)%10,WenDu%10);
			}
			else if(WenDu<0){
				SMG_Show(12,16,16,16,17,((WenDu/100)%10)+32,(WenDu/10)%10,WenDu%10);
			}
			break;
		}
		case 1:{
			if(CanShu>=10){
				SMG_Show(24,16,16,16,16,16,CanShu/10,CanShu%10);
			}
			else if(CanShu>=0){
				SMG_Show(24,16,16,16,16,16,16,CanShu);
			}
			else if(CanShu<0){
				SMG_Show(24,16,16,16,16,16,17,-CanShu);
			}
			break;
		}
		case 2:{
			SMG_Show(10,16,16,16,16,(DAC_DianYa/100)+32,(DAC_DianYa/10)%10,DAC_DianYa%10);
			break;
		}
	}
	
	LED_Show();
}

void LED_Show(){
	uchar L1=0xff;
	uchar L2_3_4=0xff;
	if(MoShi==0){
		L1=0xfe;
	}
	switch(JieMian){
		case 0:{
			L2_3_4=0xfd;
			break;
		}
		case 1:{
			L2_3_4=0xfb;
			break;
		}
		case 2:{
			L2_3_4=0xf7;
			break;
		}
	}
	P0=L1&L2_3_4;
	P2=(P2&0x1f)|0x80;
	P2=(P2&0x1f)|0x00;
}