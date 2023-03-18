#include "main.h"

uchar key_num=0;
uchar pwm=0;
uchar pwm_time=0;
uchar GongZuo_MoShi=0;//����ģʽ��0��˯�߷磻1����Ȼ�磻2������
int ShengYu_ShiJian=0;//ʣ��ʱ�䣬��λ��s
uchar JieMian=0;//���棬0������ģʽ��ʣ�๤��ʱ����ʾ��1��������ʾ����
uint time=0;
char wen_du=0;//�¶�
uchar WenDu_Time=0;

void main(){
	Init_BZ();
	SMG_Init();
	DLKey_Init();
	DSQ_2_Init();
	PWM_DSQ_1_Init();
	while(1){
		key_num=DLKey_GetKeynum();
		if(key_num==4){
			GongZuo_MoShi++;
			GongZuo_MoShi=GongZuo_MoShi%3;
			switch(GongZuo_MoShi){
				case 0:{
					pwm=20;
					break;
				}
				case 1:{
					pwm=30;
					break;
				}
				case 2:{
					pwm=70;
					break;
				}
			}
		}
		else if(key_num==5){
			ShengYu_ShiJian=ShengYu_ShiJian+60;
			if(ShengYu_ShiJian>9999){
				ShengYu_ShiJian=9999;
			}
			ET1=1;
		}
		else if(key_num==6){
			ShengYu_ShiJian=0;
			ET1=0;
		}
		else if(key_num==7){
			JieMian++;
			JieMian=JieMian%2;
		}
		
		if(WenDu_Time>=20){
			WenDu_Time=0;
			wen_du=(char)DS18B20_GetWenDu();
		}
	}
}

void DSQ_1_ZD() interrupt 3{
	pwm_time++;
	if(pwm_time<pwm){
		P34=1;
	}
	else if(pwm_time>=pwm&&pwm_time<=100){
		P34=0;
	}
	else if(pwm_time>=101){
		P34=0;
		pwm_time=0;
	}
}

void DSQ_2_ZD() interrupt 12{
	WenDu_Time++;
	if(ShengYu_ShiJian>0){
		time++;
		if(time>=1000){
			time=0;
			ShengYu_ShiJian--;
			if(ShengYu_ShiJian<=0){
				ShengYu_ShiJian=0;
				ET1=0;
			}
		}
	}
	
	SMG_XianShi();
	LED_XianShi();
}

void SMG_XianShi(){
	if(JieMian){
		if(wen_du>=10){
			SMG_Show(17,4,17,16,16,wen_du/10,wen_du%10,12);
		}
		else if(wen_du>=0){
			SMG_Show(17,4,17,16,16,16,wen_du,12);
		}
		else if(wen_du<0){
			SMG_Show(17,4,17,16,16,17,-wen_du,12);
		}
	}
	else{
		SMG_Show(17,GongZuo_MoShi+1,17,16,ShengYu_ShiJian/1000,(ShengYu_ShiJian/100)%10,(ShengYu_ShiJian/10)%10,ShengYu_ShiJian%10);
	}
}

void LED_XianShi(){
	uchar L1=0xff;
	uchar L2=0xff;
	uchar L3=0xff;
	if(ShengYu_ShiJian>0){
		switch(GongZuo_MoShi){
			case 0:{
				L1=0xfe;
				break;
			}
			case 1:{
				L2=0xfd;
				break;
			}
			case 2:{
				L3=0xfb;
				break;
			}
		}
		
	}
	P0=L1&L2&L3;
	P2=(P2&0x1f)|0x80;
	P2=(P2&0x1f)|0x00;
}