#ifndef __REGISTER_H
#define __REGISTER_H

#include "simcom.h"
#include "alarmtime.h"

//commands register

/*
DK ADMIN MK
DMK
XDK MK
*/

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

STRUCTCOMMAND_SIM900 GetCmdDataSim900(char *str);
void SIM900_commands(char *);
char SearchExistUser(char *numberphone);
char Register_commands(STRUCTCOMMAND_SIM900 cmd_data);
#endif
