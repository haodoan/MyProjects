/*********************************************************************************************************
*
* File                : app_cfg.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__
					  

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/
#define  OS_VIEW_MODULE                  DEF_ENABLED	     	/* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */


/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/ 

#define APP_TASK_START_PRIO     1
#define OS_VIEW_TASK_PRIO       2
#define READ_TASK_PRIORITY       4
#define WRITE_TASKA_PRIORITY     5
#define WRITE_TASKB_PRIORITY     6
#define WRITE_TASKC_PRIORITY     7
#define APP_TASK_OSVIEW_TERMINAL_PRIO	 (OS_LOWEST_PRIO - 6)
#define OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 2)




/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/
#define	WRITE_TASK_STACK_SIZE       					128u
#define READ_TASK_STACK_SIZE     							128u


#define  APP_TASK_START_STK_SIZE              64u
#define  APP_TASK_OSVIEW_TERMINAL_STK_SIZE   	256u
#define  OS_VIEW_TASK_STK_SIZE                256u




/*
*********************************************************************************************************
*                                                  LIB
*********************************************************************************************************
*/

#define  uC_CFG_OPTIMIZE_ASM_EN                 DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                      DEF_DISABLED



#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

