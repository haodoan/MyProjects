/**
  ******************************************************************************
  * @file    TIM/PWM_Output/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32_eval.h"
#include <stdio.h>
#include "comm.h"
#include <simcom.h>
#include "sim900_register.h"
#include "flasheeprom.h"
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
EXTI_InitTypeDef   EXTI_InitStructure;
GPIO_InitTypeDef   GPIO_InitStructure;
NVIC_InitTypeDef   NVIC_InitStructure;
void GPIO_Configuration(void);
void EXTI9_5_Config(void);
char Test_echoUART(char *read_uart);
char New_SMS(char *SMS);
char Reply_SMS(char *SMS);
char CallOnOff(char *ID);
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

USART_IT 	usart_1;
extern volatile uint32_t sysTick_counter;
extern GSM GSM1;
SMS_GSM decodeSMS;
extern char function_eeprom;
STRUCT_EEPROM_SAVE flashv;
char status_tb[2]={0,0};
u32 temp_=0;
u32 timedoor_check = 0;
char status_door = CLOSE;
char prev_status_door = NONE;
u32 timedoor = 0;
int main(void)
{  
  GSM1.flag_rx = 0;
	sysTick_counter = 0;

  /* System Clocks Configuration */
	if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
  USART1_usart1_Configuration();
	NVIC_usart1_Configuration();
	GPIO_Configuration();
   if(InputGsmRi == 0) 
	LED4(1);
		Start_Gsm();  // Kiem tra chan Ri
	DelAllSmsCmgda();
	ReadMemmory(&flashv);
	if(flashv.cnt_user == 0xFFFF) 
		flashv.cnt_user = 0;
	delay_ms(1000);
	SentEnglis_SIMmsg("0944500186","Thiet bi vua khoi dong\r\n");
	//CallPhone(flashv.user[0].PHONE_NO);
	while(1)
	{      
		if(GSM1.flag_rx)
		{         
			 GSM1.flag_rx=0;
			 Test_echoUART(GSM1.buff_rx[GSM1.co_rx]);
			 SaveThayDoi(&flashv,function_eeprom);
		}
	}
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : GPIO_Configuration
* Returned Value   : void 
* Comments         :
*   This function config pin LCD 
*
*END*----------------------------------------------------------------------*/
void GPIO_Configuration(void)
{  
  	  
		GPIO_InitTypeDef GPIO_InitStructure; 				 	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 			//GPIOB clock
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;  	//ÉèÖÃGPIOB0¡¢GPIOB1¡¢GPIOB12
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				  	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				  	
  	GPIO_Init(GPIOA, &GPIO_InitStructure);							  	 	
	
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 			//GPIOB clock
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9;  	//ÉèÖÃGPIOB0¡¢GPIOB1¡¢GPIOB12
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				  	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				  	
  	GPIO_Init(GPIOB, &GPIO_InitStructure);							  	 	
}


/*******************************************************************************
* Function Name  : Test_echoUART
* Description    : Kiem tra du lieu tra ve xem la thong tin gi
* Input          : dong thong tin
* Output         : None
* Return         : 1:cac buoc xu ly
* Attention		 : None
*******************************************************************************/
char AccountInfo[160];
char tkkm = 0;
char Test_echoUART(char *read_uart)
{
//	char stt_sms = 0;
	if(strstr(read_uart,"+CMTI:"))
	{
				unsigned char numberSmsStt = 0;
				char *GetPointTextPhay;
				GetPointTextPhay = GetPointText(read_uart,',',1);
				if(isdigit(*(GetPointTextPhay+1))) numberSmsStt = *(GetPointTextPhay+1)-'0'; 
				if(isdigit(*(GetPointTextPhay+2))) numberSmsStt = numberSmsStt*10 + ((*(GetPointTextPhay+2))-'0');
				if(ReadEnglis_SIMmsg(numberSmsStt)==1)
				{  
						Reply_SMS(read_uart);
                  if(numberSmsStt > 20)
                     DelAllSmsCmgda();
				}
				return 2;	
	}	
	else if(strstr(read_uart,"+CUSD:"))
	{
      char *searchtk;
      searchtk = strstr(read_uart,"=");
      if(!tkkm){
         if(searchtk != NULL)
            sprintf(AccountInfo,"Tai khoan cua thiet bi : %s\r",searchtk+1);
         else{
            searchtk = strstr(read_uart,"la");
            if(searchtk != NULL){
               sprintf(AccountInfo,"Tai khoan cua thiet bi :%s", strtok (searchtk+2,"\","));
            }  
            else
               sprintf(AccountInfo,"Khong the kiem tra duoc tai khoan thiet bi\r");
         }
      }
      SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,AccountInfo);
      memset(AccountInfo,160,0);
      return 3;
	}	  
	if(strstr(read_uart,"+CLIP:"))
	{
      CallOnOff(read_uart);
      return 1;	
	}   
	return ERROR;
}

