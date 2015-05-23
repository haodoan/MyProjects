/****************************************Copyright (c)****************************************************
**                                      
**                                  Kien thuc khong bao gio la huu han.
**          yeu cau include them tim2_interrupt.h de phuc vu cho getecho va getlongecho
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               
** Descriptions:            File khai bao cac ham cai dat
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Hai Dang
** Created date:            2010-10-30
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
Note : su dung systick de tinh toan thoi gian timeout
*********************************************************************************************************/
#include <simcom.h>
/* Private variables ---------------------------------------------------------*/
GSM 			GSM1;
/* end -----------------------------------------------------------------------*/
extern void __delay_ms(u16 time)
{
 	delay_ms(time);
}
/*******************************************************************************
* Function Name  : Pwrkey
* Description    : kich hoat cho module khoi dong
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern void Pwrkey(char key)
{
		key? GPIO_SetBits(GPIOB , GPIO_Pin_9):GPIO_ResetBits(GPIOB , GPIO_Pin_9);
}
/*******************************************************************************
* Function Name  : GSM_putsf
* Description    : truyen 1 chuoi~ len SIMCOM
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern void GSM_putsf(char *string)
{
	printf(string);
}
/*******************************************************************************
* Function Name  : GSM_putsf
* Description    : truyen 1 chuoi~ len SIMCOM
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char TestConnecSim(void)
{
	GSM_putsf("AT\r"); 
      if(!GetEcho("OK")) return 0;
	
	GSM_putsf("AT+CREG?\r");                        //register network GSM
    if(!GetEcho("+CREG: 1,1")) return 0;
	return 1;
}
/*******************************************************************************
* Function Name  : GetEchoLong
* Description    : kiem tra 1 chuoi~ xem co trong bo dem nhan khong
* Input          : chuoi~ can kiem tra
* Output         : None
* Return         : 1 : neu chuoi~ duoc tim thay
				   0 : neu khong tim thay chuoi~
* Attention		 : None
*******************************************************************************/
extern volatile unsigned long int sysTick_counter;
extern char GetEchoLong(char *string)
{
	uint32_t systickTimeOut = sysTick_counter + 8000;
	while(GSM1.flag_rx==0 && (sysTick_counter < systickTimeOut)); /*dang khong du lieu va ngoi cho, dem thoi gian timeout*/
	/*reset lai co` nhan biet co du lieu ve*/
	if(GSM1.flag_rx)
	{
		GSM1.flag_rx = 0;
		if(strstr(GSM1.buff_rx[GSM1.co_rx],string))	/*kiem tra chuoi~*/
				return 1;
	}
	GSM1.flag_rx = 0;
 	return 0;
}
/*******************************************************************************
* Function Name  : GetEcho
* Description    : kiem tra 1 chuoi~ xem co trong bo dem nhan khong
* Input          : chuoi~ can kiem tra
* Output         : None
* Return         : 1 : neu chuoi~ duoc tim thay
				   0 : neu khong tim thay chuoi~
* Attention		 : None
*******************************************************************************/
extern char GetEcho(char *string)
{
	uint32_t systickTimeOut = sysTick_counter + 5000;
	
	#if 1
	while(GSM1.flag_rx==0 && (sysTick_counter < systickTimeOut)); /*dang khong du lieu va ngoi cho, dem thoi gian timeout*/
	/*reset lai co` nhan biet co du lieu ve*/
	if(GSM1.flag_rx)
	{
		GSM1.flag_rx = 0;
		if(strstr(GSM1.buff_rx[GSM1.co_rx],string))	/*kiem tra chuoi~*/
            return 1;
	}
	GSM1.flag_rx = 0;
 	return 0;
	#endif
	
		
	
}

