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
#ifndef __DEFINE_H
#define __DEFINE_H
typedef struct simcomstruct
{
		char nameTheNetwork[16]; // 
		unsigned char percentCapacityPin; // phan tram cua PIN
		unsigned int capacityPin; // dien ap cua PIN
		unsigned int mobileWaveIntensity; // Cuong do song di dong
	  char seconds; //giay
    char minutes; //phut
    char hours;   //gio
    char days;    //ngay
    char months;  //thang
    unsigned int years;   //nam
} SIMCOMSTRUCT;
//SIMCOMSTRUCT simComInfo;
#define tempMaxFire 450 //tuc la 40,0 do C
#define gasMax1     800
#define gasMax2     2000
#define gasMax3     2800
#define InputDefaut               GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0)
#define InputGsmRi                GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_6)
#define InputLostPowerExternal    GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_1)
#define InputChargingStatus       GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_5)
#define InputSwitchOff            GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10)
#define Alarm(x)    x? GPIO_SetBits(GPIOA , GPIO_Pin_11):GPIO_ResetBits(GPIOA , GPIO_Pin_11)
#define Nreset(x)   x? GPIO_SetBits(GPIOA , GPIO_Pin_8 ):GPIO_ResetBits(GPIOA , GPIO_Pin_8)
#define Buzz(x)     x? GPIO_SetBits(GPIOB , GPIO_Pin_9|GPIO_Pin_10) :GPIO_ResetBits(GPIOB , GPIO_Pin_9)
#define PwrGas(x)   x? GPIO_SetBits(GPIOB , GPIO_Pin_8) :GPIO_ResetBits(GPIOB , GPIO_Pin_8)
#define GsmDtr(x)   x? GPIO_SetBits(GPIOA , GPIO_Pin_5) :GPIO_ResetBits(GPIOA , GPIO_Pin_5)
//#define checkTimeGSM         15000
#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
