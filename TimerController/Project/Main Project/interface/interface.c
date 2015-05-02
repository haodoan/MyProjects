/*********************************************************************
AddTime:HrOn:MinOn,HrOff:MinOff
TimeSet:Hr:Min
Timeis=?
Mode:Auto
Mode:Manual
Season:Winter,HrOn:MinOn,HrOff:MinOff
Season:Summer,HrOn:MinOn,HrOff:MinOff

*******************************************************************/
#include "interface.h"
extern char function_eeprom ;
// enum of commander
enum
{
	ADDTIME1 = 0,
	ADDTIME2,
	TIMEIS,
	TIMESET,
	DATESET,
	MODE,
	SEASONSUMMER,
	SEASONWINTER,
	TB1ON,
    TB2ON,
    TB1OFF,
    TB2OFF,
    TBALLON,
    TBALLOFF,
	PH,
	KPH,
	LOG,
	HELP,
    MODE_ERROR,
	CMDERR
};

extern char status_tb[2];
uint8_t mode = 0;
extern SMS_GSM decodeSMS;
extern char function_eeprom ;
extern STRUCT_EEPROM_SAVE flashv;
/*******************************************************************************
* Function Name  : GetCmdData
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
STRUCTCOMMAND GetCmdData(char *str)
{
	STRUCTCOMMAND command_receive;
	char * pch,temp;
	int i=0;		 
	pch = strtok (str," :,=#");  
	strcpy(command_receive.cmd,pch);
	while (pch != NULL)
	{ 
		pch = strtok(NULL,", ");  	
		temp = atoi(pch);
		if(!temp)
		{         
		   if(pch != NULL){
					if(*pch != '0')
					{	
						if(i < 2)
							strcpy(command_receive.data_str[i],pch);
					}
					else
						command_receive.data[i] = 0;
					}
		}
		else
		   command_receive.data[i] = temp;
			command_receive.countdata = i++;					
	}
		return command_receive;
}

extern void help()
{
    char help_buf[160] = {0};
    strcat(help_buf,"1.Hengio:Giobat1,Giotat1,..,..,\n");
    strcat(help_buf,"2.Thoigian\n");
    strcat(help_buf,"3.Caidatgio:hh,mm,ss\n");
    strcat(help_buf,"4.Caidatngay:dd,mm,yy\n");
    strcat(help_buf,"5.Chedo:0 hoac 1\n");
    //printf("    0 :Gio tu dong theo tung mua\n");
    //printf("    1 :Tu cai dat \n");
    //printf("6.Mua:0 hoac 1,giobat,phutbat,giotat,phuttat\n");
    //printf("    0 :mua he\n");
    //printf("    1 :mua dong\n");
    strcat(help_buf,"6.tb1 bat hoac tat\n");
    strcat(help_buf,"7.tb2 bat hoac tat\n");
    strcat(help_buf,"8.batca hoac tatca\n");
    SentEnglis_SIMmsg(decodeSMS.numberPhone,help_buf);

    //printf("10.PH hoac KPH\n");
    //printf("11.LOG\n");

}
/*******************************************************************************
* Function Name  : UARTCommand
* Description    : Get data through frame header
* Input          : buffer data from UART
* Output         : data 
* Return         : command numberous
* Attention		   : None
*******************************************************************************/
uint8_t UARTCommand(UART_PACKKET_STRUCT *uart_packet, STRUCTCOMMAND result)
{
		//STRUCTCOMMAND result;
		char *p;
        char mode ;
        mode = GetMode();
		p = result.cmd;
		if(strcmp(p,"HENGIO1") == 0)
		{
			char i,j,k = 0;
			/*Analy and get ON and OFF setting time */
			for(j = 0;j < 2;j++){
				for(i = j;i < result.countdata ;i = i+4)
				{
						if(i%4 == 0){
							uart_packet->alarmtime[i/4].hr = result.data[i];
							uart_packet->alarmtime[i/4].min = result.data[i+1];
							uart_packet->alarmtime[i/4].status =  TRUE;
							uart_packet->cntdata = result.countdata;
							k = i/4 +1;
						 }	
						else if(i%4 == 1){
							uart_packet->alarmtime[k + i/4].hr = result.data[i+1];
							uart_packet->alarmtime[k + i/4].min = result.data[i+2];
							uart_packet->alarmtime[k + i/4].status =  FALSE;
						}
				}
			}
			flashv.cnt_alarm[0] = uart_packet->cntdata /2;
			function_eeprom = SAVEMEM;
			return ADDTIME1;
		}
		else if(strcmp(p,"HENGIO2") == 0)
		{
			char i,j,k = 0;
			/*Analy and get ON and OFF setting time */
			for(j = 0;j < 2;j++){
				for(i = j;i < result.countdata ;i = i+4)
				{
						if(i%4 == 0){
							uart_packet->alarmtime[i/4].hr = result.data[i];
							uart_packet->alarmtime[i/4].min = result.data[i+1];
							uart_packet->alarmtime[i/4].status =  TRUE;
							uart_packet->cntdata = result.countdata;
							k = i/4 +1;
						 }	
						else if(i%4 == 1){
							uart_packet->alarmtime[k + i/4].hr = result.data[i+1];
							uart_packet->alarmtime[k + i/4].min = result.data[i+2];
							uart_packet->alarmtime[k + i/4].status =  FALSE;
						}
				}
			}
			flashv.cnt_alarm[1] = uart_packet->cntdata /2;
			function_eeprom = SAVEMEM;
			return ADDTIME2;
		}
      
		else if(strcmp(p,"THOIGIAN") == 0)
		{
				return TIMEIS;
		}
		else if(strcmp(p,"CAIDATGIO") == 0)
		{
            memcpy(uart_packet->timeset,result.data,3);
            return TIMESET;
		}		
		else if(strcmp(p,"CAIDATNGAY") == 0)
		{
            memcpy(uart_packet->dateset,result.data,3);
            return DATESET;
		}	
		else if(strcmp(p,"CHEDO") == 0)
		{
         if(!strcmp(result.data_str[0],"HENGIO"))
            uart_packet->mode = 1;
         else if(!strcmp(result.data_str[0],"THEOMUA"))
            uart_packet->mode = 2;
         else if(!strcmp(result.data_str[0],"BATTAT"))
            uart_packet->mode = 0;
			function_eeprom = SAVEMEM;	 
			return MODE;

		}
		else if(strcmp(p,"MUA") == 0) 
		{
			if(result.data[0] == 0){ // mua he
				uart_packet->summer[0].hr =  result.data[1];
				uart_packet->summer[0].min = result.data[2];
				uart_packet->summer[0].status = TRUE;
				uart_packet->summer[1].hr = result.data[3];
				uart_packet->summer[1].min = result.data[4];
				uart_packet->summer[1].status = FALSE;
				return SEASONSUMMER;	
			}
			else if(result.data[0] == 1){ // mua dong 
				uart_packet->winter[0].hr =  result.data[1];
				uart_packet->winter[0].min = result.data[2];
				uart_packet->winter[0].status = TRUE;
				uart_packet->winter[1].hr = result.data[3];
				uart_packet->winter[1].min = result.data[4];
				uart_packet->winter[1].status = FALSE;			
				return SEASONWINTER;	
			}		
			function_eeprom = SAVEMEM;	
			return CMDERR;
		}
      	else if(strcmp(p,"TB1") == 0)
      	{

         	if(0 == mode)
         	{             
				if(!strcmp(result.data_str[0],"BAT"))
				{				 
				    status_tb[0] = 1;     
                    return TB1ON ;
				}
				else if(!strcmp(result.data_str[0],"TAT"))
				{
				    status_tb[0] = 0;   
                    return TB1OFF;
				}
				else
				{
				    return CMDERR;
				}
            }
            else
            {
                return MODE_ERROR ;    
            }
            
      }
        else if(strcmp(p,"TB2") == 0)
        {

            if(0 == mode)
            {             
                if(!strcmp(result.data_str[0],"BAT"))
                {                
                    status_tb[1] = 1;     
                    return TB2ON;
                }
                else if(!strcmp(result.data_str[0],"TAT"))
                {
                    status_tb[1] = 0;   
                    return TB2OFF;
                }
                else
                {
                    return CMDERR;
                }
            }
            else
            {
                return MODE_ERROR ;    
            }
        }          
        else if(!strcmp(p,"BATCA"))
        {
            if(0  == mode)
            {
                status_tb[0] = 1;
                status_tb[1] = 1;   
                return TBALLON;
            }
            else
            {
                return MODE_ERROR ;    
            }            
        }
        else if(!strcmp(p,"TATCA"))
        {
            if(0 == mode)
            {
                status_tb[0] = 0;
                status_tb[1] = 0;   
                return TBALLOFF;
            }
            else
            {
                return MODE_ERROR ;    
            }            
        }      
		else if(!strcmp(p,"PH"))
		{
            function_eeprom = SAVEMEM;
            return PH;
		} 	  
		else if(!strcmp(p,"KPH"))
		{
            function_eeprom = SAVEMEM;
            return KPH;
		} 	  	  
		else if(strcmp(p,"LOG") == 0)
		{
			return LOG;
		}
//		else if(strcmp(p,"HELP") == 0)
//		{
//			return HELP;
//		}
		else
		{
			return CMDERR;
		}

}

