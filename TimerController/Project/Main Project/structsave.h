#ifndef __STRUCT_SAVE_H
#define __STRUCT_SAVE_H

typedef struct
{
	STRUCT_USER user[6];
	TIMESETUP alarmtime[10];
	TIMESETUP alarmtime1[10];
	TIMESETUP summer[2];
	TIMESETUP winter[2];		
	uint16_t cnt_user;
	uint16_t mode;
}STRUCT_EEPROM_SAVE;
#endif