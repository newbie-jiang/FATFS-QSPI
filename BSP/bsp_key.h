
#ifndef __KEY_H
#define __KEY_H

#include "gpio.h"



/*  GPIO_PIN_RESET = 0U,   ����*/ 
   

 
 /*��������*/
 #define  __KEY_Port             GPIOH
 #define  __KEY_Pin              GPIO_PIN_4 
 #define  __KEY_CLK_ENABLE       do{ __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)
 
 
 /*��ȡ����״̬*/
 #define     READ_KEY()       HAL_GPIO_ReadPin(__KEY_Port, __KEY_Pin) /* ��ȡSDA */

 
 
 /*��ʼ��key*/
 void key_init(void);
 




#endif /*__KEY_H*/

