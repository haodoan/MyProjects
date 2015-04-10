/****************************************Copyright (c)****************************************************
**                                      
**                                  Kien thuc khong bao gio la huu han.
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               
** Descriptions:            File khai bao cac ham cai dat
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Hai Dang
** Created date:            2010-10-30
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/
#include "tim2_interrupt.h"
/*******************************************************************************
* Function Name  : TIM_time2_Configuration
* Description    : TIM_Configuration program.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern void TIM_time2_Configuration(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
		TIM_DeInit(TIM2);
		TIM_TimeBaseStructure.TIM_Period=1; /*2000*/
																		
		TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM2, ENABLE);										
}
/*******************************************************************************
* Function Name  : NVIC_time2_Configuration
* Description    : Configuration the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern void NVIC_time2_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure; 
		 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 Handler.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

extern void TIM2_IRQHandler(void)
{
		if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
		{
			 TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
    }	
}
