#ifndef __CONFIG_H
#define __CONFIG_H


/******************  APP1 ******************************/
/* QSPI APP1 ӳ�����ַ */
#define     QSPI_APP1_MM_BASE_ADDRESS       0x90000000UL

/* QSPI APP1 ��ӳ�����ַ*/
#define     QSPI_APP1_BASE_ADDRESS          0x00000000UL

/* SDRAM ����APP1�Ļ���ַ*/
#define     SDRAM_TARGET1_ADDR              0xC0000000UL

/* ��ȡAPP1�ļ���app1.bin */
#define     APP1_FILE_NAME      "app1.bin"
/********************************************************/

/******************  APP2   ****************************/
/* QSPI APP2 ӳ�����ַ */
#define     QSPI_APP2_MM_BASE_ADDRESS       0x90400000UL  

/* QSPI APP2 ��ӳ�����ַ */
#define     QSPI_APP2_BASE_ADDRESS          0x00400000UL  

/* SDRAM ����APP2�Ļ���ַ*/
#define     SDRAM_TARGET2_ADDR              0xC1000000UL

/* ��ȡAPP2�ļ���app2.bin */
#define     APP2_FILE_NAME       "app2.bin"
/***********************************************************/

/* SD����SDRAM���ݶԱȻ�����*/
#define     BLOCK_SIZE                1024   // 1KB

/* ************���̰���ʱ��************************/
#define     SHORT_PRESS_TIME          20   
#define     LONG_PRESS_TIME           1500  
/**************************************************/


/****************�ϵ�QSPIĬ�ϲ�������***************/
/* ������־λ QSPI��ַ(���һ������4k) */
#define     SYS_PARAMENT_ADDRESS      0x7FF000 

/* Ĭ�ϲ������� (����һ����¼��Ч) */
#define      JUMP_TIME                 0x03
#define      WHICH_APP                 0x01
#define      PARA_FLAG                 0x01
#define      DEBUG_FLAG                0x01

/* ����ϵͳ����(ע:��Ҫ����Ĭ�ϲ������ò���Ч,�����øú꿪��1�� ��¼֮��رոú��ٴ���¼ ��ֹÿ���������)*/
//#define      CLEAN_SYS_PARAMENT        

/***************************************************/
#define      AHEAD_SP        0x24
#define      AHEAD_PC        0x90
/***************************************************/
/* ��������С���ڲ�flash��*/
#define BUFFER_SIZE              1024
/* QSPIҳ��С */
#define W25Q64JV_SECTOR_SIZE     0x1000   
/***************************************************/


#endif /* __CONFIG_H */

