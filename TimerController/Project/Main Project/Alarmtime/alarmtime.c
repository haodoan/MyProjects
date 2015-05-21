//#include "interface.h"
#include "alarmtime.h"
#include "flasheeprom.h"
#include "interface.h"
extern STRUCT_EEPROM_SAVE flashv;
extern char status_tb[2];
/*******************************************************************************
* Function Name  : sapxeptime
* Description    : Sap xep cac thoi gian bat tat theo truc thoi gian 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void sapxeptime(TIMESETUP *time,int n)
{
	uint16_t i,j,temp;
    BOOL tempstatus ;
	for (i = 0; i < n; i++){
		time[i].hrmin = 100*time[i].hr + time[i].min;		
	}
	time[n].hrmin     =  0;
	time[n].status    =  FALSE;	
	time[n+1].hrmin   =  2400;
	time[n+1].status  =  FALSE;	
	n = n + 2;
   /*Arrange time from smallest to bigest*/
	for (i = 0; i < n - 1; i++){
		for (j = i + 1; j < n; j++)
		{
			if (time[i].hrmin > time[j].hrmin)
			{
				temp = time[i].hrmin ;
				time[i].hrmin  = time[j].hrmin ;
				time[j].hrmin  = temp;
			
				tempstatus = time[i].status;
				time[i].status = time[j].status;
				time[j].status = tempstatus;
			}
		}
	}
/*Get Hr and Min*/
	for (i = 0; i < n; i++){
      time[i].hr = time[i].hrmin/100;
      time[i].min = time[i].hrmin%100;
	}
	if(time[n-2].status == TRUE) time[0].status = TRUE;
}
/*******************************************************************************
* Function Name  : CheckKhoang
* Description    : Kiem tra thoi gian hien tai nam trong khoang nao
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
unsigned char CheckKhoang(TIMESETUP *time,DATE_STRUCT timecurrent,int n)
{
	int i;
	int number;
	n = n + 2;
	if(n >9) n= 9;
	number = 100*timecurrent.HOUR + timecurrent.MINUTE;
	for(i= 0;i<n;i++)
	{
		if((number>= time[i].hrmin)&&(number<time[i+1].hrmin))
			return i;	
	}
	return 50;
}
//void PrintfData(unsigned char *GetDatastr,unsigned char n)
//{	
//   char buffer[100];
//   static char i = 0;
//	  while(n--)
//	  {
//	  	sprintf(buffer,"%d",*GetDatastr++);
//	  	printf(buffer);
//			if(i++ == 4)
//			{
//				printf(";");
//				i = 1;
//			}
//			else  printf(",");
//	  }
//		 i = 0;
//			printf(";");		
//}
 char number_alarm[2] = {0,0};
char init_time(TIMESETUP timeonoff[][10],unsigned char season)
{
	char mode;
	mode = GetMode();
	if(mode == 2)// che do bat tat theo mua
	{	
		if(season == SUMMER)
		{
			GetSeason(timeonoff[0],0);
			GetSeason(timeonoff[1],0);
		}
		else
		{
			GetSeason(timeonoff[0],1);				
			GetSeason(timeonoff[1],1);	
		}
		timeonoff[0][0].status  =  TRUE;
		timeonoff[0][1].status  =  FALSE;
		timeonoff[1][0].status  =  TRUE;
		timeonoff[1][1].status  =  FALSE;      
		sapxeptime(timeonoff[0],2);
		sapxeptime(timeonoff[1],2);
		number_alarm[0] = 2;
		number_alarm[1] = 2;
	}
   else if(mode == 1)// che do cai dat
   {
        cpy(timeonoff[0],flashv.alarmtime[0]);
        cpy(timeonoff[1],flashv.alarmtime[1]); 
        number_alarm[0] = flashv.cnt_alarm[0];
        number_alarm[1] = flashv.cnt_alarm[1];
   }
   else
   {

        return 0;
   }
   return 1;
}


/*******************************************************************************
* Function Name  : OnoffOutput
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
BOOL statuscur[2];
void OnoffOutput(TIMESETUP timeonoff[][10] , DATE_STRUCT timecurr)//n = 2*command_receive.data[0]
{
    char mode ;
    static char khoang[2];
    /*Get time currenr here*/
    //timecurr = get_cclk();
    if((timecurr.MONTH > 3)&&(timecurr.MONTH < 11))
    {	
        mode = init_time(timeonoff,SUMMER);
        if(!mode) return ;
    }
    else
    {
        mode = init_time(timeonoff,WINTER);
        if(!mode) return ;
    }
    statuscur[0] = (BOOL)timeonoff[0][khoang[0]].status;
    statuscur[1] = (BOOL)timeonoff[1][khoang[1]].status;
    khoang[0] = CheckKhoang(timeonoff[0],timecurr,number_alarm[0]);
    khoang[1] = CheckKhoang(timeonoff[1],timecurr,number_alarm[1]);
    if((BOOL)(statuscur[0] ^ (BOOL)timeonoff[0][khoang[0]].status) == TRUE)
    {           
        //first[0] = FALSE;
        if((BOOL)timeonoff[0][khoang[0]].status == 1) 
        {
            RELAY1(1);
            if(flashv.message_report == 1)
                SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,"Thiet bi 1 da duoc bat\n");
        }
        else 
        {
            RELAY1(0);
            if(flashv.message_report == 1)
                SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,"Thiet bi 1 da Tat\n");
        }
    }	
    if((BOOL)((BOOL)statuscur[1] ^ (BOOL)timeonoff[1][khoang[1]].status) == TRUE)
    {
        //first[1] = FALSE;
        if((BOOL)timeonoff[1][khoang[1]].status == 1) 
        {
            RELAY2(1);
            if(flashv.message_report == 1)
                SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,"Thiet bi 2 da duoc bat\n");
        }
        else 
        {
            RELAY2(0);
            if(flashv.message_report == 1)
                SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,"Thiet bi 2 da Tat\n");
        }
    }	         
}

