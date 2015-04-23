#include <string.h>
#include <stdio.h>
#include "sim900_register.h"
#include "flasheeprom.h"
#include "interface.h"


/*************************************************************
Dang ki 
   DK    ADMIN PASSWORD                      Dang ki ADMIN
   DK    USER sdt1,sdt2,sdt3,sdt4,sdt5       Dang ki USER 
   DMK   mkcu mkmoi                          Doi mat khau
   DB                                        Xem danh ba 
   XDB   sdt1,sdt2,sdt3,sdt4,sdt5            Xoa cac so trong danh ba      
   XDB   HET                                 Xoa het cac so trong danh ba 
   TTK                                        Tra tai khoan
   NTK                                       Nap tien tai khoan
   KTTB                                      Kiem tra cac line cua thiet bi
**************************************************************/
//STRUCT_USER user[6],temp_user; 
extern SMS_GSM decodeSMS;
char function_eeprom = UNSAVE;
TIMESETUP timeonoff[2][10];
extern STRUCT_EEPROM_SAVE flashv;
//Ham get ma lenh va tham so cua tin nhan
STRUCTCOMMAND_SIM900 GetCmdDataSim900(char *str)
{
		STRUCTCOMMAND_SIM900 command_receive;
		char * pch;
 		 int i=0;		 
		 pch = strtok (str," :,=#");  
		 strcpy(command_receive.CMD,pch);
		 while (pch != NULL)
		 { 
				pch = strtok(NULL,":, #");  	
				strcpy(command_receive.DATA[i++],pch);
			  command_receive.CNT_DATA = i - 1;			
			  if(i == 6) break;          
		 }
			return command_receive;
}

// Ham Thuc thi cac ma lenh tin nhan nhan duoc cua Sim900
void SIM900_commands(char *buffer)
{
	STRUCTCOMMAND_SIM900 result;
    static char tempbuff[50];
	memset(tempbuff,0,50);
    strcpy(tempbuff,buffer);
	/*get header command*/
	result = GetCmdDataSim900(buffer);	
	if(!Register_commands(result))
		CommadProcess(timeonoff,tempbuff);
 //  CommadProcess(timeonoff,decodeSMS.contentTextSms);
}
//Ham kiem tra su ton tai cua user trong danh ba
char SearchExistUser(char *numberphone)
{
	char i = 0;
	for(i = 0;i < flashv.cnt_user;i++)
	{
		if(!strcmp(numberphone,(char*)flashv.user[i].PHONE_NO))
			return i;	
	}
	return 10;
}

