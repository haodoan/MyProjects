#include "systick.h"

volatile uint32_t sysTick_counter = 0;
void delay_ms(uint32_t nms)
{    
    uint32_t temp = sysTick_counter + nms;
	 while(sysTick_counter < temp );
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
/*
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
*/
