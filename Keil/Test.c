#include <reg52.h>
sbit p10=P1^0;
sbit p11=P1^1;
sbit p12=P1^2;
sbit p13=P1^3;
sbit p14=P1^4;
sbit p15=P1^5;
sbit p16=P1^6;
sbit p17=P1^7;
sbit p20=P2^0;
sbit p21=P2^1;
sbit p22=P2^2;
sbit p23=P2^3;
sbit p24=P2^4;
sbit p25=P2^5;
sbit p26=P2^6;
sbit p27=P2^7;
sbit p34=P3^4;
sbit p35=P3^5;
sbit p36=P3^6;
sbit p37=P3^7;
int Elevator1=0,Elevator2=0;
int up1=0,up2=0,down1=0,down2=0;	
unsigned char e1=0,e2=0,up=0,down=0;
void delay(int x){
	while(x>0){x--;}
}
void display_elevator(){
	P0=Elevator1+Elevator2*16+17;
}
void display_LED(){
	int i=0;
	while(i<8){
		P3=i^7;
		p34=(e1>>i)&1>0;
		p35=(e2>>i)&1>0;
		p36=(up>>i)&1>0;
		p37=(down>>i)&1>0;
		delay(50);
		i++;
	}
}
void move(int *pup1,int *pup2,int *pdown1,int *pdown2,unsigned char *pe,int *pElevator1,int *pElevator2){
	int f;
	bit flag;
	if(*pup1){
		f=7;flag=1;
		while(f>*pElevator1){
			if(*pe&(1<<f)){flag=0;(*pElevator1)++;break;}
			if(up&(1<<f)){if(*pdown2||*pElevator2<=*pElevator1||*pElevator2>f){flag=0;(*pElevator1)++;break;}}
			if(down&(1<<f)){if(*pdown2||*pElevator2<=*pElevator1||*pElevator2>f){flag=0;(*pElevator1)++;break;}}
			f--;
		}
		if(flag)*pup1=0;
		else{
			if(*pElevator1==f){
				up=up&((1<<f)^255);
				down=down&((1<<f)^255);
				*pe=*pe&((1<<f)^255);
			}
			else{
				up=up&((1<<*pElevator1)^255);
				*pe=*pe&((1<<*pElevator1)^255);
			}
		}
	}
	else if(*pdown1){
		f=0;flag=1;
		while(f<*pElevator1){
			if(*pe&(1<<f)){flag=0;(*pElevator1)--;break;}
			if(down&(1<<f)){if(*pup2||*pElevator2>=*pElevator1||*pElevator2<f){flag=0;(*pElevator1)--;break;}}
			if(up&(1<<f)){if(*pup2||*pElevator2>=*pElevator1||*pElevator2<f){flag=0;(*pElevator1)--;break;}}
			f++;
		}
		if(flag)*pdown1=0;
		else{
			if(*pElevator1==f){
				up=up&((1<<f)^255);
				down=down&((1<<f)^255);
				*pe=*pe&((1<<f)^255);
			}
			else{
				down=down&((1<<*pElevator1)^255);
				*pe=*pe&((1<<*pElevator1)^255);
			}
		}
	}
	else{
		f=7;
		while(f>=0){
			if(f>*pElevator1){
				if(*pe&(1<<f)){*pup1=1;break;}
				if(up&(1<<f)){if(*pdown2||*pElevator2<*pElevator1||*pElevator2==*pElevator1&&!*pup2||*pElevator2>f){*pup1=1;break;}}
				if(down&(1<<f)){if(*pdown2||*pElevator2<*pElevator1||*pElevator2==*pElevator1&&!*pup2||*pElevator2>f){*pup1=1;break;}}
			}
			else if(f<*pElevator1){
				if(*pe&(1<<f)){*pdown1=1;break;}
				if(down&(1<<f)){if(*pup2||*pElevator2>*pElevator1||*pElevator2==*pElevator1&&!*pdown2||*pElevator2<f){*pdown1=1;break;}}
				if(up&(1<<f)){if(*pup2||*pElevator2>*pElevator1||*pElevator2==*pElevator1&&!*pdown2||*pElevator2<f){*pdown1=1;break;}}
			}
			else{
				up=up&((1<<f)^255);
				down=down&((1<<f)^255);
				*pe=*pe&((1<<f)^255);
			}
			f--;
		}
	}
}
void main(){
	int i;
	bit push=1;
	display_elevator();
	while(1){
		i=0;
		while(i<120){
			if((p13&p17&p23&p27)^push){
				if(push){
					push=0;
					if(!p13){
						up=up|(1<<(p10|p11*2|p12*4))&127;
					}
					if(!p17){
						down=down|(1<<(p14|p15*2|p16*4))&254;
					}
					if(!p23){
						e1=e1|(1<<(p20|p21*2|p22*4));
					}
					if(!p27){
						e2=e2|(1<<(p24|p25*2|p26*4));
					}
				}
				else{
					push=1;
				}
			}
			i++;
			display_LED();
		}
		move(&up1,&up2,&down1,&down2,&e1,&Elevator1,&Elevator2);
		move(&up2,&up1,&down2,&down1,&e2,&Elevator2,&Elevator1);
		display_elevator();
	}
}