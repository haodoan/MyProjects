/****************************************Copyright (c)****************************************************
**                                      
**                                  Kien thuc khong bao gio la huu han.
**
**--------------File Info---------------------------------------------------------------------------------
** File name:                Main.c
** Descriptions:            File khai bao cac ham cai dat
**
**--------------------------------------------------------------------------------------------------------
** Created by:               Hai Dang
** Created date:            2010-10-30
** Version:                    v1.0
** Descriptions:             The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#include "simcom.h"
#include "comm.h"
/* Exported types ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef struct usart1_it
{
	char *str_usart1;
	char usart1_interrupt_flag_complete;
}USART1_IT;
USART1_IT usart1;

uint32_t timeout_rx = 0;
char buff_rx_ch[buffrx1];
unsigned int i_rx = 0;
unsigned int j_rx = 0;
char flag_rx_start = 0;
extern GSM GSM1;
int k_rx = 0;  /*so bo dem can luu tru*/
/* Includes ------------------------------------------------------------------*/
/*******************************************************************************
* Function Name  : TIM_time2_Configuration
* Description    : TIM_Configuration program.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TIM_time3_Configuration(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
		TIM_DeInit(TIM3);
		TIM_TimeBaseStructure.TIM_Period=1; /*2000*/
																		
		TIM_TimeBaseStructure.TIM_Prescaler= (18000 - 1);				    
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM3, ENABLE);										
}
/*******************************************************************************
* Function Name  : NVIC_time1_Configuration
* Description    : Configuration the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void NVIC_time3_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure; 
		 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : usart1_putsf
* Description       : Configure USART1 
* Input               : con tro ma no tro toi
* Output             : truyen di mot chuoi ky tu
* Return             : co thanh cong hay khong -1 khong thanh khong,0 dang truyen,1 OK
* Attention	    : None
*******************************************************************************/
void usart1_putsf(char *st)
{	
	while(usart1.usart1_interrupt_flag_complete != COMPLETE)
	{
	}; /*cho den khi truyen xong chuoi truoc*/
	/*tro toi con tro truyen du lieu*/
	usart1.str_usart1 = st;
	/*cai dat ngat truyen USART*/
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	/*dat co trang thai truyen di bang NULL*/
	usart1.usart1_interrupt_flag_complete = NULL;
}
/*******************************************************************************
* Function Name  : usart1_putch
* Description    : Configure USART1 
* Input          : con tro ma no tro toi
* Output         : truyen di mot ky tu
* Return         : co thanh cong hay khong -1 khong thanh khong,0 dang truyen,1 OK
* Attention		 : None
*******************************************************************************/
void usart1_putch(char st)
{	
	/*cho den khi truyen xong chuoi truoc*/
	while(usart1.usart1_interrupt_flag_complete != COMPLETE)
	{
	};
	USART_SendData(USART1,st);
}
/*******************************************************************************
* Function Name  : USART1_usart1_Configuration
* Description    : Configure USART1 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void USART1_usart1_Configuration(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 

	RCC_APB2PeriphClockCmd( USARTx_GPIO_CLK | USARTx_CLK, ENABLE);
	/*
	*  USART1_TX -> PA9 , USART1_RX ->	PA10
	*/				
	GPIO_InitStructure.GPIO_Pin = USARTx_TxPin;	         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);		   
	
	GPIO_InitStructure.GPIO_Pin = USARTx_RxPin;	        
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	//  UTIL_SetIrqHandler(USART1_IRQHandler, (tHandler)USART1_Handler);  //pointing out the handler

	USART_Init(USARTx, &USART_InitStructure); 
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
	USART_Cmd(USARTx, ENABLE);
	usart1.usart1_interrupt_flag_complete = COMPLETE;

	/*cai dat TIM3 cho lay timeout USART1*/
	TIM_time3_Configuration();
	NVIC_time3_Configuration();
}
/*******************************************************************************
* Function Name  : NVIC_usart1_Configuration
* Description    : Configure NVIC USART1 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void NVIC_usart1_Configuration(void)
{
		//cai dat ngat NVIC
		NVIC_InitTypeDef NVIC_InitStructure;
      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
      NVIC_InitStructure.NVIC_IRQChannel                    = USARTx_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;
		NVIC_Init( &NVIC_InitStructure );	
}
/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM2 Handler.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{	
	   TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
	   if(flag_rx_start == 1) timeout_rx++;
	   if(timeout_rx > timeout_rx1)
	   {	/*Disable time3 va bo nhan du lieu usart*/
	   		TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
	   		USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);
			/*kiem tra luu bo dem neu co, xay ra hien tuong tran*/
			if(k_rx<buffrx0-1) k_rx++; else k_rx = 0;
			/*bat dau tu dau mang toi i_rx la nhan du lieu,chuoi ve sau bo trong*/
			for(j_rx = 0;j_rx < i_rx;j_rx++)
				{
				 	 GSM1.buff_rx[k_rx][j_rx] = buff_rx_ch[j_rx];
				}
			for(j_rx = i_rx;j_rx < buffrx1;j_rx++)
				{
				 	 GSM1.buff_rx[k_rx][j_rx] = 0;
				}
			/*Reset lai cac bo dem*/
			GSM1.nu_rx = i_rx;
			i_rx = 0;
			flag_rx_start = 0;
			timeout_rx = 0;
			GSM1.co_rx = k_rx;
			GSM1.flag_rx = 1;
			/*Enable time3 va bo nhan du lieu usart*/
			USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	   }
   }	
}
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : Handler USART1 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
  	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    /*xoa co ngat nhan USART va cho phep nhan ky tu moi*/
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		/*nhan tung ky tu*/
		buff_rx_ch[i_rx++] = USART_ReceiveData(USART1);
		/*kiem tra xem day bo nho chua, neu day thi giu~ nguyen i_rx bang toi da bo dem khai bao*/
		if(i_rx >= buffrx1) i_rx = buffrx1;
		/*bat co bao hieu bat dau dem time*/
		flag_rx_start = 1;
		/*Reset lai bo dem time3 de timeout ky tu ke tiep*/
		timeout_rx = 0;
  }
  
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
		USART_SendData(USART1, *(usart1.str_usart1++));
		USART_ClearFlag(USART1, USART_IT_TXE);
    if(*(usart1.str_usart1)==NULL)
    {
      /* Disable the USART1 Transmit interrupt */
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			usart1.usart1_interrupt_flag_complete = COMPLETE;
    }    
  }
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