/*******************************************************************************
* Function Name  : CommadProcess
* Description    : Process commander
* Input          : buffer data from UART
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void CommadProcess(TIMESETUP timeonoffG[][10],UART_PACKKET_STRUCT uart_packet , uint8_t command)
{
	//uint8_t command;
	//UART_PACKKET_STRUCT uart_packet;
	char db_vt;
	DATE_STRUCT timecurr,timeset;
	function_eeprom = UNSAVE;	
	db_vt = SearchExistUser(decodeSMS.numberPhone);// xem sdt nam o vt nao cua danh ba
	if(db_vt == 10)
      return ;   
	switch(command)
	{
		case ADDTIME1: //set alarm time command
			cpy(timeonoffG[0],(TIMESETUP*)uart_packet.alarmtime);
			sapxeptime(timeonoffG[0],uart_packet.cntdata/2);
			cpy(flashv.alarmtime[0],timeonoffG[0]);
			function_eeprom = SAVEMEM;	
			SentEnglis_SIMmsg(decodeSMS.numberPhone,"Hen gio cho thiet bi 1 thanh cong\n");
			break;	
		case ADDTIME2: //set alarm time command
			cpy(timeonoffG[1],(TIMESETUP*)uart_packet.alarmtime);
			sapxeptime(timeonoffG[1],uart_packet.cntdata/2);
			cpy(flashv.alarmtime[1],timeonoffG[1]);
			function_eeprom = SAVEMEM;	        
			SentEnglis_SIMmsg(decodeSMS.numberPhone,"Hen gio cho thiet bi 2 thanh cong\n");
			break;	      
		case TIMESET: //set time clock command
			timeset = get_cclk();
			timeset.HOUR 		= uart_packet.timeset[0];
			timeset.MINUTE 	= uart_packet.timeset[1];
			timeset.SECOND 	= uart_packet.timeset[2];
            delay_ms(500);
            if(!SetingCCLK(timeset ,"+07"))
            {
                delay_ms(500);
                SentEnglis_SIMmsg(decodeSMS.numberPhone,"Cai dat gio ERROR\n");            
            }    
            else
            {
                delay_ms(500);
                SentEnglis_SIMmsg(decodeSMS.numberPhone,"Cai dat gio thanh cong.Vui long kiem tra lai thoi gian\n");
            }
            
            break;
		case DATESET: //set date command
			timeset = get_cclk();
			timeset.DAY 		= uart_packet.dateset[0];
			timeset.MONTH 	    = uart_packet.dateset[1];
			timeset.YEAR 		= uart_packet.dateset[2];			
            SetingCCLK(timeset ,"+07");
			if(!SetingCCLK(timeset ,"+07"))
            {
                delay_ms(500);
                SentEnglis_SIMmsg(decodeSMS.numberPhone,"Cai dat ngay ERROR\n");            
            }    
            else
            {
                delay_ms(500);
                SentEnglis_SIMmsg(decodeSMS.numberPhone,"Cai dat ngay thanh cong.Vui long kiem tra lai thoi gian\n");
            }
		case TIMEIS: //show time clock command
		{
            char tempbuff[50];
			timecurr = get_cclk();
			sprintf(tempbuff,"Gio :%d:%d:%d\nNgay : %d-%d-%d\n",timecurr.HOUR,timecurr.MINUTE,timecurr.SECOND,timecurr.DAY,timecurr.MONTH,timecurr.YEAR)	;
			SentEnglis_SIMmsg(decodeSMS.numberPhone,tempbuff);         
			break;	
		}            				
		case MODE:// mode command
			 /****
				2 - auto season
				1 - fix time 
				0 - Manual setting on off
			 **/
			 flashv.mode = uart_packet.mode;	
			 if(flashv.mode == 0)	SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban da cai dat che do bat tat thanh cong\r");
			 else if(flashv.mode == 1)	SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban da cai dat che do cai dat hen gio \r");
			 else if(flashv.mode == 2)	SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban da cai dat che do tu dong thanh cong\r");
			 function_eeprom = SAVEMEM;	
			 break;			
		case SEASONSUMMER:
			 flashv.summer[0] = uart_packet.summer[0];
			 flashv.summer[1] = uart_packet.summer[1];
			 SentEnglis_SIMmsg(decodeSMS.numberPhone,"Cai dat gio mua he thanh cong\n");	
			 function_eeprom = SAVEMEM;	
			break;			
		case SEASONWINTER:
			 flashv.winter[0] = uart_packet.winter[0];
			 flashv.winter[1] = uart_packet.winter[1];
			 SentEnglis_SIMmsg(decodeSMS.numberPhone,"Cai dat gio mua dong thanh cong\n");	
			 function_eeprom = SAVEMEM;		
			 break;				
        case TB1ON: 
            SentEnglis_SIMmsg(decodeSMS.numberPhone,"Thiet bi 1 da bat\n");
            break;
        case TB1OFF: 
            SentEnglis_SIMmsg(decodeSMS.numberPhone,"Thiet bi 1 da tat\n");      
            break;
        case TB2ON: 
            SentEnglis_SIMmsg(decodeSMS.numberPhone,"Thiet bi 2 da bat\n");      
            break;
        case TB2OFF: 
            SentEnglis_SIMmsg(decodeSMS.numberPhone,"Thiet bi 2 da tat\n");                  
            break;
        case TBALLON: 
            SentEnglis_SIMmsg(decodeSMS.numberPhone,"Da bat ca hai thiet bi\n");
            break ;
        case TBALLOFF: 
            SentEnglis_SIMmsg(decodeSMS.numberPhone,"Da tat ca hai thiet bi\n");           
            break;                                                            
		case LOG : 
		{
			char logbuff[160];
			char timeseason[50];
			memset(logbuff,0,160);
			if(0==flashv.mode)
				strcat(logbuff,"Chedo:bat tat\n");
			else if(1==flashv.mode)
				strcat(logbuff,"Chedo:hen gio\n");				
			else if(2 == flashv.mode)
				strcat(logbuff,"Chedo:theo mua\n");				
			sprintf(timeseason,"Mua he :Bat %d:%d va Tat:%d:%d.\n",flashv.summer[0].hr,flashv.summer[0].min, \
			flashv.summer[1].hr,flashv.summer[1].min);
			strcat(logbuff,timeseason);				
			sprintf(timeseason,"Mua dong :Bat %d:%d va Tat:%d:%d.\n",flashv.winter[0].hr,flashv.winter[0].min, \
			flashv.winter[1].hr,flashv.winter[1].min);          		
            strcat(logbuff,timeseason);				            
			SentEnglis_SIMmsg(decodeSMS.numberPhone,logbuff);
			break;				
		}
		case PH:
			flashv.message_report = 1;
			SentEnglis_SIMmsg(decodeSMS.numberPhone,"Da cai dat che do phan hoi tin nhan khi thay doi trang thai thiet bi\n");
            break;
		case KPH:
			flashv.message_report = 0;
		    SentEnglis_SIMmsg(decodeSMS.numberPhone,"Da Huy cai dat phan hoi tin nhan\n"); 
            break;		