extern char GetEcho_(char *string)
{
	uint32_t systickTimeOut = sysTick_counter + 5000;
	while(1)
	{
		if(GSM1.flag_rx)
		{
			GSM1.flag_rx = 0;
			if(strstr(GSM1.buff_rx[GSM1.co_rx],string))	/*kiem tra chuoi~*/
					return 1;		
      }
      if(sysTick_counter > systickTimeOut)
         return 0;

	}

}
/*******************************************************************************
* Function Name  : GetEchoTwo
* Description    : kiem tra 1 chuoi~ xem co trong bo dem nhan khong
* Input          : chuoi~ can kiem tra
* Output         : None
* Return         : 1 : neu chuoi~ duoc tim thay
				   0 : neu khong tim thay chuoi~
* Attention		 : None
*******************************************************************************/
extern char GetEchoTwo(char *string, char *string2)
{
	uint32_t systickTimeOut = sysTick_counter + 20000;
	while(sysTick_counter < systickTimeOut)
	{
			if(strstr(GSM1.buff_rx[GSM1.co_rx],string) || strstr(GSM1.buff_rx[GSM1.co_rx],string2))	/*kiem tra chuoi~*/
			{
					GSM1.flag_rx = 0;
					return 1;
			}
	}
	GSM1.flag_rx = 0;
 	return 0;
}