/*******************************************************************************
* Function Name  : Reply_CALL
* Description    : tra loi khi co cuoc goi den
* Input          : dong thong tin
* Output         : None
* Return         : 1:cac buoc xu ly
* Attention		 : None
*******************************************************************************/
//close ->open	-> dung cua(60s) ->open
//			 	-> close
//open -> close -> dung cua(60s) ->close
//				-> open
char CallOnOff(char *ID)
{
	CALL_GSM decodeCALL;
	char db_vt;
   static char mess_report = 0;
//	static char status_door = CLOSE,prev_status_door = NONE;
//	static u32 timedoor;
	decode_call(ID,&decodeCALL);
	//check user on contact
	db_vt = SearchExistUser(decodeCALL.numberPhone);
	if(db_vt != 10)
	{       
      if(mess_report > 60)
      {
         mess_report = 0;
         SentEnglis_SIMmsg("0944500186","Thiet bi hoat dong binh thuong\n");

      }
		HangUpCall();       //huy cuoc goi
		if(prev_status_door == CLOSE)
		{
			if(sysTick_counter > timedoor + DOORTIME )
			{
				status_door  = OPEN;
			}
		}
		else if(prev_status_door == OPEN)
		{
			if(sysTick_counter > timedoor + DOORTIME )
			{
				status_door  = CLOSE;
			}
		}	
		else if(prev_status_door == CLOSSING)  
		{
			status_door = CLOSE;
		}		
		else if(prev_status_door == OPENNING )  
		{
			status_door = OPEN;
		}			
		switch(status_door)
		{
			case CLOSE:
					//dk cua mo;
				DoorControl(STOP);
				//delay_ms(200);	
				DoorControl(UP);
				status_door  		= OPENNING;
				timedoor 			= sysTick_counter;
				prev_status_door	= CLOSE;
				break;			
			case OPEN:
				//dk cua dong;
				DoorControl(STOP);
				//delay_ms();	
				DoorControl(DOWN);
				status_door  		= CLOSSING;
				timedoor 			= sysTick_counter;
				timedoor_check 		= sysTick_counter;
				prev_status_door 	= OPEN;
				break;		
			case CLOSSING :
				prev_status_door	= CLOSSING;
				status_door  		= STOPED;	
				DoorControl(STOP);
				// dk cua dung
				break;
			case OPENNING :
				status_door  		= STOPED;	
				prev_status_door 	= OPENNING;
				DoorControl(STOP);
				//dk cua dung
				break;			
			case  STOPED :	
				break;
		}	
	}
	return 1;	
}