//Xoa 1 so co trong danh ba
void Remove_user_contact(STRUCT_USER *src_user,char mem)
{
   char number_user;
   STRUCT_USER dst_user[6];
   if(mem == 0)//xoa tat ca
   {
      for(number_user = 1;number_user < flashv.cnt_user ;number_user++)
      {
        memset(src_user[number_user].PHONE_NO,0,11);
        memset(src_user[number_user].PASSWORD,0,10);
      }
      flashv.cnt_user = 1;//chi co admin, so user = 1
   }
   else{ // xoa 1 so
      for(number_user = 0;number_user < mem ;number_user++)
      {
         dst_user[number_user] = src_user[number_user];
      }
      for(number_user = mem ;number_user < flashv.cnt_user ;number_user++)
      {
         dst_user[number_user] = src_user[number_user + 1];
      }
      for(number_user = 0;number_user < flashv.cnt_user - 1 ;number_user++)
      {
         src_user[number_user] = dst_user[number_user];
      }
      flashv.cnt_user = flashv.cnt_user - 1;//giam user di 1
   }
}
/*Ham thuc hien cac lenh dang ki nguoi dung *************************************************************
Dang ki 
   DK    ADMIN PASSWORD                      Dang ki ADMIN
   DK    USER sdt1,sdt2,sdt3,sdt4,sdt5       Dang ki USER 
   DMK   mkcu mkmoi                          Doi mat khau
   DB                                        Xem danh ba 
   XDB   sdt1,sdt2,sdt3,sdt4,sdt5            Xoa cac so trong danh ba      
   XDB   HET                                 Xoa het cac so trong danh ba 
   TT                                        Tra tai khoan
   NTK                                       Nap tien tai khoan
*********************************************************************************************************/
char Register_commands(STRUCTCOMMAND_SIM900 cmd_data)
{
	
	char *p;
	p = cmd_data.CMD;
	function_eeprom = UNSAVE; 
	if(strcmp(p,"DK") == 0 )
	{
		if(!strcmp(cmd_data.DATA[0],"ADMIN"))
		{      
         function_eeprom = UNSAVE;   
         if(!strcmp(flashv.user[0].PHONE_NO,decodeSMS.numberPhone))
         {
            //gui tin nhan da ton tai admin
            SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban dang la ADMIN\r");
            return 1;
         }	         
         if(flashv.user[0].ADMINI == 1)
         {
            SentEnglis_SIMmsg(decodeSMS.numberPhone,"Da ton tai ADMIN ");
           //putsUART2("Da ton tai ADMIN ");
            return 1;
         }
         if(strlen(cmd_data.DATA[1]) > 10 ||strlen(cmd_data.DATA[1]) < 6)
         {  
            SentEnglis_SIMmsg(decodeSMS.numberPhone,"PASSWORD phai co do dai lon hon 6 hoac nho hon 10 ki tu.\r");
            return 1;
         }
         else{
            strcpy(flashv.user[0].PHONE_NO,decodeSMS.numberPhone);					 
            strcpy(flashv.user[0].PASSWORD,cmd_data.DATA[1]);
            SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban da dang ki thanh cong ADMIN ");
            flashv.cnt_user = 1;
            function_eeprom = SAVEMEM;
            flashv.user[0].ADMINI = 1;
						return 1;					 
            //putsUART2("Ban da dang ki thanh cong ADMIN ");
         }	
		}
		else if(!strcmp(cmd_data.DATA[0],"USER"))//dang ki cho user
		{
			char number_user;
         function_eeprom = UNSAVE;
         if(strcmp(decodeSMS.numberPhone, flashv.user[0].PHONE_NO)) // kiem tra xem co phai so admin khong?
         {
            return 1;
         }   
			for(number_user = 1;number_user < cmd_data.CNT_DATA;number_user++)// kiem tra cu phap cac so dang ki
			{
            if(strlen(cmd_data.DATA[number_user]) == 10 ||strlen(cmd_data.DATA[number_user]) == 11)
            {
               char phone_cnt;
               for(phone_cnt = 0;phone_cnt < strlen(cmd_data.DATA[number_user]);phone_cnt++)
               {
                  if(cmd_data.DATA[number_user][phone_cnt] > 0x2F && cmd_data.DATA[number_user][phone_cnt] < 0x40) //sdt trong khoang 0->9
                     continue;
                  else
                  {
                     SentEnglis_SIMmsg(decodeSMS.numberPhone,"So dien thoai khong dung,phai khong duoc chua ki tu\r");
                     //putsUART2("So dien khong thoai dung,phai khong duoc chua ki tu");
                     return 1;
                  }
               }
               strcpy(flashv.user[number_user].PHONE_NO,cmd_data.DATA[number_user]);
               strcpy(flashv.user[number_user].PASSWORD,"123456");    
               SentEnglis_SIMmsg(flashv.user[number_user].PHONE_NO,"ADMIN da dang ki cho ban vao danh ba cua thiet bi.");      
            }
            else
            {
               SentEnglis_SIMmsg(decodeSMS.numberPhone,"So dien thoai khong dung\r");
               //putsUART2("So dien thoai khong dung");
               return 1;
            }
			}
         flashv.cnt_user =  cmd_data.CNT_DATA ;
         //putsUART2("Da dang ki thanh cong cac so dien thoai");
         SentEnglis_SIMmsg(decodeSMS.numberPhone,"Da dang ki thanh cong cac so dien thoai\r");
         function_eeprom = SAVEMEM;
				 return 1;	
		}
		else
		{	
      function_eeprom = UNSAVE;
			SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban da nhan sai cu phap,Vui long kiem tra lai\r");
			return 1;
         //putsUART2("Ban da nhan sai cu phap,Vui long kiem tra lai ");
		}
	}		
	else if(!strcmp(p,"DMK"))
	{
      function_eeprom = UNSAVE;
      if(strcmp(decodeSMS.numberPhone, flashv.user[0].PHONE_NO)) // kiem tra xem co phai so admin khong?
      {
         //SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban phai la ADMIN thi moi co quyen doi mat khau");
         //putsUART2("Ban phai la ADMIN thi moi co quyen doi mat khau");         
         return 1;
      } 	
      if(strlen(cmd_data.DATA[1]) < 6 ||strlen(cmd_data.DATA[1]) > 10)
      {  
         SentEnglis_SIMmsg(decodeSMS.numberPhone,"PASSWORD phai viet lien hoac co do dai lon hon 6 hoac nho hon 10 ki tu\r");
         return 1;
      }      
      if(!strcmp(flashv.user[0].PASSWORD,cmd_data.DATA[0]))
      {
         SentEnglis_SIMmsg( flashv.user[0].PHONE_NO,"Ban da doi mat khau thanh cong\r");
         strcpy(flashv.user[0].PASSWORD,cmd_data.DATA[1]);
         function_eeprom = SAVEMEM;
      }   
      else{
         SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,"Mat khau cu khong dung\r");
      }  
			return 1;	
	}
	else if(!strcmp(p,"XDB"))
	{
      function_eeprom = UNSAVE;
      if(strcmp(decodeSMS.numberPhone, flashv.user[0].PHONE_NO)) // kiem tra xem co phai so admin khong?
      {
         //SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban phai la ADMIN thi moi co quyen xoa danh ba");
         return 1;
      } 
      if(!strcmp(flashv.user[0].PASSWORD,cmd_data.DATA[0]))
      {
         char phone_number;
         char db_vt;   
         if(!strcmp(cmd_data.DATA[1] ,"HET"))  
         {
            Remove_user_contact(flashv.user,0); // xoa dat ca danh ba user
            SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,"Da xoa het danh ba USER\n");
             function_eeprom = SAVEMEM;
            //putsUART2("Da xoa het danh ba USER\n");
            return 1;
         }         
         for(phone_number = 1; phone_number < cmd_data.CNT_DATA ;phone_number++)
         {
            db_vt = SearchExistUser(cmd_data.DATA[phone_number]);// xem sdt nam o vt nao cua danh ba
            if(db_vt == 0) //ko thuoc danh ba
						{
							 continue;
						}
						else if(db_vt == 10) //ko thuoc danh ba
            {
               //putsUART2("Khong ton tai so trong danh ba\n");
               SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,"Khong ton tai so trong danh ba\n");
               return 1 ;
            }
            else{
               //putsUART2("Ban da bi admin xoa khoi danh ba thiet bi\n");
               SentEnglis_SIMmsg(flashv.user[db_vt].PHONE_NO,"Ban da bi admin xoa khoi danh ba thiet bi\n");
               Remove_user_contact(flashv.user,db_vt);          //remove contact user  
            }   
         }
         function_eeprom = SAVEMEM;
         SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,"Da xoa thanh cong cac so vua chon trong danh ba ");
				 return 1;
         //putsUART2("Da xoa thanh cong cac so vua chon trong danh ba ");
      }
      else
      {
          function_eeprom = UNSAVE;
          SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,"Sai mat khau \n");  
          //putsUART2("Sai mat khau \n");  
          return 1;

      }
	}
   else if(!strcmp(p,"DB"))
   {  
      char phone_number;
      char db[100];
      function_eeprom = UNSAVE;
      if(strcmp(decodeSMS.numberPhone, flashv.user[0].PHONE_NO)) // kiem tra xem co phai so admin khong?
      {
         //SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban phai la ADMIN thi moi co quyen xem danh ba");
         return 1;
      }       
      if(flashv.cnt_user == 1 )
      {
         SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,"Danh ba trong\n");  
         //putsUART2("Danh ba trong\n");
         return 1;
      }
      strcpy(db,"Cac so trong Danh Ba :");
      for(phone_number = 0; phone_number < flashv.cnt_user;phone_number++)
      {
         strcat(db,"\r\n");
         strcat(db,flashv.user[phone_number].PHONE_NO);
      }
         strcat(db,"\r\n");
         SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,db);
         //putsUART2(db);
         memset(db,0,100);
				return 1;

   }
   else if(!strcmp(p,"TADMIN"))
   {
      char temp_mess[160];
      function_eeprom = UNSAVE;
      if(!strcmp(flashv.user[0].PASSWORD,cmd_data.DATA[0]))
      {
         sprintf(temp_mess,"Ban vua chuyen quyen ADMIN den so %s thanh cong\n",decodeSMS.numberPhone);
         SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,temp_mess);
         strcpy(flashv.user[0].PASSWORD,"123456");
         strcpy(flashv.user[0].PHONE_NO,decodeSMS.numberPhone);
         SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban da thay doi ADMIN thanh cong\n");
         function_eeprom = SAVEMEM;
      }  
      else{
         function_eeprom = UNSAVE;
         SentEnglis_SIMmsg(flashv.user[0].PHONE_NO,"Mat khau khong dung\r");
      }     
			return 1;
   }   
   else if(!strcmp(p,"TTK"))
   {
      CheckUsd();
      return 1;
   }
   else if(!strcmp(p,"NTK"))
   {
      Naptien(cmd_data.DATA[0]);
      return 1;
   } 
   else
   {
			//SentEnglis_SIMmsg(decodeSMS.numberPhone,"Ban da nhan sai cu phap, vui long kiem tra lai\n");
      return 0;
   }
}