/*******************************************************************************
* Function Name  : Init_GSM
* Description    : Kiem tra cac ket noi mang
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char Init_GSM(void)
{
   GSM_putsf("AT\r"); /*kiem tra ket noi*/
      if(!GetEcho("OK")) return 0;
   GSM_putsf("AT+CLTS=1\r"); /*Update thoi gian khi khoi dong*/
      if(!GetEcho("OK")) return 0;
	GSM_putsf("ATE0\r"); /*khong phan hoi dong lenh . ATE1 phan hoi dong lenh*/
      if(!GetEcho("OK")) return 0;

	GSM_putsf("AT+CLIP=1\r");
      if(!GetEcho("OK")) return 0; /*Dinh dang chuoi tra ve khi co cuoc goi den*/
	
	GSM_putsf("AT&W\r");
      if(!GetEcho("OK")) return 0;  /*Luu cau hinh vao bo nho*/

	GSM_putsf("AT+CMGF=1\r");       //mode SMS text
      if(!GetEcho("OK")) return 0; 

	GSM_putsf("AT+ICF=3,3\r");	 /*8 data , 0 parity, 1stop*/
	  if(!GetEcho("OK")) return 0; 

	GSM_putsf("AT+IFC=2,2\r");   /* RTS va CTS*/
	  if(!GetEcho("OK")) return 0;

	GSM_putsf("AT+IPR=0\r");   /*auto baurate*/
	  if(!GetEcho("OK")) return 0;

	GSM_putsf("AT+CNMI=3,1,2,0\r"); /*Thiet lap che do thong bao cho TE khi MT nhan duoc tin nhan moi*/
	//GSM_putsf("AT+CNMI=1,2,0,0,0\r"); // Tra ve tin nhan luon
    if(!GetEcho("OK")) return 0; 

	GSM_putsf("AT+CSAS\r");         /*luu cau hinh cai dat AT+CMGF va AT+CNMI*/
    if(!GetEcho("OK")) return 0;

	GSM_putsf("AT+CIPMODE=0\r");    /*Lua chon phuong thuc giao tiep voi modem*/
    if(!GetEcho("OK")) return 0;  
					
	GSM_putsf("AT+CPIN?\r");         /*test SIM ID*/
		if(!GetEcho("OK")) 
			{
					return 0;
			}
	
	GSM_putsf("AT+CREG=1\r");         /*register network GSM*/
      if(!GetEcho("OK")) 
				{ 
						return 0;
				}
		

	GSM_putsf("AT+CREG?\r");                        //register network GSM
      if(!GetEcho("+CREG: 1,1")) return 0;

	GSM_putsf("AT+CFUN=1\r");         /*set phone functionality*/
      if(!GetEcho("OK")) return 0;

	GSM_putsf("AT+CFUN?\r");         /*set phone functionality*/
      if(!GetEcho("+CFUN: 1")) return 0;
								
	 return 1;
}
/*******************************************************************************
* Function Name  : StartGSM
* Description    : chan kich hoat cho module bat dau khoi dong
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern void StartGSM(void)
{
	Pwrkey(1);  /*bat cau hinh*/
	__delay_ms(1200);
	Pwrkey(0);
}
/*******************************************************************************
* Function Name  : Start_SIMCOMC
* Description    : Khoi dong va kiem tra cac ket noi mang
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern void Start_SIMCOM(void)
{
  char c=1;
	char errorStart = 0;
	StartGSM();
	//Ket thuc khoi dong SIM lan dau tien
    Re_Start:
		if(++errorStart>5)
		{
    		GSM_putsf("AT\r"); /*kiem tra ket noi*/
      		if(!GetEcho("OK")) 
					{
						StartGSM();
						errorStart = 0;
					}
		}
		__delay_ms(100);
        c = Init_GSM();
		__delay_ms(100);
		
        if(c==0)
    goto Re_Start;
}  
/*******************************************************************************
* Function Name  : Init_GSM
* Description    : Kiem tra cac ket noi mang
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char ReadEnglis_SIMmsg(unsigned char msgnum)
{
	char buff_st[20]="";
	sprintf(buff_st,"AT+CMGR=%d\r",msgnum);
	GSM_putsf(buff_st);
	if (GetEcho("+CMGR:")) 
		return 1;
	else
		return 0;
}
/*******************************************************************************
* Function Name  : GetPointText
* Description    : Tra ve vi tri con tro noi ky tu checkText xuat hien lan 
*					thu number trong chuoi~ textsms
* Input          : (char *textsms,char checkText,char number)
* Output         : None
* Return         : con tro xuat hien ky tu
* Attention		 : neu khong co tra ve NULL
*******************************************************************************/
extern char *GetPointText(char *textsms,char checkText,char number)
{
	char i=0,j=0; 	
	char len_Text = 0;
	len_Text = strlen(textsms);
	for(i=0;i<len_Text;i++)
	{
	 	if(*(textsms)==checkText)
		{
		 	j++;
		}
		if(j==number)
		{
		 	return textsms;
		}
		else
			textsms++;
	}
	return 0;
}
/*******************************************************************************
* Function Name  : Sdecode_sms
* Description    : Giai ma tin nhan lay thong tin
* Input          : Chuoi tin nhan duoc tra ve tu SIMCOM
* Output         : None
* Return         : struct SMS
* Attention		 : None
*******************************************************************************/
/*+CMTI: "SM",28  gui lenh AT+CMGR=28\r
*+CMGR: "REC READ","+841294193048",,"11/11/02,20:00:43+28"
*Yeu anh nhjeu lam
*
*OK
*/
extern char decode_sms(char *textSms,SMS_GSM *sms1)
{
	u16 i=0,j=0;
	char *seachChar3,*seachChar4,*seachChar5,*seachChar6;
	seachChar3 = GetPointText(textSms,'"',3);
	seachChar4 = GetPointText(textSms,'"',4);
	seachChar5 = GetPointText(textSms,'"',5);
	seachChar6 = GetPointText(textSms,'"',6);
	if(!(seachChar3&&seachChar4&&seachChar5&&seachChar6)) return 0;
	/* Lay thong tin ve so dien thoai*/
	seachChar3 += 4;
	sms1->numberPhone[0] = '0';
	i = 1;
	while(1)
	{
	 	sms1->numberPhone[i] = *seachChar3;
		if((seachChar3 == seachChar4)||(i==nPhone))
		{
		 	break;
		}
		else
			{
			 	i++;
				seachChar3++;
			}
	}

	for(j=i;j<nPhone;j++)
	{
	 	sms1->numberPhone[j]=0;	
	}
	sms1->numberPhone[nPhone-1] = 0;
	/*
	+CMGR: "REC READ","+841294193048",,"11/11/02,20:00:43+28"
	Yeu anh nhjeu lam
	-Lay thong tin ve thang nam
	Nam	 thang ngay gio phut giay "so thu tu tin nhan"
	*/
	sms1->textSmsTime.years[0]  = *(seachChar5+1);
	sms1->textSmsTime.years[1]  = *(seachChar5+2);
	sms1->textSmsTime.months[0] = *(seachChar5+4);
	sms1->textSmsTime.months[1] = *(seachChar5+5);
	sms1->textSmsTime.days[0]   = *(seachChar5+7);
	sms1->textSmsTime.days[1]   = *(seachChar5+8);

	sms1->textSmsTime.hours[0]  = *(seachChar5+10);
	sms1->textSmsTime.hours[1]  = *(seachChar5+11);
	sms1->textSmsTime.minutes[0]= *(seachChar5+13);
	sms1->textSmsTime.minutes[1]= *(seachChar5+14);
	sms1->textSmsTime.seconds[0]= *(seachChar5+16);
	sms1->textSmsTime.seconds[1]= *(seachChar5+17);
	sms1->sttSms[0]              = *(seachChar5+19);
	sms1->sttSms[1]              = *(seachChar5+20);
	/* Lay thong tin ve noi dung tin nhan*/
	while(*seachChar6 != '\n'&&*seachChar6!='\0')
		seachChar6++ ;
	seachChar6++;
	i = 0;
	while(1)
	{
	 	sms1->contentTextSms[i] = *seachChar6;
		if((*seachChar6 == '\r')||(*seachChar6 == '\n')||(i>=cTextSms-1))
		{
		 	break;
		}
		else
			{
			 	i++;
				seachChar6++;
			}
	}
	for(j=i;j<cTextSms;j++)
	{
	 	sms1->contentTextSms[j]=0;	
	}
	sms1->contentTextSms[cTextSms-1] = '\0';
	return 1;
}
/*******************************************************************************
* Function Name  : decode_call
* Description    : giai ma cuoc goi 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
RING


+CLIP: "0936418566",161,"",,"Hai dang",0
*******************************************************************************/
extern char decode_call(char *textSms,CALL_GSM *call)
{
	u16 i=0,j=0;
	char *seachChar1,*seachChar2;
	seachChar1 = GetPointText(textSms,'"',1);
	seachChar2 = GetPointText(textSms,'"',2);
	if((!seachChar1)||(!seachChar2)) 
		return 0;

	seachChar1++;
	i=0;
	while(1)
	{
	 	call->numberPhone[i] = *seachChar1;
		if((seachChar1 == seachChar2)||(i==nPhone))
		{
		 	break;
		}
		else
			{
			 	i++;
				seachChar1++;
			}
	}

	for(j=i;j<nPhone;j++)
	{
	 	call->numberPhone[j]=0;	
	}
	call->numberPhone[nPhone-1] = 0;
	return 1;
}
/*******************************************************************************
* Function Name  : SentEnglis_SIMmsg
* Description    : gui tin nhan 
* Input          : so dien thoai va noi dung tin nhan
* Output         : None
* Return         : co gui duoc thanh cong hay khong
* Attention		 : None
*******************************************************************************/
char SentEnglis_SIMmsg(char *phonenum,const char *msgstr)
{	
	char phone[30] ={0};	
	sprintf(phone,"AT+CMGS=%c+84%s%c\r",34,phonenum+1,34);
	GSM_putsf(phone);
	if(GetEcho(">")) 
   {
     printf("%s%c",msgstr,26);
     if(GetEcho("OK")) 
         return 1;
      return 0;
   }
	return 0;
}
/*******************************************************************************
* Function Name  : WaitSendSms
* Description    : cho phan hoi cua viec gui tin nhan
* Input          : None
* Output         : None
* Return         : 0:chua gui,1:gui thanh cong
* Attention		 : None
*******************************************************************************/
extern char WaitSendSms(void)
{
        __delay_ms(3000);
        if(!GetEcho("+CMGS:")) return 0;
        return 1;
}
extern char WaitSendSMS(void)
{
        if(GetEcho("+CMGS:")) return 0;
        return 1;
}
/*******************************************************************************
* Function Name  : CallPhone
* Description    : cho phan hoi cua viec gui tin nhan
* Input          : None
* Output         : None
* Return         : 0:chua gui,1:gui thanh cong
* Attention		 : None
*******************************************************************************/
extern char CallPhone(char *phonenum)
{
		GSM_putsf("ATD");
		GSM_putsf(phonenum);
		GSM_putsf(";\r");
    return 1;
}
/*******************************************************************************
* Function Name  : DisconnectCall
* Description    : Ngat ket noi cuoc goi
* Input          : None
* Output         : None
* Return         : 0:ngat ket noi khong thanh cong,1:ngat ket noi thanh cong
* Attention		 : None
*******************************************************************************/
extern char DisconnectCall(void)
{   
       GSM_putsf("ATH\r");
       if (!GetEcho("OK")) return 0;
       return 0;
}
/*******************************************************************************
* Function Name  : HangUpCall
* Description    : Ket thuc cuoc goi
* Input          : None
* Output         : None
* Return         : 0:ket thuc khong thanh cong,1:ket thuc thanh cong
* Attention		 : None
*******************************************************************************/
extern char HangUpCall(void)
{   
        GSM_putsf("AT+CHUP\r");
        if (!GetEcho("OK")) return 0;
        return 1;
}
/*******************************************************************************
* Function Name  : CallUpCall
* Description    : Nhan cuoc goi
* Input          : None
* Output         : None
* Return         : 0:nhan cuoc khong thanh cong,1:nhan cuoc goi thanh cong
* Attention		 : None
*******************************************************************************/
extern char CallUpCall(void)
{ 
       GSM_putsf("ATA\r");
       if (!GetEcho("OK")) return 0;
       return 1;
}
/*******************************************************************************
* Function Name  : CCLK
* Description    : Lay thong tin ngay thang tu module SIMCOMC
* Input          : None
* Output         : None
* Return         : 0:nhan cuoc khong thanh cong,1:nhan cuoc goi thanh cong
* Attention		 : None
*******************************************************************************/
extern char CCLK(void)
{
       GSM_putsf("AT+CCLK?\r");
			 //if (!GetEcho("OK")) return 0;
       return 1;
}

