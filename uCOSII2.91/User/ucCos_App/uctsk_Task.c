/*********************************************************************************************************
*
* File                : uctsk_Task.c
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

/* Includes ------------------------------------------------------------------*/
#include <includes.h>            
#include "os_abstraction.h"
#include "app_cfg.h"
/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_read_task               [READ_TASK_STACK_SIZE];
static  OS_STK         App_write_taskA             [WRITE_TASK_STACK_SIZE];
static  OS_STK         App_write_taskB             [WRITE_TASK_STACK_SIZE];
static  OS_STK         App_write_taskC             [WRITE_TASK_STACK_SIZE];

/* Private function prototypes -----------------------------------------------*/

/* task function propotypes */
void write_taskA(void* param);
void write_taskB(void* param);
void write_taskC(void* param);
void read_task  (void* param );

typedef struct sw_fifo
{
    semaphore_t         READ_SEM;
    semaphore_t         WRITE_SEM;
    char                DATA;
} SW_FIFO, * SW_FIFO_PTR;


SW_FIFO     fifo;

 
void  MyApp_TaskCreate (void)
{
    CPU_INT08U  os_err;

	  os_err = os_err; 
	  os_err = OSTaskCreate((void (*)(void *)) read_task,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_read_task[READ_TASK_STACK_SIZE - 1],		
                          (INT8U           ) READ_TASK_PRIORITY  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(READ_TASK_PRIORITY, "Task LED Blink", &os_err);
	#endif


}							 


static void uctsk_Blink (void) 
{                 
    for(;;)
   	{  
     	
        /*====LED-ON=======*/
	    GPIO_SetBits(GPIOB , GPIO_Pin_12);
	    GPIO_SetBits(GPIOB , GPIO_Pin_13);
	    GPIO_SetBits(GPIOB , GPIO_Pin_14);
	    GPIO_SetBits(GPIOB , GPIO_Pin_15);
		OSTimeDlyHMSM(0, 0, 0, 50);	 /* 50 MS  */
      	
	    /*====LED-OFF=======*/ 
	    GPIO_ResetBits(GPIOB , GPIO_Pin_12);
	    GPIO_ResetBits(GPIOB , GPIO_Pin_13);
	    GPIO_ResetBits(GPIOB , GPIO_Pin_14);
	    GPIO_ResetBits(GPIOB , GPIO_Pin_15); 
		OSTimeDlyHMSM(0, 0, 0, 50);	 /* 50 MS  */
    }
}

/*TASK************************************************************************
*
* Task Name    : write_taskA
* Comments     : This task waits for the write semaphore,
*                then writes a character to "data" and posts a read semaphore.

*END*************************************************************************/

void write_taskA( task_param_t init_data )
{
    printf("Running  write_taskA %c\r\n",  (char*)init_data);

    while (1)
    {
        if (OSA_SemaWait(&fifo.WRITE_SEM, OSA_WAIT_FOREVER) != kStatus_OSA_Success) {
            printf("\nsync_wait failed");
        }

        OSA_TimeDelay(100);

        fifo.DATA = (char)init_data;
        OSA_SemaPost(&fifo.READ_SEM);
    }
}

/*TASK************************************************************************
*
* Task Name    : write_taskA
* Comments     : This task waits for the write semaphore,
*                then writes a character to "data" and posts a read semaphore.

*END*************************************************************************/

void write_taskB( task_param_t init_data )
{
    printf("Running  write_taskB %c\r\n",  (char)init_data);

    while (1)
    {
        if (OSA_SemaWait(&fifo.WRITE_SEM, OSA_WAIT_FOREVER) != kStatus_OSA_Success) {
            printf("\nsync_wait failed");
        }

        OSA_TimeDelay(100);

        fifo.DATA = (char)init_data;
        OSA_SemaPost(&fifo.READ_SEM);
    }
}

/*TASK************************************************************************
*
* Task Name    : write_taskA
* Comments     : This task waits for the write semaphore,
*                then writes a character to "data" and posts a read semaphore.

*END*************************************************************************/

void write_taskC( task_param_t init_data )
{
    printf("Running  write_taskC %c\r\n",  (char*)init_data);

    while (1)
    {
        if (OSA_SemaWait(&fifo.WRITE_SEM, OSA_WAIT_FOREVER) != kStatus_OSA_Success) {
            printf("\nsync_wait failed");
        }

        OSA_TimeDelay(100);

        fifo.DATA = (char)init_data;
        OSA_SemaPost(&fifo.READ_SEM);
    }
}
/*TASK************************************************************************
*
* Task Name : read_task
* Comments  : This task creates two semaphores and NUM_WRITER write_taskAs.
*             It waits on the read_sem and finally outputs the "data" variable.
*
*END*************************************************************************/

void read_task( task_param_t init_data )
{
    osa_status_t        result;
    //uint32_t            i;

    //puts("\r\n****************** Sync OSA Example ******************\r\n");

    /* Create OSA semaphore */
    result = OSA_SemaCreate(&fifo.READ_SEM, 0);
    if (result != kStatus_OSA_Success) {
        printf("Creating read semaphore failed: 0x%02X\r\n", result);
    }

    result = OSA_SemaCreate(&fifo.WRITE_SEM, 3);
    if (result != kStatus_OSA_Success) {
        printf("Creating write_sem failed: 0x%02X\r\n", result);
    }

			OSTaskCreate((void (*)(void *)) write_taskA,				
                          (char          * ) 'A',							
                          (OS_STK        * )&App_write_taskA[WRITE_TASK_STACK_SIZE - 1],		
                          (INT8U           ) WRITE_TASKA_PRIORITY);	

			OSTaskCreate((void (*)(void *)) write_taskB,				
                          (char          * ) 'B',							
                          (OS_STK        * )&App_write_taskB[WRITE_TASK_STACK_SIZE - 1],		
                          (INT8U           ) WRITE_TASKB_PRIORITY);	

			OSTaskCreate((void (*)(void *)) write_taskC,				
                          (char          * ) 'C',							
                          (OS_STK        * )&App_write_taskC[WRITE_TASK_STACK_SIZE - 1],		
                          (INT8U           ) WRITE_TASKC_PRIORITY);														


    while(1)
    {
        result = OSA_SemaWait(&fifo.READ_SEM, OSA_WAIT_FOREVER);
        if (result != kStatus_OSA_Success) {
            printf("\n_lwsem_wait failed: 0x%X", result);
        }

        putchar(fifo.DATA);
        OSA_SemaPost(&fifo.WRITE_SEM);
    }
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
