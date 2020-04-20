/*
 * MyNewTask.c
 *
 *  Created on: 19 abr. 2020
 *      Author: edson
 */
#include "MyNewTask.h"

osaEventId_t mMyEvents;
/* Global Variable to store our TimerID */
tmrTimerID_t myTimerID = gTmrInvalidTimerID_c;
/* Handler ID for task */
osaTaskId_t gMyTaskHandler_ID;
/* Local variable to store the current state of the LEDs */
static uint8_t ledsState = 0;

static void myTaskTimerCallback(void *param)
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent2_c);
}

static void SwitchCallback(void *param)
{
	OSA_EventSet(mMyEvents, gMyNewTaskEvent2_c);
}
/* Main custom task */
void Task_Counter(osaTaskParam_t argument)
{
	osaEventFlags_t customEvent;
	myTimerID = TMR_AllocateTimer();
	uint8_t counter = 0;
	while(1)
	{
		OSA_EventWait(mMyEvents, osaEventFlagsAll_c, FALSE, osaWaitForever_c,&customEvent);
		if( !gUseRtos_c && !customEvent)
		{
			break;
		}
		/* Depending on the received event */
		switch(customEvent){
		case gMyNewTaskEvent1_c:
			TMR_StartIntervalTimer(myTimerID, /*myTimerID*/
					3000, /* Timer's Timeout  */
					myTaskTimerCallback, /* pointer to myTaskTimerCallback function */
					NULL
			);
			TurnOffLeds(); /* Ensure all LEDs are turned off */
			break;
		case gMyNewTaskEvent2_c: /* Event called from myTaskTimerCallback */

			if(counter == 0 )
			{
				TurnOffLeds();
				Led_RGB_Red(LED1,1);
				Led_TurnOn(LED1);
				counter++;
				break;
			}

			if(counter == 1 || gKBD_EventSW3_c)
			{
				TurnOffLeds();
				Led_RGB_Green(LED1,1);
				Led_TurnOn(LED1);
				counter++;
				break;
			}

			if(counter == 2 || gKBD_EventSW4_c)
			{
				TurnOffLeds();
				Led_RGB_Blue(LED1,1);
				Led_TurnOn(LED1);
				counter++;
				break;

			}

			if(counter == 3)
			{
				turnOffLeds();
				TurnOnLeds();
				counter = 0;
				break;
			}
		case gMyNewTaskEvent3_c: /* Event to stop the timer */
			ledsState = 0;
			TurnOffLeds();
			TMR_StopTimer(myTimerID);
			break;
		default:
			break;
		}
	}
}

OSA_TASK_DEFINE(Task_Counter, gMyTaskPriority_c, 1, gMyTaskStackSize_c, FALSE );




void MyTask_Init(void)
{
mMyEvents = OSA_EventCreate(TRUE);
/* The instance of the MAC is passed at task creaton */
gMyTaskHandler_ID = OSA_TaskCreate(OSA_TASK(Task_Counter), NULL);
KBD_Init(SwitchCallback);
}

void MyTaskTimer_Stop(void)
{
OSA_EventSet(mMyEvents, gMyNewTaskEvent3_c);
}

void MyTaskTimer_Start(void)
{
OSA_EventSet(mMyEvents, gMyNewTaskEvent1_c);
}
