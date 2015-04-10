#ifndef __NOKIA_LCD5110_H
#define __NOKIA_LCD5110_H

/* Includes ------------------------------------------------------------------*/	   
#include "stm32f10x.h"
//#include "stm32_eval.h"
//#include "stm32f10x_systick.h"
//#include "systick.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define sce0   GPIO_ResetBits(GPIOB, GPIO_Pin_0)   //CE	
#define res0   GPIO_ResetBits(GPIOB, GPIO_Pin_1)   //Reset
#define dc0    GPIO_ResetBits(GPIOA, GPIO_Pin_7)   //DC
#define sdin0  GPIO_ResetBits(GPIOA, GPIO_Pin_6)   //SDI
#define sclk0  GPIO_ResetBits(GPIOA, GPIO_Pin_5)   //SCK
#define backled0  GPIO_ResetBits(GPIOA, GPIO_Pin_4)   //BL

#define sce1   GPIO_SetBits(GPIOB, GPIO_Pin_0)  //CE
#define res1   GPIO_SetBits(GPIOB, GPIO_Pin_1)  //RST
#define dc1    GPIO_SetBits(GPIOA, GPIO_Pin_7)   //DC
#define sdin1  GPIO_SetBits(GPIOA, GPIO_Pin_6)   //SDI
#define sclk1  GPIO_SetBits(GPIOA, GPIO_Pin_5)   //SCK
#define backled1  GPIO_SetBits(GPIOA, GPIO_Pin_4)   //BL


void LCD_write_byte(unsigned char dt, unsigned char command);
void LCD_init(void);
void LCD_set_XY(unsigned char X, unsigned char Y);
void LCD_clear(void);
void LCD_cursor(unsigned char row);
void LCD_write_char(unsigned char c,unsigned char invert);
void LCD_write_String(unsigned char X,unsigned char Y,unsigned char invert,char *s);
void GPIO_ConfigurationNOKIA_5110(void);
void LCD_write_String_HD(unsigned char X,unsigned char Y,unsigned char invert,char *s);
void LCD_set_XY_HD(unsigned char X, unsigned char Y);


#endif
