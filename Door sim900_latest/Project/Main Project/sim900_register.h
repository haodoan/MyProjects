#ifndef __REGISTER_H
#define __REGISTER_H

#include "simcom.h"
//commands register

/*
DK ADMIN MK
DMK
XDK MK
*/
//RELAY CONTROL
#define LED1(x) x?GPIO_SetBits(GPIOA , GPIO_Pin_11):GPIO_ResetBits(GPIOA , GPIO_Pin_11)
#define LED2(x) x?GPIO_SetBits(GPIOA , GPIO_Pin_12):GPIO_ResetBits(GPIOA , GPIO_Pin_12)
#define LED3(x) x?GPIO_SetBits(GPIOB , GPIO_Pin_0):GPIO_ResetBits(GPIOB , GPIO_Pin_0)
#define LED4(x) x?GPIO_SetBits(GPIOB , GPIO_Pin_1):GPIO_ResetBits(GPIOB , GPIO_Pin_1)

#define RELAY1(x) x?GPIO_SetBits(GPIOB , GPIO_Pin_8):GPIO_ResetBits(GPIOB , GPIO_Pin_8)
#define RELAY2(x) x?GPIO_SetBits(GPIOB , GPIO_Pin_10):GPIO_ResetBits(GPIOB , GPIO_Pin_10)
#define RELAY3(x) x?GPIO_SetBits(GPIOB , GPIO_Pin_11):GPIO_ResetBits(GPIOB , GPIO_Pin_11)

//DOOR CONTROL
#define UP_DOOR_ON()           RELAY1(1)
#define STOP_DOOR_ON()         RELAY2(1)
#define DOWN_DOOR_ON()         RELAY3(1)

#define UP_DOOR_OFF()          RELAY1(0)
#define STOP_DOOR_OFF()        RELAY2(0)
#define DOWN_DOOR_OFF()        RELAY3(0)

//Struct define for data received from UART
typedef struct {
  	char CMD[10];
		char DATA[6][20];
  	unsigned char CNT_DATA; 	
}STRUCTCOMMAND_SIM900;

typedef struct {
	char PASSWORD[10];
	char PHONE_NO[12];	
	char ADMINI;
}STRUCT_USER;

enum
{
	UP = 0,
	DOWN,
	STOP
};

enum
{
	CLOSE = 0,
	OPEN,
	CLOSSING,
	OPENNING,
	STOPED,
	NONE
};
#define DOORTIME	60000

STRUCTCOMMAND_SIM900 GetCmdDataSim900(char *str);
void SIM900_commands(char *);
char SearchExistUser(char *numberphone);
char User_commands(STRUCTCOMMAND_SIM900 cmd_data);
void DoorControl(char Control);
#endif
