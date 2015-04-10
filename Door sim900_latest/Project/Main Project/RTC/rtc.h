
#ifndef __RTC_H
#define __RTC_H

#include<stdio.h>
#include "stm32f10x.h"
#include <calendar.h>
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include <alarmtime.h>



void RTC_Configuration(void);
extern void Time_Adjust(DATE_STRUCT TimeSet);
void Time_Show(TIMESETUP timeonoff[][10]);
uint8_t USART_Scanf(uint32_t value);
extern void RTCruntime(void);
extern void RTC_Init(void);
uint32_t Time_Regulate(DATE_STRUCT TimeSet);
extern DATE_STRUCT GetTimeCurrent(uint32_t TimeVar);
#endif
