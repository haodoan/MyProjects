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
#ifndef __SIMCOM_H
#define __SIMCOM_H
#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <systick.h>
#include <comm.h>
/*
 *   *************DEFINE****************************
 */
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
#define nPhone      15	/*toi da so dien thoai duoc luu*/
#define cTextSms    160	/*Noi dung tin nhan toi da*/
/* luu tru du lieu bo dem nhan ve*/
#define buffrx1 270 /*moi~ 1 bo dem can bao nhieu ky tu*/
#define buffrx0 1 /* bo dem, can bao nhieu bo dem de cho */ 
#define buffrx3 270 /*moi~ 1 bo dem can bao nhieu ky tu*/
#define buffrx2 1 /* bo dem, can bao nhieu bo dem de cho */ 
#define timeout_rx1 250
#define timeout_rx3 100
#define InputDefaut               GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0)
#define InputGsmRi                GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_6)
#define InputSwitchOff            GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10)
#define Alarm(x)    x? GPIO_SetBits(GPIOA , GPIO_Pin_11):GPIO_ResetBits(GPIOA , GPIO_Pin_11)
#define Nreset(x)   x? GPIO_SetBits(GPIOA , GPIO_Pin_8 ):GPIO_ResetBits(GPIOA , GPIO_Pin_8)
#define Buzz(x)     x? GPIO_SetBits(GPIOB , GPIO_Pin_9|GPIO_Pin_10) :GPIO_ResetBits(GPIOB , GPIO_Pin_9)
#define PwrGas(x)   x? GPIO_SetBits(GPIOB , GPIO_Pin_8) :GPIO_ResetBits(GPIOB , GPIO_Pin_8)
#define GsmDtr(x)   x? GPIO_SetBits(GPIOA , GPIO_Pin_5) :GPIO_ResetBits(GPIOA , GPIO_Pin_5)

typedef struct gsm
{
	char buff_rx[buffrx0][buffrx1];	  // bo dem du lieu
	char flag_rx;		 //co bao hieu co' du~ lieu truyen toi
	unsigned int co_rx;	 // So thu tu bo trong bo dem, tuc la bo co 10 bo dem
	unsigned int nu_rx;	 // so ky tu nhan ve bo dem
} GSM;
typedef struct time_sms
{
   char seconds[2];
   char minutes[2];
   char hours[2];
   char days[2];
   char months[2];
   char years[4];
} TIME_SMS;
typedef struct sms_gsm
{
   char numberPhone[nPhone];			//so dien thoai
   char contentTextSms[cTextSms]; //noi dung tin nhan
   char sttSms[3];								//so thu tin nhan
   TIME_SMS textSmsTime;  				//thoi gian cua tin nhan
} SMS_GSM;
typedef struct call_gsm
{
   char numberPhone[nPhone];			//so dien thoai
} CALL_GSM;
/*
 *   ****END******DEFINE****************************
 */
typedef struct vbat
{
   unsigned char level;	// muc pin con lai
   unsigned int mV;
} VBAT;


void Pwrkey(char key);
void Start_Gsm(void);
void __delay_ms(u16 dl);
void GSM_putsf(char *string);
char TestConnecSim(void);
char GetEchoLong(char *string);
char GetEcho(char *string);
char GetEchoTwo(char *string, char *string2);
void NameNetWork(void);
char Init_GSM(void);
void Start_SIMCOM(void);
char ReadEnglis_SIMmsg(unsigned char msgnum);
char *GetPointText(char *textsms,char checkText,char number);
char decode_sms(char *textSms,SMS_GSM *sms1);
char decode_call(char *textSms,CALL_GSM *call);
char SentEnglis_SIMmsg(char *phonenum,char *msgstr);
char WaitSendSms(void);
char WaitSendSMS(void);
char CallPhone(char *phonenum);
char DisconnectCall(void);
char HangUpCall(void);
char CallUpCall(void);
char CCLK(void);
char SetingCCLK(char *years,char *months,char *days,char *hours,char *minutes,char *seconds,char *timezone);
char DellSms(u16 index);
char DellAllSms(u16 total);
char DelAllSmsCmgda(void);
char CheckUsd(void);
unsigned int CheckCsq(void);
char Naptien(char *mathe);

VBAT CheckCbc(void);
char ResetDefaultConfig_ATZ(void);
char DeactivateGprsPdpContext_CIPSHUT(void);
char AttachDetachFromGprsService_CGATT(void);
#endif /* __PLATFORM_CONFIG_H */
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
