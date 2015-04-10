/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.lhd.vn
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               define.h
** Descriptions:            define 
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Haodt
** Created date:            2012-07-21
** Version:                 
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/
#ifndef __DEFINE_H
#define __DEFINE_H 


#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include <systick.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//USART
#define BUFF_RX_USART      300 // bo dem, can bao nhieu bo dem de cho
#define TIMEOUT_RX_USART_1 50 // 115200
#define TIMEOUT_RX_USART_2 150 // 9600

//RELAY CONTROL
//#define RELAY1(x) x?GPIO_SetBits(GPIOA, GPIO_Pin_13):GPIO_ResetBits(GPIOA , GPIO_Pin_13)  // len
//#define RELAY2(x) x?GPIO_SetBits(GPIOA , GPIO_Pin_14):GPIO_ResetBits(GPIOA , GPIO_Pin_14) // xuong
//#define RELAY3(x) x?GPIO_SetBits(GPIOA , GPIO_Pin_15):GPIO_ResetBits(GPIOA , GPIO_Pin_15) // Dung

#define RELAY1(x)   x? GPIO_SetBits(GPIOB , GPIO_Pin_0|GPIO_Pin_8) :GPIO_ResetBits(GPIOB , GPIO_Pin_0|GPIO_Pin_8)
#define RELAY2(x)   x? GPIO_SetBits(GPIOB , GPIO_Pin_1|GPIO_Pin_10) :GPIO_ResetBits(GPIOB , GPIO_Pin_1|GPIO_Pin_10)

// DEFINE USART1
#define USARTx                   USART1
#define USARTx_GPIO              GPIOA
#define USARTx_CLK               RCC_APB2Periph_USART1
#define USARTx_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USARTx_RxPin             GPIO_Pin_10
#define USARTx_TxPin             GPIO_Pin_9
#define USARTx_Tx_DMA_Channel    DMA1_Channel4
#define USARTx_Tx_DMA_FLAG       DMA1_FLAG_TC4
#define USARTx_DR_Base           0x40013804
#define USARTx_IRQn              USART1_IRQn
#define USARTx_IRQHandler        USART1_IRQHandler

// DEFINE USART2
#define USARTy                   USART2
#define USARTy_GPIO              GPIOA
#define USARTy_CLK               RCC_APB1Periph_USART2
#define USARTy_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USARTy_RxPin             GPIO_Pin_3
#define USARTy_TxPin             GPIO_Pin_2
#define USARTy_IRQn              USART2_IRQn
#define USARTy_IRQHandler        USART2_IRQHandler

enum
{
	FAIL = 0,
	COMPLETE
};
// DINH NGHIA CAC KIEU DU LIEU
typedef struct _type_door
{
	uint16_t type;
} _TYPE_DOOR;

typedef struct usart_it
{
	// Tx
	volatile char *str_Tx_Usart;
	volatile char flag_Tx_Usart_Complete;
	// Rx
	char buff_Rx[BUFF_RX_USART];
  char buff_Rx_Update[BUFF_RX_USART];
	volatile char flag_Rx_Usart_Start;		 
	volatile unsigned int number_rx;	
	volatile unsigned int timeout_Rx;
	volatile char flag_Rx_Usart_Complete;
} USART_IT;

// KHAI BAO BIEN
extern USART_IT 	usart_1;

#if defined (USE_USART2)
extern USART_IT 	usart_2;
#endif

// SETTING PIN
void ConfigOutput(void);
void ConfigInput(void);
void ConfigUSART1(void);
void ConfigUSART2(void);

void USART1_IRQHandler(void);
void TIM3_IRQHandler(void);
void usart_1_putsf(char *st);
void usart_1_putch(char st);

void TIM7_IRQHandler(void);
void USART2_IRQHandler(void);
void usart_2_putsf(char *st);
void usart_2_putch(char st);

void TIM_time2_Configuration(void);
void TIM2_IRQHandler(void);

void EXTI_Configuration_A0(void);
void EXTI0_IRQHandler(void) ;
void USART1_usart1_Configuration(void);
void NVIC_usart1_Configuration(void);
void usart1_putch(char st);

//app
uint8_t UART_Frame_Receive(void);
#endif  /* __PLATFORM_CONFIG_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
