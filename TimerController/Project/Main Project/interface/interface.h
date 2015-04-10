#include "alarmtime.h"
#include "simcom.h"
//Struct define for data received from UART
typedef struct {
  	char cmd[15];
		char data_str[2][20];
  	unsigned char data[50];
  	unsigned char countdata; 	
}STRUCTCOMMAND;

//Struct define for packet data 
typedef struct
{
	TIMESETUP alarmtime[10];
	DATE_STRUCT timecurr;
	uint8_t *timeset;
	uint8_t *dateset;
	uint16_t mode;
	uint8_t cntdata;
	TIMESETUP summer[2];
	TIMESETUP winter[2];	
}UART_PACKKET_STRUCT;

//TIMESETUP *GetSeason(uint8_t sea);
void GetTimeAlarm(TIMESETUP *timeonoff);
void cpy(TIMESETUP *a,TIMESETUP *b);
void CommadProcess(TIMESETUP timeonoffG[][10],char *buffer);
void GetSeason(TIMESETUP *timeonoff,uint8_t sea);
uint8_t GetMode(void);
