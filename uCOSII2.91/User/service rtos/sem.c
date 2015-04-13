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
#include "sem.h"

/*
** Global data structure accessible by read and write tasks.
** It contains two semaphores that govern access to
** the data variable.
*/
typedef struct sw_fifo
{
    semaphore_t         READ_SEM;
    semaphore_t         WRITE_SEM;
    char                DATA;
} SW_FIFO, * SW_FIFO_PTR;

SW_FIFO     fifo;
/*TASK************************************************************************
*
* Task Name    : write_taskA
* Comments     : This task waits for the write semaphore,
*                then writes a character to "data" and posts a read semaphore.

*END*************************************************************************/

void write_taskA( task_param_t init_data )
{
    printf("Running  write_taskA %c\r\n",  (char)init_data);

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
    printf("Running  write_taskC %c\r\n",  (char)init_data);

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

    result = OSA_SemaCreate(&fifo.WRITE_SEM, 2);
    if (result != kStatus_OSA_Success) {
        printf("Creating write_sem failed: 0x%02X\r\n", result);
    }

    /* Create the write tasks */
    result = OSA_TaskCreate(   write_taskA,
                            "write_taskA",
                            WRITE_TASK_STACK_SIZE,
                            write_taskA_stack,
                            WRITE_TASKA_PRIORITY_OSA,
                            (task_param_t)('A'),
                            false,
                            &write_taskA_task_handler
                        );
    if (result != kStatus_OSA_Success) {
        printf("Creating write_taskA %c failed\r\n", (char)('A'));
    }
    else    {
        printf("Creating write_taskA %c\r\n", (char)('A'));
    }
    
    result = OSA_TaskCreate(   write_taskB,
                            "write_taskB",
                            WRITE_TASK_STACK_SIZE,
                            write_taskB_stack,
                            WRITE_TASKB_PRIORITY_OSA,
                            (task_param_t)('B'),
                            false,
                            &write_taskB_task_handler
                        );
    if (result != kStatus_OSA_Success) {
        printf("Creating write_taskA %c failed\r\n", (char)('B'));
    }
    else    {
        printf("Creating write_taskA %c\r\n", (char)('B'));
    }
    
    result = OSA_TaskCreate(   write_taskC,
                            "write_taskC",
                            WRITE_TASK_STACK_SIZE,
                            write_taskC_stack,
                            WRITE_TASKC_PRIORITY_OSA,
                            (task_param_t)('C'),
                            false,
                            &write_taskC_task_handler
                        );
    if (result != kStatus_OSA_Success) {
        printf("Creating write_taskA %c failed\r\n", (char)('C'));
    }
    else    {
        printf("Creating write_taskA %c\r\n", (char)('C'));
    }    
    

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