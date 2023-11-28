/*
 * scheduler.c
 *
 *  Created on: Nov 27, 2023
 *      Author: LENOVO
 */

#include <scheduler.h>
sTask SCH_tasks_G[SCH_MAX_TASKS];

uint32_t current_task = 0;

void SCH_Init(){
	current_task = 0;
}

void SCH_Update(void){
	if(SCH_tasks_G[0].pTask && SCH_tasks_G[0].RunMe == 0){
		if(SCH_tasks_G[0].Delay > 0){
			SCH_tasks_G[0].Delay--;
		}
		if(SCH_tasks_G[0].Delay == 0){
			SCH_tasks_G[0].RunMe = 1; // flag = 1 -> run
		}
	}
}

void SCH_Add_Task(void(*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
	if(current_task < SCH_MAX_TASKS){
		if(PERIOD == 0) return;
		uint32_t i = 0;
		while(DELAY >= SCH_tasks_G[i].Delay && i < current_task){
			DELAY -= SCH_tasks_G[i].Delay;
			i++;
		}
		if(i != current_task){
			for(uint32_t index = current_task; index > i; index--){
				SCH_tasks_G[index].pTask = SCH_tasks_G[index - 1].pTask;
				SCH_tasks_G[index].Delay = SCH_tasks_G[index - 1].Delay;
				SCH_tasks_G[index].Period = SCH_tasks_G[index - 1].Period;
				SCH_tasks_G[index].RunMe = SCH_tasks_G[index - 1].RunMe;
				SCH_tasks_G[index].TaskID = index;
			}
			SCH_tasks_G[i+1].Delay -= DELAY; //update delay time of next task
		}
		SCH_tasks_G[i].pTask = pFunction;
		SCH_tasks_G[i].Delay = DELAY;
		SCH_tasks_G[i].Period = PERIOD;
		SCH_tasks_G[i].RunMe = 0;
		SCH_tasks_G[i].TaskID = i;
		current_task++;
	}
}


void SCH_Dispatch_Tasks(void){
	if(SCH_tasks_G[0].RunMe > 0){
		(*SCH_tasks_G[0].pTask)(); // run task
		void(*pFunction)() = SCH_tasks_G[0].pTask;
		uint32_t period = SCH_tasks_G[0].Period;
		SCH_Delete_Task(0);
		SCH_Add_Task(pFunction, period, period);
	}
}

void SCH_Delete_Task(uint32_t taskID){
	for(uint32_t i = taskID; i < current_task; i ++){
		SCH_tasks_G[i].pTask = SCH_tasks_G[i+1].pTask;
		SCH_tasks_G[i].Delay = SCH_tasks_G[i+1].Delay;
		SCH_tasks_G[i].Period = SCH_tasks_G[i+1].Period;
		SCH_tasks_G[i].RunMe = SCH_tasks_G[i+1].RunMe;
		SCH_tasks_G[i].TaskID = i;
	}
	current_task--;
}
