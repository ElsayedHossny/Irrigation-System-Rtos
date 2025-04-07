#include "Tasks.h"
#include <util/delay.h>
#include "../MemMap.h"
#include "../StdTypes.h"
#include "../Utils.h"
#include "../MCAL/DIO_Interface.h"
#include "../HAL/LCD.h"
#include "../HAL/KeyPad.h"
#include "../MCAL/ADC.h"
#include "../HAL/Sensors.h"
#include "../MCAL/EXInterrupt.h"
#include "../MCAL/Timers.h"
#include "../SERVICES/TimerServices.h"
#include "../HAL/servo.h"
#include "../HAL/ultrasonic.h"
#include "../SERVICES/UART_Services.h"
#include "../MCAL/UART.h"
#include "../MCAL/SPI.h"
#include "OS.h"

#include "../HAL/Sensor_DHT22_int.h"
/*
void DHT_SensorRunnable(void); // Tr=5s
u16 DHT_Tempgetter(void);
u16 DHT_Humiditygetter(void);
u16 DHT_CRCSumgetter(void);
u16 DHT_CheckSum(void)
*/
#define Threshold_Humidity 20  //Threshold-Irrigation 200->mean 20.0

#define Humidity_openPump 40 //Normal-Irrigation

#define PUMP_ON_TIME 10 //10 second

static volatile u8 Error_Count=0;

volatile u8 Flag=0;

void Task_SensorRunnable(void){
	//periodic Runnable Sensor after 5sec
	DHT_SensorRunnable();
}

void Task_Periodic_Irrigation(void){
	//periodic Irrigation after 60sec
	u8 HUM =((u16)DHT_Humiditygetter() /10 );
	if( HUM < Humidity_openPump){
		OS_TaskState(Task_Pump_Control , BLOCKED);
		Flag=1;
	}else{
		OS_TaskState(Task_Pump_Control , SUSPENDED);
	}
}

void Task_Critical_Irrigation(void){
	//periodic Irrigation after 15sec
	u16 HUM =( DHT_Humiditygetter() /10 );
	if(HUM < Threshold_Humidity){
		OS_TaskState(Task_Pump_Control , BLOCKED);
		Flag=1;
	}else{
		OS_TaskState(Task_Pump_Control , SUSPENDED);
	}
}

void Task_ErrorHandle(void){
	//periodic after 6sec	
	if(DHT_CRCSumgetter() == Check_Sum_Only()){
		OS_TaskState(Task_Pump_Control , BLOCKED);
		Error_Count=0;
	}else{
		OS_TaskState(Task_Pump_Control , SUSPENDED);
		Error_Count++;
	}
}

void Start_Pump(void) {
	DIO_WritePin(PINA0,LOW);
	DIO_WritePin(PINA1,HIGH);
}

void Stop_Pump(void) {
	DIO_WritePin(PINA0,LOW);
	DIO_WritePin(PINA1,LOW);
	OS_TaskState(Task_Pump_Control , SUSPENDED);
	Flag=0;
}

void Task_Pump_Control(void){
	//periodic after 2sec (Should when create -> State Suspended)
	static u8 Time_Count=0;
	if(Flag){
	Start_Pump();
	Time_Count+=2;
	}
	if(Time_Count > PUMP_ON_TIME ) //5 because i enter function after 2sec
	{
		Stop_Pump();
		Time_Count=0;	
	}
}


void Task_DisplaySystem(void){
	//periodic after 10sec
	LCD_GoTo(1,0);
	LCD_WriteString((u8*)"      ");
	LCD_GoTo(1,0);
	LCD_WriteString((u8*)"H=");
	LCD_WriteNumber(DHT_Humiditygetter());
	
	if(Error_Count >= 2){
		Error_Count=0;
		LCD_GoTo(1,0);
		LCD_WriteString((u8*)"Error");
	}
}


void Task_1S_LcdUpdate(void)
{
	static u16 u16Seconds = 1;
	LCD_GoTo(0,10);
	LCD_WriteNumber(u16Seconds);
	u16Seconds++;
}


/*


void Task_4S_Runnable(void){
	//Runnable Sensor (T=5sec)
	DHT_SensorRunnable();
}

void Task_5S_Blink(void){
	//Function to use Suspend State
	static u8 f=0;
	f++;
	u16 Calc ;
	if(f==3){
		Calc= Check_Sum_Only()+1 ;
	}else{
		Calc= Check_Sum_Only();
	}
	if(DHT_CRCSumgetter() == Calc){
		OS_TaskState(Task_6_GetVAlUE , BLOCKED);
		LCD_GoTo(1,0);
		LCD_WriteString((u8*)"                 ");
	}
	else{
		OS_TaskState(Task_6_GetVAlUE , SUSPENDED);
		LCD_GoTo(1,0);
		LCD_WriteString((u8*)"Error_Sum        ");
	}
}

void Task_6_GetVAlUE(void){
	LCD_GoTo(1,0);
	LCD_WriteString((u8*)"T=");
	LCD_WriteNumber(DHT_Tempgetter());
	LCD_GoTo(1,7);
	LCD_WriteString((u8*)"H=");
	LCD_WriteNumber(DHT_Humiditygetter());
}


void Task_2S_Blink(void)
{
	DIO_TogglePin(PINB0);
}




void Task_3S_Blink(void)
{
	DIO_TogglePin(PINB2);
}
*/
