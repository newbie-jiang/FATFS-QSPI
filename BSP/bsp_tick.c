#include "bsp_tick.h" 


/*Hal����ʱ������װms*/
void delay_ms(uint32_t ms)
{
  HAL_Delay(ms);
}

/*��ȡϵͳʱ��ms*/
uint32_t get_tick_ms(void)
{
 return HAL_GetTick();
}