DATE_STRUCT get_cclk(void)
{
   DATE_STRUCT clock ={12,1,1,0,0,0};
	 uint32_t systickTimeOut ;//= sysTick_counter + 5000;
   char *p;
   if(1 == CCLK())
   {		  
		  systickTimeOut = sysTick_counter + 5000;
			while(GSM1.flag_rx==0 && (sysTick_counter < systickTimeOut)); /*dang khong du lieu va ngoi cho, dem thoi gian timeout*/
      if(GSM1.flag_rx)
      {
         GSM1.flag_rx = 0;
         if(strstr(GSM1.buff_rx[GSM1.co_rx],"\r\n+CCLK:"))
         {   
            p = strstr(GSM1.buff_rx[GSM1.co_rx],"+CCLK:") + 8;
            clock.YEAR = ((*(p))-'0')*10 + (*(p+1))-'0';
            p +=3;		
            clock.MONTH = ((*(p))-'0')*10 + (*(p+1))-'0';
            p +=3;		
            clock.DAY = ((*(p))-'0')*10 + (*(p+1))-'0';
            p +=3;
            clock.HOUR = ((*(p))-'0')*10 + (*(p+1))-'0';	
            p +=3;
            clock.MINUTE = ((*(p))-'0')*10 + (*(p+1))-'0';
            p +=3;
            clock.SECOND = ((*(p))-'0')*10 + (*(p+1))-'0';
         }
      }         
   }
   else
   {
      printf("update time error\n");
   }
   return clock;   
}