//		case HELP:
//			help();
//			break;
        case MODE_ERROR:
            SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban can chuyen sang che do bat tat .Nhan tin \"chedo battat\" de thay doi\n");
            break ;
		case CMDERR:
				SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ma lenh sai,Vui long kiem tra lai\n");
				break;			
	}
}

/*******************************************************************************
* Function Name  : GetMode
* Description    : check set mode is auto or fix time ? 
* Input          : None
* Output         : Mode 0:auto,1 :fix
* Return         : Mode 0:auto,1 :fix
* Attention		   : None
*******************************************************************************/
uint8_t GetMode(void)
{
	return (flashv.mode );		
}

/*******************************************************************************
* Function Name  : cpy
* Description    : copy struct data 
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
extern void cpy(TIMESETUP *a,TIMESETUP *b)
{
	int i;
	for(i = 0;i<10;i++)
		a[i] = b[i];
}
/*******************************************************************************
* Function Name  : GetSeason
* Description    : Get season time of summer and winter 
* Input          : None
* Output         : None
* Return         : None
* Attention		   : None
*******************************************************************************/
void GetSeason(TIMESETUP *timeonoff,uint8_t sea)
{
	if(!sea)
		cpy(timeonoff,flashv.summer);
	else	
		cpy(timeonoff,flashv.winter);
}

