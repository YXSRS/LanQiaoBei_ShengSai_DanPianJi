#include "main.h"

uchar pwm;
uchar pwm_time=0;
uchar QiDong_TingZhi=0;//启动停止。0：停止；1：启动。
uchar key_num;
uchar SheZhi=0;//设置。0：数码管全熄灭；1：模式编号；2流转间隔
uchar MoShi_BianHao=1;//模式编号。1-4
uchar MoShi_BianHao_temp=1;//模式编号临时值。1-4
uchar LiuZhuan_JianGe[4];//流转间隔
uchar LiuZhuan_JianGe_temp;//流转间隔临时值
uchar LiangDu_DengJi_flag=0;//亮度等级标志位。0：不显示亮度等级；1：显示亮度等级
uint XianShi_DanYuan_time=0;//显示单元的时间。
uchar LiangDu_DengJi;//亮度等级
uchar LiangDu;//亮度
uchar EEPROM_Data_flag;//EEPROM数据标志位
uchar code MoShi1_2[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uchar code MoShi3_4[]={0xe7,0xdb,0xbd,0x7e};
uint time=0;
char num=0;
uchar ADC_time=0;

unsigned char DLKey_GetKeynum(){
	unsigned char Key_num=0;
	if(P30==0){
		Delay10ms();
		if(P30==0){
			Key_num=7;
		}
		while(P30==0);
	}
	else if(P31==0){
		Delay10ms();
		if(P31==0){
			Key_num=6;
		}
		while(P31==0);
	}
	else if(P32==0){
		Delay10ms();
		if(P32==0){
			Key_num=5;
		}
		while(P32==0);
	}
	else if(P33==0){
		Delay10ms();
		if(P33==0){
			Key_num=4;
		}
		if(SheZhi>0){
			while(P33==0);
		}
	}
	DLKey_Init();
	return Key_num;
}

void main(){
	uchar i;
	Init_BZ();
	SMG_Init();
	DSQ_2_Init();
	PWM_DSQ_1_Init();
	EEPROM_Data_flag=IIC_EEPROM_GetData(10);
	Delay10ms();
	if(EEPROM_Data_flag!=10){
		IIC_EEPROM_XieData(10,10);
		Delay10ms();
		for(i=11;i<=14;i++){
			IIC_EEPROM_XieData(i,4);
			Delay10ms();
		}
	}
	Delay10ms();
	for(i=0;i<=3;i++){
		LiuZhuan_JianGe[i]=IIC_EEPROM_GetData(i+11);
		Delay10ms();
	}
	while(1){
		key_num=DLKey_GetKeynum();
		if(key_num==7){
			QiDong_TingZhi++;
			QiDong_TingZhi=QiDong_TingZhi%2;
			if(QiDong_TingZhi){
				ET1=1;
			}
			else{
				ET1=0;
			}
		}
		else if(key_num==6){
			SheZhi++;
			SheZhi=SheZhi%3;
			if(SheZhi==0){
				LiuZhuan_JianGe[MoShi_BianHao_temp-1]=LiuZhuan_JianGe_temp;
				MoShi_BianHao=MoShi_BianHao_temp;
				IIC_EEPROM_XieData(MoShi_BianHao+10,LiuZhuan_JianGe_temp);
				num=0;
			}
			else if(SheZhi==1){
				MoShi_BianHao_temp=MoShi_BianHao;
				LiuZhuan_JianGe_temp=LiuZhuan_JianGe[MoShi_BianHao-1];
			}
		}
		else if(key_num==5){
			if(SheZhi==1){
				MoShi_BianHao_temp++;
				if(MoShi_BianHao_temp>=4){
					MoShi_BianHao_temp=4;
				}
				LiuZhuan_JianGe_temp=LiuZhuan_JianGe[MoShi_BianHao_temp-1];
			}
			else if(SheZhi==2){
				LiuZhuan_JianGe_temp++;
				if(LiuZhuan_JianGe_temp>=12){
					LiuZhuan_JianGe_temp=12;
				}
			}
		}
		else if(key_num==4){
			if(SheZhi==0){
				LiangDu_DengJi_flag=1;
			}
			else if(SheZhi==1){
				MoShi_BianHao_temp--;
				if(MoShi_BianHao_temp<=1){
					MoShi_BianHao_temp=1;
				}
				LiuZhuan_JianGe_temp=LiuZhuan_JianGe[MoShi_BianHao_temp-1];
			}
			else if(SheZhi==2){
				LiuZhuan_JianGe_temp--;
				if(LiuZhuan_JianGe_temp<=4){
					LiuZhuan_JianGe_temp=4;
				}
			}
		}
		else{
			LiangDu_DengJi_flag=0;
		}
		
		if(ADC_time>=3){
			ADC_time=0;
			LiangDu=IIC_ADC_GetData(3);
			if(0<=LiangDu&&LiangDu<63){
				LiangDu_DengJi=1;
				pwm=10;
			}
			else if(63<=LiangDu&&LiangDu<126){
				LiangDu_DengJi=2;
				pwm=40;
			}
			else if(126<=LiangDu&&LiangDu<=189){
				LiangDu_DengJi=3;
				pwm=70;
			}
			else{
				LiangDu_DengJi=4;
				pwm=100;
			}
		}
	}
}

void DSQ_2_ZD() interrupt 12{
	ADC_time++;
	
	if(QiDong_TingZhi){
		time++;
		if(time>=LiuZhuan_JianGe[MoShi_BianHao-1]*100){
			time=0;
			if(MoShi_BianHao==1){
				num++;
				num=num%8;
			}
			else if(MoShi_BianHao==2){
				num--;
				if(num<0){
					num=7;
				}
			}
			else if(MoShi_BianHao==3){
				num++;
				num=num%4;
			}
			else if(MoShi_BianHao==4){
				num--;
				if(num<0){
					num=3;
				}
			}
		}
	}
	
	XianShi_DanYuan_time++;
	if(XianShi_DanYuan_time>=1600){
		XianShi_DanYuan_time=0;
	}
	switch(SheZhi){
		case 0:{
			if(LiangDu_DengJi_flag){
				SMG_Show(16,16,16,16,16,16,17,LiangDu_DengJi);
			}
			else{
				SMG_Show(16,16,16,16,16,16,16,16);
			}
			break;
		}
		case 1:{
			if(XianShi_DanYuan_time<800){
				if(LiuZhuan_JianGe_temp>=10){
					SMG_Show(17,MoShi_BianHao_temp,17,16,LiuZhuan_JianGe_temp/10,LiuZhuan_JianGe_temp%10,0,0);
				}
				else if(LiuZhuan_JianGe_temp>=0){
					SMG_Show(17,MoShi_BianHao_temp,17,16,16,LiuZhuan_JianGe_temp,0,0);
				}
			}
			else{
				if(LiuZhuan_JianGe_temp>=10){
					SMG_Show(16,16,16,16,LiuZhuan_JianGe_temp/10,LiuZhuan_JianGe_temp%10,0,0);
				}
				else if(LiuZhuan_JianGe_temp>=0){
					SMG_Show(16,16,16,16,16,LiuZhuan_JianGe_temp,0,0);
				}
			}
			break;
		}
		case 2:{
			if(XianShi_DanYuan_time<800){
				if(LiuZhuan_JianGe_temp>=10){
					SMG_Show(17,MoShi_BianHao_temp,17,16,LiuZhuan_JianGe_temp/10,LiuZhuan_JianGe_temp%10,0,0);
				}
				else if(LiuZhuan_JianGe_temp>=0){
					SMG_Show(17,MoShi_BianHao_temp,17,16,16,LiuZhuan_JianGe_temp,0,0);
				}
			}
			else{
				SMG_Show(17,MoShi_BianHao_temp,17,16,16,16,16,16);
			}
			break;
		}
	}
}

void DSQ_1_ZD() interrupt 3{
	if(QiDong_TingZhi){
		pwm_time++;
		if(pwm_time<pwm){
			if(MoShi_BianHao==1||MoShi_BianHao==2){
				P0=0xff&MoShi1_2[num];
			}
			else if(MoShi_BianHao==3||MoShi_BianHao==4){
				P0=0xff&MoShi3_4[num];
			}
		}
		else if(pwm_time>=pwm&&pwm_time<=100){
			P0=0xff;
		}
		else if(pwm_time>=101){
			pwm_time=0;
			P0=0xff;
		}
		P2=(P2&0x1f)|0x80;
		P2=(P2&0x1f)|0x00;
	}
}