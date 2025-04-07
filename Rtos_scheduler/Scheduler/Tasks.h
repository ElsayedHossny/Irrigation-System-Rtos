/*
 * Tasks.h
 *
 *  Created on: May 7, 2021
 *      Author: Zack
 */

#ifndef SCHEDULER_TASKS_H_
#define SCHEDULER_TASKS_H_



void Start_Pump(void) ;
void Stop_Pump(void);

void Task_DisplaySystem(void);
void Task_Pump_Control(void);
void Task_ErrorHandle(void);
void Task_Critical_Irrigation(void);
void Task_Periodic_Irrigation(void);
void Task_SensorRunnable(void);
void Task_1S_LcdUpdate(void);

// void Task_4S_Runnable(void);
// void Task_5S_Blink(void);
// void Task_6_GetVAlUE(void);

// void Task_2S_Blink(void);
// void Task_3S_Blink(void);


#endif /* SCHEDULER_TASKS_H_ */
