
#ifndef OS_H_
#define OS_H_

#include "../StdTypes.h"

typedef void (*fncPtr) (void);

#define MAX_NUM_OF_TASKS ((u8) 7)
#define HYPER_PERIOD 70

typedef enum{
	BLOCKED,
	READY,
	SUSPENDED,
}OS_state;

// TCB
typedef struct{
	fncPtr      function; //Function_Called
	u8          task_periodicity; //periodicity
	OS_state    task_state;//states
	u8          task_Counter;
}OS_Stuct;

typedef enum{
	// Error_handel
	OK=0,
	NOK_COUNT_LIMIT,
	NOK_TIME_LIMIT,
	NOK_NULL_POINTER
	}OS_feedback;

void OS_TaskState(fncPtr task , OS_state default_state);

OS_feedback OS_TaskCreate(fncPtr task, u8 default_time_burst, OS_state default_state);

void OS_Task_Handler(void);

void OS_TaskExecution(void);


#endif /* OS_H_ */
