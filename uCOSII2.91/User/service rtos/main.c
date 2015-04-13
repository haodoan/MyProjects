/*HEADER**********************************************************************
*
* Copyright 2008, 2014 Freescale Semiconductor, Inc.
* Copyright 1989-2008 ARC International
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*
*   This file contains the source for the sync osa example program.
*
*END************************************************************************/

#include <stdio.h>
#include <assert.h>

#include "fsl_os_abstraction.h"
#include "main.h"

#if defined(USE_SEM)
/* Handlers definitions */
OSA_TASK_DEFINE(read_task, READ_TASK_STACK_SIZE);
OSA_TASK_DEFINE(write_taskA, WRITE_TASK_STACK_SIZE);
OSA_TASK_DEFINE(write_taskB, WRITE_TASK_STACK_SIZE);
OSA_TASK_DEFINE(write_taskC, WRITE_TASK_STACK_SIZE);

extern void write_taskA(task_param_t param);
extern void write_taskB(task_param_t param);
extern void write_taskC(task_param_t param);
extern void read_task(task_param_t param );
#elif defined(USE_MUTEX)
OSA_TASK_DEFINE(print_task1, PRINT_TASK_STACK_SIZE);
OSA_TASK_DEFINE(print_task2, PRINT_TASK_STACK_SIZE);

extern void print_task1(task_param_t param);
extern void print_task2(task_param_t param);
#elif defined(USE_MSG)
OSA_TASK_DEFINE(server_task,  SERVER_TASK_STACK_SIZE);
OSA_TASK_DEFINE(client_task1, CLIENT_TASK1_STACK_SIZE);
OSA_TASK_DEFINE(client_task2, CLIENT_TASK2_STACK_SIZE);

void server_task ( task_param_t init_data );
void client_task1 ( task_param_t init_data );
void client_task2 ( task_param_t init_data );
char *strings[] = {"message 1","message 2","message 3"};
#endif
    
int main(void)
{
    osa_status_t     result;
    // hardware init need call here 
    hardware_init() ;
    dbg_uart_init();
    puts("\r\n****************** Sync OSA Example ******************\r\n");
    
    OSA_Init();
    /* Create print tasks */
    result = OSA_TaskCreate( read_task,
                          "read task",
                          READ_TASK_STACK_SIZE,
                          read_task_stack,
                          READ_TASK_PRIORITY_OSA,
                          (task_param_t)0,
                          false,
                          &read_task_task_handler
                        );
    if (result != kStatus_OSA_Success) {
        printf("Task creation failed.\r\n");
    }
    OSA_Start();
    
    for(;;) {}                   
    return 1;
}
