#include "alarmtime.h"
#include "sim900_register.h"
#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#define FLASH_PAGE_SIZE         ((uint16_t)0x400)
#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08007800)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x08008000)

void FlashReadEEprom(void *Data,uint16_t write_number);
FLASH_Status FlashWriteEEprom(void *Data,uint16_t write_number);

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

void ReadMemmory(STRUCT_EEPROM_SAVE *flashv);
void SaveThayDoi(STRUCT_EEPROM_SAVE flashv,char function);