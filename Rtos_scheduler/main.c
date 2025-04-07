#define  F_CPU  16000000UL
#include <util/delay.h>

#include "MemMap.h"
#include "StdTypes.h"
#include "Utils.h"
#include "MCAL/DIO_Interface.h"
#include "HAL/LCD.h"
#include "HAL/KeyPad.h"
#include "MCAL/ADC.h"
#include "HAL/Sensors.h"
#include "MCAL/EXInterrupt.h"
#include "MCAL/Timers.h"
#include "SERVICES/TimerServices.h"
#include "HAL/servo.h"
#include "HAL/ultrasonic.h"
#include "SERVICES/UART_Services.h"
#include "MCAL/UART.h"
#include "MCAL/SPI.h"
//slave
#include "Scheduler/OS.h"
#include "Scheduler/Tasks.h"
#include "HAL/Sensor_DHT22_int.h"

// ISR Callback
void vMillisIncrement(void)
{	
	static u16 u16_100uSecCounter = 0;
	static u16 u16_millisCounter = 0;
	u16_100uSecCounter++;

	if(u16_100uSecCounter >= 10)
	{
		u16_100uSecCounter = 0;
		u16_millisCounter++;
	}

	if(u16_millisCounter >= 1000)
	{
		// To be called every 1 Sec
		OS_Task_Handler();
		// Reset u16_millisCounter
		u16_millisCounter = 0;
	}
	TCNT0 = 156;  // to int timer to calculation 1us
}



void vInitEcu(void)
{
	u8 str[] = {"Seconds: "};
	Timer0_Init(TIMER0_NORMAL_MODE, TIMER0_SCALER_8, OCO_DISCONNECTED);
	Timer0_OV_InterruptEnable();
	Timer0_OC_SetCallBack(vMillisIncrement);
	
Timer1_Init(TIMER1_NORMAL_MODE,TIMER1_SCALER_8,OCRA_DISCONNECTED,OCRB_DISCONNECTED);
TCNT1=0;

	sei();
	DIO_Init();
	DIO_InitPin(PINB0,OUTPUT);
	DIO_InitPin(PINB2,OUTPUT);

	DIO_WritePin(PINB0,LOW);
	DIO_WritePin(PINB2,LOW);

	LCD_Init();
	LCD_WriteString(str);
    DHT_SensorINT();
}

/*
void Task_DisplaySystem(void);
void Task_Pump_Control(void);
void Task_ErrorHandle(void);
void Task_Critical_Irrigation(void);
void Task_Periodic_Irrigation(void);
void Task_SensorRunnable(void);
*/

void InitScheduler(void)
{
	// 1 ->mean (0.5)s because mcu 16MHZ
	OS_TaskCreate(Task_1S_LcdUpdate,1,BLOCKED);
	
	OS_TaskCreate(Task_Pump_Control,2,SUSPENDED);
	
	OS_TaskCreate(Task_Critical_Irrigation,15,BLOCKED); // 15 s
	
	OS_TaskCreate(Task_Periodic_Irrigation, 60 ,BLOCKED); // 60 s
	
	OS_TaskCreate(Task_SensorRunnable,5,BLOCKED); // 5 s
	
	OS_TaskCreate(Task_ErrorHandle,6,BLOCKED);
	
	OS_TaskCreate(Task_DisplaySystem,10,BLOCKED); //10sec
}


int main(void)
{
	// Sw initilaization
	vInitEcu();
   
	// Start the Scheduler
	InitScheduler();
	
	while(1)
	{
		// Execute the scheduler
		OS_TaskExecution();
	}
}



