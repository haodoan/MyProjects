
#ifndef __CALENDAR_H
#define __CALENDAR_H
#include <stdio.h>
#include "stm32f10x.h"


typedef enum {FALSE = 0, TRUE = !FALSE}BOOL;
/*--------------------------------------------------------------------------*/
/*
**                            TIME CONSTANTS
*/
#define PICOSECS_IN_NANOSECOND   (1000)
#define NANOSECS_IN_MICROSECOND  (1000)
#define MICROSECS_IN_MILLISECOND (1000)
#define MILLISECS_IN_SECOND      (1000)
#define MICROSECS_IN_SECOND      (MILLISECS_IN_SECOND*MICROSECS_IN_MILLISECOND)
#define NANOSECS_IN_SECOND       (MICROSECS_IN_SECOND*NANOSECS_IN_MICROSECOND)
#define SECS_IN_MINUTE           (60UL)
#define MINUTES_IN_HOUR          (60UL)
#define HOURS_IN_DAY             (24UL)
#define MAX_DAYS_IN_MONTH        (31UL)
#define DAYS_IN_YEAR             (365UL)
#define MAX_DAYS_IN_YEAR         (366UL)
#define MONTHS_IN_YEAR           (12UL)
#define SECS_IN_HOUR             (SECS_IN_MINUTE*MINUTES_IN_HOUR)
#define SECS_IN_DAY              (SECS_IN_HOUR*HOURS_IN_DAY)
#define SECS_IN_YEAR             (SECS_IN_DAY*DAYS_IN_YEAR)
#define CLK_FIRST_YEAR           (1970UL)
#define XCLK_FIRST_YEAR          (1970)
#define CLK_LAST_YEAR            (2099UL)
#define XCLK_LAST_YEAR           (2481)
#define XCLK_ENTRIES             (512)
#define MAXIMUM_SECONDS_IN_TIME  (4102444800UL)
#define MAXIMUM_MILLISECONDS     (999UL)
#define MAXIMUM_DAYS_IN_XDATE    (187004UL)

typedef  uint32_t uint_32;
typedef  uint16_t uint_16;


typedef struct time_struct
{

   /* The number of seconds in the time.  */
   uint_32     SECONDS;

   /* The number of milliseconds in the time. */
   uint_32     MILLISECONDS;

} TIME_STRUCT, * TIME_STRUCT_PTR;


typedef struct date_struct
{
   /* 1970 - 2099 */
   uint16_t YEAR;

   /* 1 - 12 */
   uint16_t MONTH;

   /* 1 - 31 (depending on month) */
   uint16_t DAY;

   /* 0 - 23 */
   uint16_t HOUR;

   /* 0 - 59 */
   uint16_t MINUTE;

   /* 0 - 59 */
   uint16_t SECOND;

   /* 0 - 999 */
   uint16_t MILLISEC;

} DATE_STRUCT, * DATE_STRUCT_PTR;

uint_32 _is_leap_year( uint_16 year );
BOOL _time_to_date( TIME_STRUCT_PTR  ts_ptr, DATE_STRUCT_PTR  time_ptr );
uint_32 _date_to_sec( DATE_STRUCT_PTR time );
#endif
