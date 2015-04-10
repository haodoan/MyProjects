#include "menu_lcd_nokia5110.h"
#include <stdio.h>
#include <string.h>
#include <rtc.h>
#define LCD_MAX_ROW 5
const char *mainmenu[] =  {" CHANGE PASS  ","1.Manager","2.Worker"};
const char *changepass_menu[] =  {"    MANAGER    ","1.Old Password","2.New Password","3.Retype Pass"};
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : DisplayMenuItem
* Returned Value   : void 
* Comments         :
*   This function display items of menu
*
*END*----------------------------------------------------------------------*/
void DisplayMenuItem(char *string[],uint8_t ItemNumber)
{
	uint8_t i;
	if(ItemNumber > LCD_MAX_ROW) ItemNumber = LCD_MAX_ROW;
	LCD_write_String(1,0,1,(char*)string[1]);		
	for(i = 0;i < ItemNumber; i++)
	{
		LCD_write_String(i,0,0,(char*)string[i]);		
	}
	//LCD_write_String(1,0,1,(char*)string[1]);		
	
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : UpdateDisplayMenu
* Returned Value   : void 
* Comments         :
*   This function updat items of menu when move cursor
*
*END*----------------------------------------------------------------------*/
void UpdateDisplayMenu(uint8_t count,char *string[])
{
	uint8_t i;
	for(i = 0;i < LCD_MAX_ROW - 1; i++)
	{
		LCD_write_String(i - count,0,0,(char*)string[i]);		
	}	
}
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : LcdTextCursor
* Returned Value   : void 
* Comments         :
*   This function display cursor of item
*
*END*----------------------------------------------------------------------*/
void LcdTextCursor(uint8_t rowcursor,uint8_t NumberItem,uint8_t count,char *string[])
{
	if(rowcursor < 6){
		DisplayMenuItem(string,NumberItem);
		LCD_write_String(1,0,0,(char*)string[1]);		
		LCD_write_String(rowcursor,0,1,string[rowcursor]);				
	}
	else
	{
 		UpdateDisplayMenu(count,string);
		LCD_write_String(LCD_MAX_ROW,0,1,(char*)string[rowcursor]);		
	}
}

void DisplayCursor(uint8_t cursor)
{
	char tmpbuff[5];
	sprintf(tmpbuff,"%d",cursor);
	LCD_write_String(0,13,0,tmpbuff);
}


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : SetupNumber
* Returned Value   : void 
* Comments         :
*   This function to setup number
*
*END*----------------------------------------------------------------------*/
//uint8_t SetupNumber(signed char *number,uint8_t row,uint8_t col)
//{
//}

void LCD_write_number(uint8_t row,uint8_t col,uint8_t invert,uint8_t number)
{
	char tmpbuff[2];
	sprintf(tmpbuff,"%d",number);
	LCD_write_String(row,col,invert,tmpbuff);
}

uint8_t set_time_current(uint8_t* time,uint8_t x,uint8_t y)
{
		char key;
		uint8_t time_H,time_L;
		time_H = *time/10;
		time_L = *time%10;
    LCD_write_number(x,y,1,time_H); 
		delay_ms(1000);
	  do{
				key = CheckButtonPress();
				if(key == 'M') return 0;
				if(key == 'S') return 1;
				if(key == 'P') break;
				if(key == 'B') return 3;
		}while(key == 'N');
		if(key != 'P')
		time_H = (key -0x30);
		LCD_write_number(x,y,0,time_H); 
    LCD_write_number(x,y+1,1,time_L); 
		delay_ms(1000);
	  do{
				key = CheckButtonPress();
				if(key == 'M') return 0;
				if(key == 'S') return 1;
				if(key == 'P') break;
				if(key == 'B') return 3;
		}while(key == 'N');
		if(key != 'P')
		time_L = key - 0x30;		
		LCD_write_number(x,y+1,0,time_L); 
		*time = time_H*10 + time_L;
		return 2;
}

DATE_STRUCT timedate;
BOOL SetTimeDate(void)
{
		//DATE_STRUCT timedate;
		//char tmpbuff[2];
		signed char hour_H ,hour_L ,min_H,min_L;
		signed char day_H,day_L,month_H,month_L,year_H,year_L;
		uint8_t result;
	
		timedate = GetTimeCurrent(RTC_GetCounter());
		hour_H = timedate.HOUR/10;
		hour_L = timedate.HOUR%10;
		min_H = timedate.MINUTE/10;
		min_L = timedate.MINUTE%10;
		day_H = timedate.DAY/10;
		day_L = timedate.DAY%10;
		month_H = timedate.MONTH/10;
		month_L = timedate.MONTH%10; 
		year_H = (timedate.YEAR%100)/10;
		year_L = (timedate.YEAR%100)%10;
		LCD_write_String(0,0,0,"SET TIME/DATE");
		LCD_write_String(2,0,0,"HH:MM");
	  LCD_write_number(3,4,1,hour_H);	
		LCD_write_number(3,5,0,hour_L);	
		LCD_write_String(3,6,0,":");
	  LCD_write_number(3,7,0,min_H);	
		LCD_write_number(3,8,0,min_L);	

		LCD_write_String(4,0,0,"DD/MM/YY");
	  LCD_write_number(5,4,0,day_H);	
		LCD_write_number(5,5,0,day_L);	
		LCD_write_String(5,6,0,"/");
	  LCD_write_number(5,7,0,month_H);	
		LCD_write_number(5,8,0,month_L);	
		LCD_write_String(5,9,0,"/");
	  LCD_write_number(5,10,0,year_H);	
		LCD_write_number(5,11,0,year_L);	
		
		timedate.YEAR = timedate.YEAR%100;
		back :
		result = set_time_current((uint8_t*)&timedate.HOUR,3,4);
		if(result == 0) return FALSE;
		if(result == 1) return TRUE;
	//	if(result == 3) goto back;
		back1:
		result = set_time_current((uint8_t*)&timedate.MINUTE,3,7);
		if(result == 0) return FALSE;
		if(result == 1) return TRUE;
	//	if(result == 3) goto back;
	 //timedate.SECOND = 0;
	 back2:
		result = set_time_current((uint8_t*)&timedate.DAY,5,4);;
		if(result == 0) return FALSE;
		if(result == 1) return TRUE;		
	//	if(result == 3) goto back1;
		back3:
		result = set_time_current((uint8_t*)&timedate.MONTH,5,7);
		if(result == 0) return FALSE;
		if(result == 1) return TRUE;		
		//if(result == 3) goto back3;
		back4:
		result = set_time_current((uint8_t*)&timedate.YEAR,5,10);				
		if(result == 0) return FALSE;
		if(result == 1) return TRUE;		
	//	if(result == 3) goto back3;
		//timedate.YEAR   = (uint16_t)2000 + timedate.YEAR;	
	//	goto back;
		return TRUE;
}
	
#define UP 			'0'
#define DOWN 		'1'
#define MENU    '8'
#define SET     '9'
signed char ProcessMenuItem_DT(MENU_ELEMENT_STRUCT *menu_element)
{
		char bt;
		static signed char count = 0;
		bt = CheckButtonPress();
 		if(bt == '0'){
			if(--(menu_element->currow_curr) <= 0)
				menu_element->currow_curr = menu_element->maxitem-1;
 			LcdTextCursor(menu_element->currow_curr,menu_element->maxitem,++count,menu_element->string);
			return NONE;
 		}	
 		else if(bt == '1'){
			if(++(menu_element->currow_curr) == menu_element->maxitem)
				menu_element->currow_curr = 1;
 			LcdTextCursor(menu_element->currow_curr,menu_element->maxitem,--count,menu_element->string);
			return NONE;
 		}			
		else if(bt == '9'){
			return ENTER;
		}
		else if(bt == '8'){			
			return BACK;
		}
//		else 
//			return NONE;
}


BOOL Change_pass(char *str,char* pass_enter,uint8_t *cnt_pass)
{
	uint8_t input;
	LCD_write_String(0,0,1,"   PASSWORD   ");
	LCD_write_String(2,0,0,str);
	do{
		input = Input_password(pass_enter,cnt_pass);
		//if(input == '8') return 0;
		if(input == 'M') return 0;
	}while(input != '9');	
	pass_enter[*cnt_pass - 1] = 0;
	return 1;
}
uint8_t Menu_Change_Pass(char *new_pwd,char *user)
{
	static uint8_t process = 0;
	uint8_t result;
	char tmp_pass_old[20],tmp_pass_new[20],tmp_pass_retype[20];
	static uint8_t cnt_pass[3];
	switch(process)
	{
		case 0 : 
			result = Change_pass("Old password",tmp_pass_old,&cnt_pass[0]);
			cnt_pass[0] = 0;
			if(!result) {LCD_clear();	process = 0;return 1;}//back;
			if(strcmp(tmp_pass_old,user) != 0) 
			{
				//cnt_pass[0] = 0;
				LCD_write_String(3,0,0,"Password wrong");
				delay_ms(2000);
				LCD_clear();		
				return 0;
			}				
			process = 1;
     	LCD_clear();		
		case 1 : 
			result = Change_pass("New password",tmp_pass_new,&cnt_pass[1]);
			cnt_pass[1] = 0;
			if(!result) {LCD_clear();	process = 0;return 1;}//back;
					process = 2;
     	LCD_clear();		
			delay_ms(300);
		case 2 :		
			result = Change_pass("Conf password",tmp_pass_retype,&cnt_pass[2]);
			cnt_pass[2] = 0;
			if(!result) {LCD_clear();process = 0;	return 1;}//back;
			if(strcmp(tmp_pass_new,tmp_pass_retype) != 0) 
			{
				LCD_write_String(3,0,0,"Password wrong");
				delay_ms(2000);
				LCD_clear();		
				return 0;
			}
			LCD_clear();	
			LCD_write_String(2,3,0,"Done");
			delay_ms(2000);
			process = 0;
			strcpy(new_pwd,tmp_pass_new);
     	LCD_clear();		
			cnt_pass[2] = 0;
			return 2;				
		
	}
}


BOOL ChangePass(char *manager_pwd,char *worker_pwd)
{
	uint8_t process;
	char key = NULL;
	static	MENU_ELEMENT_STRUCT menu_element;
	static uint8_t tmp= 1;
	char new_pwd[2][20];//mat khau moi
	char user_pass[20],login_pass[20];	
	uint8_t back ;
	if(tmp)
	{
			menu_element.string = (char**)mainmenu;
			menu_element.maxitem = 3;
			menu_element.currow_curr = 1;
			tmp = 0;
			DisplayMenuItem(menu_element.string,menu_element.maxitem);
	}
	key = CheckButtonPress();
	switch(key)
	{
			case '1' : 
				LCD_clear();
				do{
					back = Menu_Change_Pass(new_pwd[0],manager_pwd);
					if(back == 1) {tmp = 1;break;}
				}while(back != 2);// change password of manager
				if(back == 2){
					strcpy(manager_pwd,new_pwd[0]);
					WriteStrEeprom(0,20,manager_pwd);//manager 
					}
				tmp = 1;
				return TRUE;
			case '2' : 
				LCD_clear();
			  do{
					back = Menu_Change_Pass(new_pwd[1],worker_pwd);
					if(back == 1) {tmp = 1;break;}
				}while(back != 2); // change password of worker
				if(back == 2){
					strcpy(worker_pwd,new_pwd[1]);
					WriteStrEeprom(50,70,worker_pwd);//worker
			  }
				tmp = 1;;
				return TRUE;			
			case '8':tmp = 1;return TRUE;
				break;

	 }		
	 return FALSE;
}

BOOL Menu_Task(char *manager_pwd,char *worker_pwd)
{
	char key;
		LCD_write_String(0,0,1,"MENU SETTING");
		LCD_write_String(1,0,0,"1.Set time/date");
		LCD_write_String(2,0,0,"2.Change pass");
		LCD_write_String(3,0,0,"3.Default pass");
		key = CheckButtonPress();
		switch(key)
		{
			case '1':
				LCD_clear();
				if(SetTimeDate())
				{
						RTC_Configuration();
						timedate.YEAR   = (uint16_t)2000 + timedate.YEAR;	
						Time_Adjust(timedate);
						BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
						LCD_clear();	
				}
				else
					LCD_clear();
				break;
			case '2' :
			  LCD_clear();
				while(!ChangePass(manager_pwd,worker_pwd));
				break;
			case '3':
			{
				char user_pass[20],login_pass[20];
				char login;
				loop:
				LCD_clear();	
				LCD_write_String(2,1,0,"Security Code ");
				login = Login_pass("Security Code ",manager_pwd,login_pass);
				if(login==0)
				{
					LCD_clear();
					LCD_write_String(3,0,0,"Wrong password");
					delay_ms(2000);
					goto loop;
				}
				else if(login==4)
				{
						break;		
        }
				else
				{
					LCD_write_String(3,0,0,"Right password");
					delay_ms(2000);
					WriteStrEeprom(50,70,"123456");
					load_default_password(manager_pwd,worker_pwd);
					LCD_clear();
				}
				break;	
			}
			case '8' : return 1;
		}
		return 0;
	
}



