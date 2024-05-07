#ifndef __BOOT_H
#define __BOOT_H

#include "stdint.h"

typedef struct {
	uint8_t jump_time;          /* ��ת����ʱ�� */
	uint8_t which_app_running;  /* ѡ��app����  APP1/APP2*/
	uint8_t sys_parameter_flag; /* ��һ��ʹ��QSPI����־λ */
	uint8_t debug_flag;         /* ��ӡ���Կ�����־λ  1�������� 0�رյ���*/
}BOOT_SYS_config; /* ϵͳ���ò��� */



typedef struct{
uint32_t app1_size;
uint32_t app2_size;
}APP_SIZE; /* app��С(�ֽ�) */





void  jump_to_qspi_flash(void);



#endif
