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
*   This file contains the source for the msgq osa example program.
*
*END************************************************************************/

#include <stdio.h>
#include <assert.h>

#include "fsl_os_abstraction.h"
#include "msgq.h"

/* Handlers definitions */
msg_queue_handler_t server_queue_handler;
msg_queue_handler_t client_queue_handler;

/* Use light weight message queues */
MSG_QUEUE_DECLARE(server_queue, NUM_MESSAGES, MSG_SIZE);
MSG_QUEUE_DECLARE(client_queue, NUM_MESSAGES, MSG_SIZE);

OSA_TASK_DEFINE(server_task,  SERVER_TASK_STACK_SIZE);
OSA_TASK_DEFINE(client_task1, CLIENT_TASK1_STACK_SIZE);
OSA_TASK_DEFINE(client_task2, CLIENT_TASK2_STACK_SIZE);

void server_task ( task_param_t init_data );
void client_task1 ( task_param_t init_data );
void client_task2 ( task_param_t init_data );
char *strings[] = {"message 1","message 2","message 3"};


int main(void)
{
    osa_status_t     result;
    // hardware init need call here 
    hardware_init() ;
    dbg_uart_init();    
    OSA_Init();
    /* Create tasks */
    result = OSA_TaskCreate( server_task,
                          "server task",
                          SERVER_TASK_STACK_SIZE,
                          server_task_stack,
                          SERVER_TASK_PRIORITY_OSA,
                          (task_param_t)0,
                          false,
                          &server_task_task_handler
                        );
    if (result != kStatus_OSA_Success) {
        printf("Task creation failed.\r\n");
    }
    OSA_Start();
    
    for(;;) {}                   
    return 1;
}


/*TASK************************************************************************
*
* Task Name : server_task
* Comments  : This task initializes the message queues,
*  creates three client tasks, and then waits for a message.
*  After recieving a message, the task returns the message to
*  the sender.

*END*************************************************************************/

void server_task ( task_param_t init_data )
{
    char         msg[10];
    msg_queue_t         *pmsg;
    osa_status_t        result;
    uint32_t            i;    
    puts("\r\n****************** MSGQ OSA Example ******************\r\n");

    server_queue_handler = OSA_MsgQCreate((msg_queue_t *)server_queue, NUM_MESSAGES, MSG_SIZE);
    client_queue_handler = OSA_MsgQCreate((msg_queue_t *)client_queue, NUM_MESSAGES, MSG_SIZE);

    /* create the client tasks */
    printf("Creating client task 1\r\n");
    result = OSA_TaskCreate( client_task1,
                          "client",
                          CLIENT_TASK1_STACK_SIZE,
                          client_task1_stack,
                          CLIENT_TASK1_PRIORITY_OSA,
                          (task_param_t)strings[0],
                          false,
                          &client_task1_task_handler
                        );
    if (result != kStatus_OSA_Success) {
        /* task creation failed */
    } /* Endif */
    OSA_TimeDelay(100);
    
    printf("Creating client task 2\r\n");
    result = OSA_TaskCreate( client_task2,
                          "client",
                          CLIENT_TASK2_STACK_SIZE,
                          client_task2_stack,
                          CLIENT_TASK2_PRIORITY_OSA,
                          (task_param_t)strings[1],
                          false,
                          &client_task2_task_handler
                        );
    if (result != kStatus_OSA_Success) {
        /* task creation failed */
    } /* Endif */
    OSA_TimeDelay(100);    

    printf("***************************************************\r\n");

    while (1) {
        OSA_MsgQGet(server_queue_handler, msg, OSA_WAIT_FOREVER);
        printf("Server task received :%s \r\n", msg);
        OSA_MsgQPut(client_queue_handler, msg);
    }
}


/*TASK************************************************************************
*
* Task Name : client_task
* Comments  : This task sends a message to the server_task and
*   then waits for a reply.
*
*END*************************************************************************/

void client_task1( task_param_t init_data )
{
    msg_queue_t msg[MSG_SIZE];
    char *taskid = (char *)init_data;
    printf("Client task %c created\r\n", taskid[8]);
    /* Wait for other client task to be created */
    OSA_TimeDelay(500);	

    while (1) {
        //OSA_TimeDelay(500);	
        printf("Client task %c sending %s\r\n", taskid[8], (char*)init_data);
	OSA_TimeDelay(500);	
        OSA_MsgQPut(server_queue_handler, (char*)init_data);
        /* wait for a return message */
        OSA_MsgQGet(client_queue_handler, msg, OSA_WAIT_FOREVER);
    }
}

/*TASK************************************************************************
*
* Task Name : client_task
* Comments  : This task sends a message to the server_task and
*   then waits for a reply.
*
*END*************************************************************************/

void client_task2( task_param_t init_data )
{
    msg_queue_t msg[MSG_SIZE];
    char *taskid = (char *)init_data;
    printf("Client task %c created\r\n", taskid[8]);
    /* Wait for other client task to be created */
    OSA_TimeDelay(500);	

    while (1) {
        //OSA_TimeDelay(500);	
        printf("Client task %c sending %s\r\n", taskid[8], (char*)init_data);
	OSA_TimeDelay(500);	
        OSA_MsgQPut(server_queue_handler, (char*)init_data);
        /* wait for a return message */
        OSA_MsgQGet(client_queue_handler, msg, OSA_WAIT_FOREVER);
    }
}
