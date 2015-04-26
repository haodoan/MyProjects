#include "alarmtime.h"
#include "simcom.h"
#include "string.h"
#include "flasheeprom.h"
//Struct define for data received from UART

typedef struct {
  	char cmd[15];
	char data_str[2][20];
  	uint8_t data[50];
  	unsigned char countdata; 	
}STRUCTCOMMAND;

//Struct define for packet data 
typedef struct
{
	TIMESETUP alarmtime[10];
	DATE_STRUCT timecurr;
	uint8_t timeset[3];
	uint8_t dateset[3];
	uint16_t mode;
	uint8_t cntdata;
	TIMESETUP summer[2];
	TIMESETUP winter[2];	
}UART_PACKKET_STRUCT;

//TIMESETUP *GetSeason(uint8_t sea);
void GetTimeAlarm(TIMESETUP *timeonoff);
void cpy(TIMESETUP *a,TIMESETUP *b);
void GetSeason(TIMESETUP *timeonoff,uint8_t sea);
uint8_t GetMode(void);
uint8_t UARTCommand(UART_PACKKET_STRUCT *uart_packet, STRUCTCOMMAND result);
void CommadProcess(TIMESETUP timeonoffG[][10],UART_PACKKET_STRUCT uart_packet , uint8_t command);
STRUCTCOMMAND GetCmdData(char *str);