/*******************************************************************************
* Function Name  : Reply_CALL
* Description    : tra loi khi co cuoc goi den
* Input          : dong thong tin
* Output         : None
* Return         : 1:cac buoc xu ly
* Attention		 : None
*******************************************************************************/
//close ->open	-> dung cua(60s) ->open
//			 	-> close
//open -> close -> dung cua(60s) ->close
//				-> open
/*
char CallOnOff(char *ID)
{
	CALL_GSM decodeCALL;
	char db_vt;
//	static char status_door = CLOSE,prev_status_door = NONE;
//	static u32 timedoor;
	decode_call(ID,&decodeCALL);
	//check user on contact
	db_vt = SearchExistUser(decodeCALL.numberPhone);
	if(db_vt != 10)
	{       
		HangUpCall();       //huy cuoc goi
		if(prev_status_door == CLOSE)
		{
			if(sysTick_counter > timedoor + DOORTIME )
			{
				status_door  = OPEN;
			}
		}
		else if(prev_status_door == OPEN)
		{
			if(sysTick_counter > timedoor + DOORTIME )
			{
				status_door  = CLOSE;
			}
		}	
		else if(prev_status_door == CLOSSING)  
		{
			status_door = CLOSE;
		}		
		else if(prev_status_door == OPENNING )  
		{
			status_door = OPEN;
		}			
		switch(status_door)
		{
			case CLOSE:
					//dk cua mo;
				DoorControl(UP);
				status_door  		= OPENNING;
				timedoor 			= sysTick_counter;
				prev_status_door	= CLOSE;
				break;			
			case OPEN:
				//dk cua dong;
				DoorControl(DOWN);
				status_door  		= CLOSSING;
				timedoor 			= sysTick_counter;
				timedoor_check 		= sysTick_counter;
				prev_status_door 	= OPEN;
				break;		
			case CLOSSING :
				prev_status_door	= CLOSSING;
				status_door  		= STOPED;	
				DoorControl(STOP);
				// dk cua dung
				break;
			case OPENNING :
				status_door  		= STOPED;	
				prev_status_door 	= OPENNING;
				DoorControl(STOP);
				//dk cua dung
				break;			
			case  STOPED :	
				break;
		}	
	}
	return 1;	
}
*/
/*******************************************************************************
* Function Name  : New_SMS
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention		 : None
ma lenh
Dang ky them SDT : Dk."matkhau"."sothutu"."SDT"
Doi mat khau     : Mkc."matkhaucu".mkm."matkhaumoi"
Huy so dt dang ky: Hdk."so thu tu muon huy"
Xem
*******************************************************************************/
char New_SMS(char *SMS)
{
	char *point;
	int i = 0, j = 0;
	point = SMS;
	i = strlen(point);
	while( j < i )
	{
			*(point + j) = toupper(*(point + j));
			j++;
	}
	if(SMS[0]==0) 
	{	
		return ERROR;
	}
	
	/*Kiem tra giai ma tin nhan*/
	if(!decode_sms(SMS,&decodeSMS))
	{	
		return ERROR;
	}
	if(flashv.cnt_user > 0)
	{
		if(SearchExistUser(decodeSMS.numberPhone) != 10)
				SIM900_commands(decodeSMS.contentTextSms);			
		else if(strstr(decodeSMS.contentTextSms,"TADMIN") ||strstr(decodeSMS.contentTextSms,"XMK9687"))
				SIM900_commands(decodeSMS.contentTextSms);
	}
	else if(flashv.cnt_user == 0){
		SIM900_commands(decodeSMS.contentTextSms);	
	}
	return 1;
 //  CommadProcess(timeonoff,decodeSMS.contentTextSms);
}
/*******************************************************************************
* Function Name  : Reply_SMS
* Description    : Tra loi khi co tin nhan moi
* Input          : dong thong tin
* Output         : None
* Return         : 1:cac buoc xu ly
* Attention		 : None
*******************************************************************************/
char Reply_SMS(char *SMS)
{
	New_SMS(SMS);
	return 1;
}

void EXTI9_5_Config(void)
{
  /* Enable GPIOB clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* Configure PB.09 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  /* Connect EXTI9 Line to PB.09 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);

  /* Configure EXTI9 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI9_5 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}

/*
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}
	// LCD_write_char((unsigned char)ch,0);

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while (1)
  {}
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
