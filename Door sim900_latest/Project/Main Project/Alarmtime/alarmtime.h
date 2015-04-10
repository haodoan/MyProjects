/**/
#ifndef __ALARM_TIME_H
#define __ALARM_TIME_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stm32f10x.h>

typedef struct{
	uint16_t hrmin;
	uint8_t hr;
	uint8_t min;
	uint16_t status;
} TIMESETUP;


#include <calendar.h>
#include <rtc.h>
#include "stm32_eval.h"
#include "lcd_nokia5110.h"
enum
{
	SUMMER = 0,
	WINTER
};


char init_time(TIMESETUP timeonoff[][10],unsigned char season);
void sapxeptime(TIMESETUP *time,int n);
unsigned char CheckKhoang(TIMESETUP *time,DATE_STRUCT timecurrent,int n);
unsigned char GetCommand(unsigned char *command);
void nhaptimeonoff(TIMESETUP *timeonoff,int batnumber,int tatnumber);
void OnoffOutput(TIMESETUP timeonoff[][10]);
void ProcessTimeTask(unsigned char FrameAvaiable,char *str,TIMESETUP *timeonoff);

#define SUMMER_HR_ON		   	18
#define SUMMER_MIN_ON				30
#define SUMMER_HR_OFF				23
#define SUMMER_MIN_OFF	   	0

#define WINTER_HR_ON		   	17
#define WINTER_MIN_ON				15
#define WINTER_HR_OFF				23
#define WINTER_MIN_OFF	   	0

#endif
