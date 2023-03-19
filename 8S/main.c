#include "main.h"

uchar ShiZhong_SheZhi=0;//时钟设置。0：时钟显示；1：时；2：分；3：秒
uchar NaoZhong_SheZhi=0;//闹钟设置。0：时钟显示；1：时；2：分；3：秒
uchar key_num;
char ShiZhong_Shi,ShiZhong_Fen,ShiZhong_Miao;
char NaoZhong_Shi=-1;
char NaoZhong_Fen=-1;
char NaoZhong_Miao=-1;
char NaoZhong_Shi_temp,NaoZhong_Fen_temp,NaoZhong_Miao_temp;
uint DanYuan_time=0;
char wen_du;
uchar TiShi_flag=0;//提示。0：提示关闭；1：提示开启。
uint TiShi_time=0;

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
		if(ShiZhong_SheZhi>0||NaoZhong_SheZhi>0){
			while(P33==0);
		}
	}
	DLKey_Init();
	return Key_num;
}

void main(){
	Init_BZ();
	SMG_Init();
	DLKey_Init();
	DSQ_2_Init();
	Ds1302_XieData(23,59,50);
	DS18B20_GetWenDu();
	while(1){
		key_num=DLKey_GetKeynum();
		if(key_num==7){
			TiShi_flag=0;
			if(NaoZhong_SheZhi==0){
				ShiZhong_SheZhi++;
				ShiZhong_SheZhi=ShiZhong_SheZhi%4;
				if(ShiZhong_SheZhi==0){
					Ds1302_XieData(ShiZhong_Shi,ShiZhong_Fen,ShiZhong_Miao);
				}
			}
		}
		else if(key_num==6){
			TiShi_flag=0;
			if(ShiZhong_SheZhi==0){
				NaoZhong_SheZhi++;
				NaoZhong_SheZhi=NaoZhong_SheZhi%4;
				if(NaoZhong_SheZhi==0){
					NaoZhong_Shi=NaoZhong_Shi_temp;
					NaoZhong_Fen=NaoZhong_Fen_temp;
					NaoZhong_Miao=NaoZhong_Miao_temp;
				}
				else if(NaoZhong_SheZhi==1){
					NaoZhong_Shi_temp=ShiZhong_Shi;
					NaoZhong_Fen_temp=ShiZhong_Fen;
					NaoZhong_Miao_temp=ShiZhong_Miao;
					
				}
			}
		}
		else if(key_num==5){
			TiShi_flag=0;
			if(ShiZhong_SheZhi>0){
				switch(ShiZhong_SheZhi){
					case 1:{
						ShiZhong_Shi++;
						if(ShiZhong_Shi>=23){
							ShiZhong_Shi=23;
						}
						break;
					}
					case 2:{
						ShiZhong_Fen++;
						if(ShiZhong_Fen>=59){
							ShiZhong_Fen=59;
						}
						break;
					}
					case 3:{
						ShiZhong_Miao++;
						if(ShiZhong_Miao>=59){
							ShiZhong_Miao=59;
						}
						break;
					}
				}
			}
			else if(NaoZhong_SheZhi>0){
				switch(NaoZhong_SheZhi){
					case 1:{
						NaoZhong_Shi_temp++;
						if(NaoZhong_Shi_temp>=23){
							NaoZhong_Shi_temp=23;
						}
						break;
					}
					case 2:{
						NaoZhong_Fen_temp++;
						if(NaoZhong_Fen_temp>=59){
							NaoZhong_Fen_temp=59;
						}
						break;
					}
					case 3:{
						NaoZhong_Miao_temp++;
						if(NaoZhong_Miao_temp>=59){
							NaoZhong_Miao_temp=59;
						}
						break;
					}
				}
			}
		}
		else if(key_num==4){
			TiShi_flag=0;
			if(ShiZhong_SheZhi>0){
				switch(ShiZhong_SheZhi){
					case 1:{
						ShiZhong_Shi--;
						if(ShiZhong_Shi<=0){
							ShiZhong_Shi=0;
						}
						break;
					}
					case 2:{
						ShiZhong_Fen--;
						if(ShiZhong_Fen<=0){
							ShiZhong_Fen=0;
						}
						break;
					}
					case 3:{
						ShiZhong_Miao--;
						if(ShiZhong_Miao<=0){
							ShiZhong_Miao=0;
						}
						break;
					}
				}
			}
			else if(NaoZhong_SheZhi>0){
				switch(NaoZhong_SheZhi){
					case 1:{
						NaoZhong_Shi_temp--;
						if(NaoZhong_Shi_temp<=0){
							NaoZhong_Shi_temp=0;
						}
						break;
					}
					case 2:{
						NaoZhong_Fen_temp--;
						if(NaoZhong_Fen_temp<=0){
							NaoZhong_Fen_temp=0;
						}
						break;
					}
					case 3:{
						NaoZhong_Miao_temp--;
						if(NaoZhong_Miao_temp<=0){
							NaoZhong_Miao_temp=0;
						}
						break;
					}
					
				}
			}
			else{
				wen_du=(char)DS18B20_GetWenDu();
			}
		}
		
		if(ShiZhong_SheZhi==0&&NaoZhong_SheZhi==0){
			ShiZhong_Shi=Ds1302_GetShi();
			ShiZhong_Fen=Ds1302_GetFen();
			ShiZhong_Miao=Ds1302_GetMiao();
		}
		
		if(ShiZhong_Shi==NaoZhong_Shi&&ShiZhong_Fen==NaoZhong_Fen&&ShiZhong_Miao==NaoZhong_Miao){
			TiShi_flag=1;
			TiShi_time=0;
		}
	}
}

