#include "../StdTypes.h"
#include "../MCAL/DIO_Interface.h"
#include "../MCAL/Timers.h"
#define  F_CPU 16000000
#include "util/delay.h"
#include "Sensor_DHT22_int.h"

#define Tick_Time 0.5

static volatile u16 t1,t2,flag=0;

volatile u8 ARRICU_DATAValue[40];
void ICU_FUN(void);


void ICU_FUN(){
	static u8 Count=0 , i=0;
	Count++;
	if (flag==0)
	{
		t1=ICR1;
		Timer1_InputCaptureEdge(FALLING);
		flag=1;
	}
	else if (flag==1)
	{
		t2=ICR1;
		Timer1_InputCaptureEdge(RISING);
		flag=0;
		ARRICU_DATAValue[i]=((t2-t1)*Tick_Time);
		t1=0;t2=0;
		i++;
		TCNT1=0;
	}
	if(Count==80){
		i=0;
		Count=0;
		Timer1_ICU_InterruptDisable();
	}
	
}


void DHT_SensorINT(void){		
	    Timer1_InputCaptureEdge(RISING);
		Timer1_ICU_SetCallBack(ICU_FUN);
}

void DHT_SensorRunnable(void){
			DIO_InitPin(PIND6,OUTPUT);
			DIO_WritePin(PIND6,LOW);
			_delay_us(1300);
			DIO_WritePin(PIND6,HIGH);
			_delay_us(30);
			DIO_InitPin(PIND6,INFREE);
			while(!DIO_ReadPin(PIND6));
			while(DIO_ReadPin(PIND6));
			Timer1_ICU_InterruptEnable();		
}


static volatile u8 ST1=0,ST2=0,SH1=0,SH2=0;

u16 DHT_Tempgetter(void){
 	u16 TEMP=0;
	u8 Tbit=0 ;
		for(u8 i=0 ;i<16;i++){
			if(ARRICU_DATAValue[i] == 70){
				Tbit=1;
				}else{
					Tbit=0;
					}
			TEMP = (TEMP << 1) | Tbit;	
			if(i<8){
				ST1= (ST1<<1) |Tbit;
			}else if(i>=8 && i<16){
				ST2= (ST2<<1) |Tbit;
			}		
		}
		return TEMP;
}

u16 DHT_Humiditygetter(void){
	u16 Humidity=0;
	u8 Hbit=0;
		for(u8 i=16 ;i<32;i++){
			if(ARRICU_DATAValue[i] == 70){
				Hbit=1;
				}else{
				Hbit=0;
			}			
			Humidity= (Humidity << 1) | Hbit;
			if(i<24){
				SH1= (SH1<<1) |Hbit;
			}else if(i>=24 && i<32){
				SH2= (SH2<<1) |Hbit;
			}	
		}	
		return Humidity;
}

u16 Check_Sum_Only(void){	
u8 Hbit=0 ; 
	for (u8 i=0 ;i<=32 ;i++)
	{
		if(ARRICU_DATAValue[i] == 70)
		{
			Hbit=1;
		}
		else{
			Hbit=0;
		}
		
		if(i<8)
		{
			ST1= (ST1<<1) |Hbit;
		}
		else if(i>=8 && i<16)
		{
			ST2= (ST2<<1) |Hbit;
		}
		else if(i<24)
		{
			SH1= (SH1<<1) |Hbit;
		}
		else if(i>=24 && i<32)
		{
			SH2= (SH2<<1) |Hbit;
		}
	}
	return (ST1+ST2+SH1+SH2);
}


u16 DHT_CRCSumgetter(void){
	u16 Sum=0;
	u8 Sbit=0;
	for(u8 i=32 ; i<40 ; i++){
			if(ARRICU_DATAValue[i] == 70){
				Sbit=1;
				}else{
				Sbit=0;
			}			
			Sum= (Sum << 1) | Sbit;	
	}
	return Sum;
}

u16 DHT_CheckSum(void){	
	return (ST1+ST2+SH1+SH2);
}