/*******************************************************************************
* Function Name  : SetingCCLK
* Description    : Cai dat ngay gio cho SIM COM
* Input          : None
* Output         : None
* Return         : 0:nhan cuoc khong thanh cong,1:nhan cuoc goi thanh cong
* Attention		 : None
*******************************************************************************/
char SetingCCLK(DATE_STRUCT clock ,char *timezone)
{
     char buffSettingTime[50] = {0};
     sprintf(buffSettingTime,"AT+CCLK=%c%02d/%02d/%02d,%02d:%02d:%02d%s%c\r",34,clock.YEAR,clock.MONTH,clock.DAY,clock.HOUR,clock.MINUTE,clock.SECOND,timezone,34);
     GSM_putsf(buffSettingTime);
     if(!GetEcho("OK"))  return 0;
     return 1;     
}
/*******************************************************************************
* Function Name  : DellSms
* Description    : xoa tin nhan
* Input          : chi so tin nhan trong SIM
* Output         : None
* Return         : 0:khong thanh cong,1:thanh cong
* Attention		 : None
*******************************************************************************/
extern char DellSms(u16 index)
{
        char buffDellSms[20]="";
        sprintf(buffDellSms,"AT+CMGD=%d\r",index);
        GSM_putsf(buffDellSms);
        if(!GetEcho("OK"))  return 0;
        return 1;
}
/*******************************************************************************
* Function Name  : DellAllSms
* Description    : xoa tin nhan tu chi so 1 den total
* Input          : so tin nhan duoc xoa
* Output         : None
* Return         : 0:khong thanh cong,1:thanh cong
* Attention		 : None
AT+CMGD=?\r   :    Kiem tra xem SIM luu toi da duoc bao nhieu tin nhan
*******************************************************************************/
extern char DellAllSms(u16 total)
{   
        u16 temp;
		if(!TestConnecSim()) 
		  return 0;  /*kiem tra ket noi SIM*/
      for (temp=1;temp<=total;temp++) //delete all
		{
			__delay_ms(20);
        	while(!DellSms(temp));
		}
		return 1;
}
/*******************************************************************************
* Function Name  : DelAllSmsCmgda
* Description    : xoa toan bo tin nhan	, ho tro kieu number hoac text
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
char DelAllSmsCmgda(void)
{

	GSM_putsf("AT+CMGDA=\"DEL ALL\"\r");
	
	if(GetEcho("OK"))
	{
		return 1;
	}
/*
	GSM_putsf("AT+CMGDA=6\r");
	if(GetEcho("OK"))
	{
		return 1;
	}
	*/
	return 0;
	


	
}
/*******************************************************************************
* Function Name  : CheckUsd
* Description    : kiem tra tai khoan
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char CheckUsd(void)
{
	GSM_putsf("AT+CUSD=1,\"*101#\"\r");
   if(GetEcho("OK")) 
      return 1;
   else 
   return 0;
}
char Naptien(char *mathe)
{
   char mathetemp[30];
   sprintf(mathetemp,"AT+CUSD=1,\"*100*%s#",mathe);
	GSM_putsf(mathetemp);
   if(GetEcho("OK")) return 1;
   else return 0;
}
/*******************************************************************************
* Function Name  : CheckCsq
* Description    : kiem tra cuong do song
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern unsigned int CheckCsq(void)
{
	unsigned int csq = 100;
	char *p;
	GSM_putsf("AT+CSQ\r");
	if(GetEcho("OK"))
	{
		p = strstr(GSM1.buff_rx[GSM1.co_rx],"+CSQ:");
		if (!isalnum(*(p+7)))
	        csq=(*(p+6))-'0';
	    else 
	        csq=(((*(p+6))-'0')*10)+((*(p+7))-'0');
	}
	return csq;
}

/*******************************************************************************
* Function Name  : CheckCbc
* Description    : Kiem tra dien ap cua nguon dien
* Input          : none
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern VBAT CheckCbc(void)
{
	char *p;
	VBAT getVbat;
	getVbat.level = 0;
	getVbat.mV = 0;
	GSM_putsf("AT+CBC\r");
	if(GetEchoLong("+CBC:"))
	{
		p = GetPointText(GSM1.buff_rx[GSM1.co_rx],',',1);
		if (!isalnum(*(p+2)))
	        getVbat.level = (*(p+2))-'0';
	    else 
			if (!isalnum(*(p+3)))
	        	getVbat.level =(((*(p+1))-'0')*10)+((*(p+2))-'0');
			else
				getVbat.level =((*(p+1))-'0')*100+((*(p+2))-'0')*10+((*(p+3))-'0');
	    p = GetPointText(GSM1.buff_rx[GSM1.co_rx],',',2);
		getVbat.mV = ((*(p+1))-'0')*1000+((*(p+2))-'0')*100+((*(p+3))-'0')*10+((*(p+4))-'0');
	}
	return getVbat;
}
/*******************************************************************************
* Function Name  : ResetDefaultConfig_ATZ
* Description    : Reset ve trang thai mac dinh
* Input          : Co 2 trang thai reset
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char ResetDefaultConfig_ATZ(void)
{
	GSM_putsf("ATZ\r");
	if(GetEcho("OK")) 
		return 1; 
	else 
		return 0;
}
/*******************************************************************************
* Function Name  : DeactivateGprsPdpContext_CIPSHUT
* Description    : Tat GPRS PDP Context
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char DeactivateGprsPdpContext_CIPSHUT(void)
{
	GSM_putsf("AT+CIPSHUT\r");
	if(GetEcho("SHUT OK")) 
		return 1; 
	else 
		return 0;
}
/*******************************************************************************
* Function Name  : SetPhoneFunctionality_CFUN
* Description    : =1 / full functionality (Default)
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char SetPhoneFunctionality_CFUN(void)
{
	GSM_putsf("AT+CFUN=1\r"); //
	if(GetEcho("OK")) 
		return 1; 
	else 
		return 0;
}
/*******************************************************************************
* Function Name  : DeactivateGprsPdpContext_CIPSHUT
* Description    : Tat GPRS PDP Context
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char AttachDetachFromGprsService_CGATT(void)
{
	GSM_putsf("AT+CGATT=1\r");
	if(GetEcho("OK")) 
		return 1; 
	else 
		return 0;
}
/*******************************************************************************
* Function Name  : BrUpWiConnecWithGprs_CIICR
* Description    : Bat GPRS
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char BrUpWiConnecWithGprs_CIICR(void)
{
	GSM_putsf("AT+CIICR\r"); 
	if(GetEcho("OK")) 
		return 1; 
	else 
		return 0;
}
/*******************************************************************************
* Function Name  : GetLocalIpAddress_CIFSR
* Description    : Tra ve dia chi IP cua thiet bi
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char GetLocalIpAddress_CIFSR(void)
{
	GSM_putsf("AT+CIFSR\r"); 
	GetEcho("OK");
	return 1; 
}
/*******************************************************************************
* Function Name  : QueryCurrentConnecStatus_CIPSTATUS
* Description    : Kiem tra trang thai co ket noi voi GPRS
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char  QueryCurrentConnecStatus_CIPSTATUS(void)
{
	GSM_putsf("AT+CIPSTATUS\r"); 
	if(GetEcho("STATE: IP STATUS"))
		return 1; 
	else
		return 0;
}
/*******************************************************************************
* Function Name  : AddAnIpHeadWhenReceivingData_CIPHEAD
* Description    : Tells module to add an 'IP Header' to receive data 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char  AddAnIpHeadWhenReceivingData_CIPHEAD(void)
{
	GSM_putsf("AT+CIPHEAD=1\r"); 
	if(GetEcho("OK"))
		return 1; 
	else
		return 0;
}
/*******************************************************************************
* Function Name  : StartUpTcpConnection_CIPSTART
* Description    : Tells module to add an 'IP Header' to receive data 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
extern char  StartUpTcpConnection_CIPSTART(char *ip,char *po,char *data)
{
	char buffGprs[100]="";
	sprintf(buffGprs,"AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r",ip,po);
	GSM_putsf(buffGprs); 
	if(GetEcho("CONNECT OK"))
	{
		GSM_putsf("AT+CIPSEND\r"); 
		if(GetEcho(">"))
		{
			GSM_putsf(data);
			usart1_putch(26);
			return 1;
		}
	}
	return 0;
}


/*******************************************************************************
* Function Name  : Start_Gsm
* Description    : Khoi dong GSM, sau 2 lan chua cai dat duoc thi thoat
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Start_Gsm(void)
{
			GsmDtr(ENABLE);
			Nreset(DISABLE);
			Alarm(1);
			//Buzz(1);
			StartGSM();
			Alarm(0);
			//Buzz(0);
			while(!Init_GSM())
			{
						static char k = 0;
						if(k++ > 5) break;
			}
}