void DSQ_2_ZD() interrupt 12{
	if(key_num!=4&&ShiZhong_SheZhi==0&&NaoZhong_SheZhi==0){
		SMG_Show(ShiZhong_Shi/10,ShiZhong_Shi%10,17,ShiZhong_Fen/10,ShiZhong_Fen%10,17,ShiZhong_Miao/10,ShiZhong_Miao%10);
	}
	else if(ShiZhong_SheZhi>0||NaoZhong_SheZhi>0){
		DanYuan_time++;
		if(DanYuan_time>=2000){
			DanYuan_time=0;
		}
		if(DanYuan_time>=0&&DanYuan_time<=1000){
			if(ShiZhong_SheZhi>0){
				SMG_Show(ShiZhong_Shi/10,ShiZhong_Shi%10,17,ShiZhong_Fen/10,ShiZhong_Fen%10,17,ShiZhong_Miao/10,ShiZhong_Miao%10);
			}
			if(NaoZhong_SheZhi>0){
				SMG_Show(NaoZhong_Shi_temp/10,NaoZhong_Shi_temp%10,17,NaoZhong_Fen_temp/10,NaoZhong_Fen_temp%10,17,NaoZhong_Miao_temp/10,NaoZhong_Miao_temp%10);
			}
		}
		else if(DanYuan_time>1000){
			switch(ShiZhong_SheZhi){
				case 1:{
					SMG_Show(16,16,17,ShiZhong_Fen/10,ShiZhong_Fen%10,17,ShiZhong_Miao/10,ShiZhong_Miao%10);
					break;
				}
				case 2:{
					SMG_Show(ShiZhong_Shi/10,ShiZhong_Shi%10,17,16,16,17,ShiZhong_Miao/10,ShiZhong_Miao%10);
					break;
				}
				case 3:{
					SMG_Show(ShiZhong_Shi/10,ShiZhong_Shi%10,17,ShiZhong_Fen/10,ShiZhong_Fen%10,17,16,16);
					break;
				}
			}
			switch(NaoZhong_SheZhi){
				case 1:{
					SMG_Show(16,16,17,NaoZhong_Fen_temp/10,NaoZhong_Fen_temp%10,17,NaoZhong_Miao_temp/10,NaoZhong_Miao_temp%10);
					break;
				}
				case 2:{
					SMG_Show(NaoZhong_Shi_temp/10,NaoZhong_Shi_temp%10,17,16,16,17,NaoZhong_Miao_temp/10,NaoZhong_Miao_temp%10);
					break;
				}
				case 3:{
					SMG_Show(NaoZhong_Shi_temp/10,NaoZhong_Shi_temp%10,17,NaoZhong_Fen_temp/10,NaoZhong_Fen_temp%10,17,16,16);
					break;
				}
			}
		}
	}
	else if(key_num==4&&ShiZhong_SheZhi==0&&NaoZhong_SheZhi==0){
		if(wen_du>=10){
			SMG_Show(16,16,16,16,16,wen_du/10,wen_du%10,12);
		}
		else if(wen_du>=0){
			SMG_Show(16,16,16,16,16,16,wen_du,12);
		}
		else if(wen_du<0){
			SMG_Show(16,16,16,16,16,17,wen_du,12);
		}
	}
	
	if(TiShi_flag){
		TiShi_time++;
		if(TiShi_time>=5000){
			TiShi_time=0;
			TiShi_flag=0;
		}
	}
	LED_Show();
}

void LED_Show(){
	uchar L1=0xff;
	if((TiShi_time%400)<200){
		L1=0xff;
	}
	else{
		L1=0xfe;
	}
	P0=L1;
	P2=(P2&0x1f)|0x80;
	P2=(P2&0x1f)|0x00;
}