#ifndef __MENU_LCD_NOKIA5110_H
#define __MENU_LCD_NOKIA5110_H

#include "nokia_lcd5110.h"
#include <gpio_button.h>
#include <nokia_lcd5110.h>
#include <eeprom.h>
#include <Login.h>
#include <rtc.h>
typedef	 struct{
	char **string;
	uint8_t maxitem;
	signed char currow_curr;
}MENU_ELEMENT_STRUCT;


typedef struct
{
	char *pass_old;
	char *pass_new;
	char *pass_retype;
}CHANGE_PASS_STRUCT;

enum{
	NONE = 0,
	BACK,
	ENTER
};

void UpdateDisplayMenu(uint8_t count,char *string[]);
void DisplayMenuItem(char *string[],uint8_t ItemNumber);
void ProcessMenuItem(char *string[],uint8_t maxitem);
void LcdTextCursor(uint8_t rowcursor,uint8_t NumberItem,uint8_t count,char *string[]);
//BOOL MenuButton(char *manager_pwd,char *worker_pwd);
BOOL ChangePass(char *manager_pwd,char *worker_pwd);
BOOL Menu_Task(char *manager_pwd,char *worker_pwd);
#endif

