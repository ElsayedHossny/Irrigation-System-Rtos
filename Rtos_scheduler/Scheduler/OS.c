#include "OS.h"

static OS_Stuct taskArray[MAX_NUM_OF_TASKS];
static u8 taskCounter = 0;

void OS_TaskState(fncPtr task , OS_state state){
	for(u8 i=0; i<MAX_NUM_OF_TASKS ; i++){
		if(taskArray[i].function == task){
			taskArray[i].task_state = state;
		}
	}	
}


OS_feedback OS_TaskCreate(fncPtr function, u8 periodicity, OS_state default_state)
{	
	
	OS_feedback errorState =OK;	
	
	if(taskCounter >=MAX_NUM_OF_TASKS)
	{	
		errorState = NOK_COUNT_LIMIT;
	}
	else if( (periodicity>HYPER_PERIOD) || (periodicity==0) )
	{
		errorState=NOK_TIME_LIMIT;
	}
	else if(function ==NULLPTR)
	{
		errorState = NOK_NULL_POINTER;
	}
	else
	{
			// Create task ->TCB
			taskArray[taskCounter].function = function;
			taskArray[taskCounter].task_periodicity= periodicity;
			taskArray[taskCounter].task_state= default_state;
			taskArray[taskCounter].task_Counter = 1; // 1->default 					
			taskCounter++;	
	}

			return errorState ;	
}

/* LInked to ISR --- Time --- Task States*/ 
void OS_Task_Handler(void)
{
	u8 task_id=0; //index
		for( task_id ; task_id < taskCounter ; task_id++){
			
				if(taskArray[task_id].task_Counter >= taskArray[task_id].task_periodicity  && taskArray[task_id].task_state != SUSPENDED){
					/*if(taskArray[task_id].task_state != SUSPENDED){}*/
					taskArray[task_id].task_state = READY; //change state To Ready
					taskArray[task_id].task_Counter = 1; // 1->default 
				}
				else{
					taskArray[task_id].task_Counter++;
				}
		}
}


void OS_TaskExecution(void)
{
	u8 task_id=0; //index
	// check if (ready ->execute) And-->set state to Blocked
	
	for( task_id ; task_id < taskCounter ; task_id++){
		
		if(taskArray[task_id].task_state == READY){
			//Run_Fun
			taskArray[task_id].function();
			taskArray[task_id].task_state = BLOCKED ;
		}
		
	}
}
