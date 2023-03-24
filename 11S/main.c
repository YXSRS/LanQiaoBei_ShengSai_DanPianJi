#include "main.h"

uchar key_num;
uchar XianShi_JieMian=0;//显示界面。0：数据界面；1：参数界面；2：计数界面
long int JiShuZhi=0;//计数值
char DianYa_CanShu=0;//电压参数
uint AIN3_DianYa=0;//AIN3的电压
uint AIN3_DianYa_old=0;
uchar AIN3_time=0;
uint L1_time=0;
uchar WuXiao_AnJian_num=0;//无效按键次数

void main(){
	Init_BZ();
	SMG_Init();
	DSQ_2_Init();
	JZKey_Init();
	DianYa_CanShu=IIC_EEPROM_GetData(0);
	while(1){
		key_num=JZKey_GetKeynum();
		if(key_num==12){
			WuXiao_AnJian_num=0;
			XianShi_JieMian++;
			XianShi_JieMian=XianShi_JieMian%3;
			if(XianShi_JieMian==2){
				IIC_EEPROM_XieData(0,DianYa_CanShu);
			}
		}
		else if(key_num==13){
			WuXiao_AnJian_num=0;
			if(XianShi_JieMian==2){
				JiShuZhi=0;
			}
		}
		else if(key_num==16&&XianShi_JieMian==1){
			WuXiao_AnJian_num=0;
			DianYa_CanShu=DianYa_CanShu+5;
			if(DianYa_CanShu>50){
				DianYa_CanShu=0;
			}
		}
		else if(key_num==17&&XianShi_JieMian==1){
			WuXiao_AnJian_num=0;
			DianYa_CanShu=DianYa_CanShu-5;
			if(DianYa_CanShu<0){
				DianYa_CanShu=50;
			}
		}
		else if(key_num>0){
			WuXiao_AnJian_num++;
			if(WuXiao_AnJian_num>=3){
				WuXiao_AnJian_num=3;
			}
		}
		
		if(AIN3_time>=30){
			AIN3_time=0;
			AIN3_DianYa_old=AIN3_DianYa;
			AIN3_DianYa=IIC_ADC_GetData(3)*1.0*(500.0/255.0);
			if(AIN3_DianYa_old>AIN3_DianYa&&AIN3_DianYa_old>(DianYa_CanShu*10)&&AIN3_DianYa<(DianYa_CanShu*10)){
				JiShuZhi++;
			}
		}
	}
}

void DSQ_2_ZD() interrupt 12{
	AIN3_time++;
	
	if(AIN3_DianYa<DianYa_CanShu*10){
		L1_time++;
		if(L1_time>5000){
			L1_time=5001;
		}
	}
	else{
		L1_time=0;
	}
	
	switch(XianShi_JieMian){
		case 0:{
			SMG_Show(25,16,16,16,16,(AIN3_DianYa/100)+32,(AIN3_DianYa/10)%10,AIN3_DianYa%10);
			break;
		}
		case 1:{
			SMG_Show(24,16,16,16,16,(DianYa_CanShu/10)+32,DianYa_CanShu%10,0);
			break;
		}
		case 2:{
			if(JiShuZhi>=1000000){
				SMG_Show(22,JiShuZhi/1000000,(JiShuZhi/100000)%10,(JiShuZhi/10000)%10,(JiShuZhi/1000)%10,(JiShuZhi/100)%10,(JiShuZhi/10)%10,JiShuZhi%10);
			}
			else if(JiShuZhi>=100000){
				SMG_Show(22,16,JiShuZhi/100000,(JiShuZhi/10000)%10,(JiShuZhi/1000)%10,(JiShuZhi/100)%10,(JiShuZhi/10)%10,JiShuZhi%10);
			}
			else if(JiShuZhi>=10000){
				SMG_Show(22,16,16,JiShuZhi/10000,(JiShuZhi/1000)%10,(JiShuZhi/100)%10,(JiShuZhi/10)%10,JiShuZhi%10);
			}
			else if(JiShuZhi>=1000){
				SMG_Show(22,16,16,16,JiShuZhi/1000,(JiShuZhi/100)%10,(JiShuZhi/10)%10,JiShuZhi%10);
			}
			else if(JiShuZhi>=100){
				SMG_Show(22,16,16,16,16,JiShuZhi/100,(JiShuZhi/10)%10,JiShuZhi%10);
			}
			else if(JiShuZhi>=10){
				SMG_Show(22,16,16,16,16,16,JiShuZhi/10,JiShuZhi%10);
			}
			else if(JiShuZhi>=0){
				SMG_Show(22,16,16,16,16,16,16,JiShuZhi);
			}
			break;
		}
	}
	
	LED_Show();
}

void LED_Show(){
	uchar L1=0xff;
	uchar L2=0xff;
	uchar L3=0xff;
	if(L1_time>5000){
		L1=0xfe;
	}
	if(JiShuZhi%2){
		L2=0xfd;
	}
	if(WuXiao_AnJian_num>=3){
		L3=0xfb;
	}
	P0=L1&L2&L3;
	P2=(P2&0x1f)|0x80;
	P2=(P2&0x1f)|0x00;
}