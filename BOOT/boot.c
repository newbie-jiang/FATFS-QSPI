#include "boot.h"

#include "bsp_qspi.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_sd.h" 
#include "bsp_sdram.h" 
#include "bsp_key.h" 

#include "config.h" 
#include <string.h>




  

 void jump_app(uint32_t base_address);
 void display_logo(void);
 int read_app_and_jump_running(void);
 void get_parament_which_boot_start(uint8_t time, uint8_t which_app, uint8_t debug);
 int which_jump_to_app1_or_app2(uint8_t para_flag);
 
 int Read_app_in_sd_card(void);



 typedef  void (*pFunction)(void);
 pFunction JumpToApplication;

// FILINFO fileInfo;
 APP_SIZE Sapp_size={0,0};
 APP_SIZE * Papp_size = &Sapp_size;

 BOOT_SYS_config sys_parameter;
 BOOT_SYS_config * psys_parameter = &sys_parameter;	
 
 
 
 
 

void  jump_to_qspi_flash(void)
{
	read_app_and_jump_running();
}



/* �Է���һ ���������qspiʱ�� */
void enable_qspi_clk(void)
{
  __HAL_RCC_QSPI_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
	
  __HAL_RCC_FMC_CLK_ENABLE(); /* �������ˣ����ʱ�ӱ��뿪 ûϸ�����ʱ����QSPI��ϵ  */
}






/* ȷ��һ���ɾ���bootloader ������Ҫ���ʼ���õ������������Լ��ж�  ֻ����QSPI����APP�оͲ���Ҫ��QSPI�� */
/* �ܿ�: �ڽ��ʼ����ʱ��ע�ⲻҪ��QSPI��ʱ��Ҳ���ʼ���� �е�������QSPIһ��ʱ�ӣ����ʼ��֮��Ͳ�������
   ������������������ʼ��֮�� �ٶ�����QSPI�Ƿ����� */
void DeInit_all_bsp_and_IRQ(void)
{ 
   SCB_DisableICache();		// �ر�ICache
	 SCB_DisableDCache();		// �ر�Dcache
 	
	 SysTick->CTRL = 0;		// �ر�SysTick
	 SysTick->LOAD = 0;		// ��������ֵ
	 SysTick->VAL = 0;		// �������ֵ
	
	/* �ر������жϣ���������жϹ����־ */
    for (int i = 0; i < 8; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    } 
	
	
//	 /* ���ʼ�� SD�� */
//	 HAL_SD_MspDeInit(&hsd1);
		
		
	 /* ���ʼ�� SDRAM */
	 HAL_SDRAM_MspDeInit(&hsdram1);
		
	 
	 /*���ʼ��LED*/ 
	 HAL_GPIO_DeInit(LED_R_GPIO_Port,LED_R_Pin);
	 HAL_GPIO_DeInit(LED_B_GPIO_Port,LED_R_Pin);
		
	 /*���ʼ�� SD_CAP*/
	 HAL_GPIO_DeInit(SD_CAP_GPIO_Port,SD_CAP_Pin);
		
		
	 /*���ʼ��usart */
   HAL_UART_MspDeInit(&huart4);
	 
	
	/*��������ʱ��Ĭ��״̬�ָ�HSI*/
	 HAL_RCC_DeInit();
	 
	 /* Ϊ��ֹqspiʱ�� ����ʼ�� */
	 enable_qspi_clk();
}


 
 

/* �������ܣ���ת�� app */
void jump_app(uint32_t base_address)
{
//    pFunction JumpToApplication;

    /* ȡ����ʼ������������ж� */
    DeInit_all_bsp_and_IRQ();
    
    /* ȷ��CPU����Ӧ�κ��жϡ�ע�⣺��Ӧ�ó����У�����Ҫ�ֶ�ʹ�� __enable_irq(); �������ж� */
    __disable_irq();
    
    /* ������ת��ַ */
    JumpToApplication = (pFunction) (*(__IO uint32_t*) (base_address + 4));
    
    /* ��������ջָ�� */
    __set_MSP(*(__IO uint32_t*) base_address);
    
    /* ִ����ת */
    JumpToApplication();
}

 

 /*     logo ������վ��http://patorjk.com/software/taag/       */
 
 void display_logo(void)
{                                                                                                                                                                  
  printf("'  BBBBBBBBBBBBBBBBB        OOOOOOOOO          OOOOOOOOO     TTTTTTTTTTTTTTTTTTTTTTT\r\n");
  printf("'  B::::::::::::::::B     OO:::::::::OO      OO:::::::::OO   T:::::::::::::::::::::T\r\n");
  printf("'  B::::::BBBBBB:::::B  OO:::::::::::::OO  OO:::::::::::::OO T:::::::::::::::::::::T\r\n");
  printf("'  BB:::::B     B:::::BO:::::::OOO:::::::OO:::::::OOO:::::::OT:::::TT:::::::TT:::::T\r\n");
  printf("'    B::::B     B:::::BO::::::O   O::::::OO::::::O   O::::::OTTTTTT  T:::::T  TTTTTT\r\n");
  printf("'    B::::B     B:::::BO:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B::::BBBBBB:::::B O:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B:::::::::::::BB  O:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B::::BBBBBB:::::B O:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B::::B     B:::::BO:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B::::B     B:::::BO:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B::::B     B:::::BO::::::O   O::::::OO::::::O   O::::::O        T:::::T        \r\n");
  printf("'  BB:::::BBBBBB::::::BO:::::::OOO:::::::OO:::::::OOO:::::::O      TT:::::::TT      \r\n");
  printf("'  B:::::::::::::::::B  OO:::::::::::::OO  OO:::::::::::::OO       T:::::::::T      \r\n");
  printf("'  B::::::::::::::::B     OO:::::::::OO      OO:::::::::OO         T:::::::::T      \r\n");
  printf("'                                                                                   \r\n");
	printf("'                                                                                   \r\n");
	printf("'  www.newbie.pics                                                                  \r\n");
	printf("'                                                                                   \r\n");
	printf("'                                                                                   \r\n");
	printf("'                                                                                   \r\n");
	printf("'                                                                                   \r\n");
}



/*��SD���ݶ�ȡ��sdram
  ��SD���ݶ�ȡ��1k����,��sdram��������ÿ���ֽڽ���У��  ѭ����ȡ���ֽ�У��
*/

/* 
 * ѡȡapp��ȡ�� sdram 
 * 1k �������ֽ�У��
 */
int read_sd_data_to_sdram(uint8_t app_which)
{
//	  FRESULT res;
//    uint32_t bytesRead;
//	  
//	
//	  if(app_which==1) /* ѡ��app1 */
//	  {
//		     uint8_t* sdram_ptr1 = (uint8_t*)SDRAM_TARGET1_ADDR;  // ָ��SDRAM��ָ��
//         uint8_t  temp_buffer1[BLOCK_SIZE];  // ��ʱ������
//		 // ��ȡ�ļ���Ϣ
//				res = f_stat(APP1_FILE_NAME, &fileInfo);
//				if (res != FR_OK)
//				{
//						printf("  Error getting file info.\r\n"); /* �ļ�������*/
//						return -1;
//				}
//				else
//				{
//					  Sapp_size.app1_size=fileInfo.fsize;
//						printf("  The size of the file  %s is %ld bytes.\r\n", APP1_FILE_NAME,fileInfo.fsize); /*��ȡ�ļ���С*/
//				}

//				// ��ȡ�����ļ���SDRAM
//				bytesRead = readFileContent(APP1_FILE_NAME, sdram_ptr1, fileInfo.fsize+1, 0);
//				
//				if (bytesRead != fileInfo.fsize)
//				{
//						printf("  Error reading the entire file.\r\n");
//						return -1;
//				}

//				// ��1KB�Ŀ��С����У��
//				for (uint32_t i = 0; i < fileInfo.fsize; i += BLOCK_SIZE)
//				{
//						// ���ļ��ж�ȡ1KB�Ŀ鵽��ʱ������
//						bytesRead = readFileContent(APP1_FILE_NAME, temp_buffer1, BLOCK_SIZE, i);
//						if (bytesRead != BLOCK_SIZE && i + bytesRead != fileInfo.fsize)
//						{
//								printf("  Error reading the file block to temp buffer.\r\n");
//								return -1;
//						}

//						// �Ƚ�SDRAM�еĿ����ʱ�������е�����
//						if (memcmp(sdram_ptr1 + i, temp_buffer1, bytesRead) != 0)
//						{
//								printf("  Data mismatch detected at block starting from byte %d.\r\n", i);
//								return -1;
//						}
//				}

//				  printf("  Data in SDRAM check the file content success.\r\n");
//				  return 0;
//	 }else if(app_which==2){
//		
//		     uint8_t* sdram_ptr2 = (uint8_t*)SDRAM_TARGET2_ADDR;  // ָ��SDRAM��ָ��
//         uint8_t  temp_buffer2[BLOCK_SIZE];  // ��ʱ������
//	
//			 // ��ȡ�ļ���Ϣ
//				res = f_stat(APP2_FILE_NAME, &fileInfo);
//				if (res != FR_OK)
//				{
//						printf("  Error getting file info.\r\n"); /*�ļ�������*/
//						return -1;
//				}
//				else
//				{   
//					  Sapp_size.app2_size=fileInfo.fsize;
//						printf("  The size of the file  %s is %ld bytes.\r\n", APP2_FILE_NAME,fileInfo.fsize); /*��ȡ�ļ���С*/
//				}

//				// ��ȡ�����ļ���SDRAM
//				bytesRead = readFileContent(APP2_FILE_NAME, sdram_ptr2, fileInfo.fsize+1, 0);
//				
//				if (bytesRead != fileInfo.fsize)
//				{
//						printf("  Error reading the entire file.\r\n");
//						return -1;
//				}

//				// ��1KB�Ŀ��С����У��
//				for (uint32_t i = 0; i < fileInfo.fsize; i += BLOCK_SIZE)
//				{
//						// ���ļ��ж�ȡ1KB�Ŀ鵽��ʱ������
//						bytesRead = readFileContent(APP2_FILE_NAME, temp_buffer2, BLOCK_SIZE, i);
//						if (bytesRead != BLOCK_SIZE && i + bytesRead != fileInfo.fsize)
//						{
//								printf("  Error reading the file block to temp buffer.\r\n");
//								return -1;
//						}

//						// �Ƚ�SDRAM�еĿ����ʱ�������е�����
//						if (memcmp(sdram_ptr2 + i, temp_buffer2, bytesRead) != 0){
//						    printf("  Data mismatch detected at block starting from byte %d.\r\n", i);
//								return -1;
//						  }
//				   }
//				   printf("  Data in SDRAM check the file content success.\r\n");
			return 0;
//	  }
//    
//	return -1;
}






/*
 *��ȡ�̼���С�����ݹ̼���С��������
 *��sdram������д��qspi flash 
 *������ѡ 1/2
 */
int write_sdram_data_to_qspi_flash(uint8_t which_app)
{
    if (which_app == 1) {
        uint8_t* sdram_data_ptr1 = (uint8_t*)SDRAM_TARGET1_ADDR;
			  uint32_t remaining_bytes1 = Papp_size->app1_size; /* APP1��С(�ֽ�)*/
       
			 /************************************��������(��4k����)***************************************/
			  uint32_t start_sector_app1 = QSPI_APP1_BASE_ADDRESS; 
        uint32_t end_sector = (remaining_bytes1 + W25Q64JV_SECTOR_SIZE - 1) / W25Q64JV_SECTOR_SIZE - 1;
        uint32_t sectors_need_Erase_num = end_sector - start_sector_app1 + 1;

        printf(" app1 fileInfo.fsize=%d\r\n", remaining_bytes1);
        printf(" app1 sectors need Erase num = %04d\r\n", sectors_need_Erase_num);

        for (uint32_t i = start_sector_app1; i < sectors_need_Erase_num; i++) {
            if (QSPI_W25Qxx_SectorErase(i * W25Q64JV_SECTOR_SIZE) != QSPI_W25Qxx_OK) {
                printf(" qspi erase err!!!\r\n");
                return -1;
            }
            printf(" qspi erase %d sectors success!!!\r\n", i);
        }
				/******************************************************************************************/
				

        if (QSPI_W25Qxx_WriteBuffer(sdram_data_ptr1, 0x0, remaining_bytes1) != QSPI_W25Qxx_OK) {
            return -1;
        }
    } else if (which_app == 2) {
			
        uint8_t* sdram_data_ptr2 = (uint8_t*)SDRAM_TARGET2_ADDR;
        uint32_t remaining_bytes2 = Papp_size->app2_size;
        /************************************��������(��4k����)***************************************/
        uint32_t start_sector_app2 = QSPI_APP2_BASE_ADDRESS / W25Q64JV_SECTOR_SIZE;
        uint32_t end_sector = start_sector_app2 + (remaining_bytes2 + W25Q64JV_SECTOR_SIZE - 1) / W25Q64JV_SECTOR_SIZE - 1;
        uint32_t sectors_need_Erase_num = end_sector - start_sector_app2 + 1;

        printf(" app2 fileInfo.fsize=%d\r\n", remaining_bytes2);
        printf(" app2 sectors_need_Erase_num = %04d\r\n", sectors_need_Erase_num);

        for (uint32_t i = start_sector_app2; i <= end_sector; i++) {
            if (QSPI_W25Qxx_SectorErase(i * W25Q64JV_SECTOR_SIZE) != QSPI_W25Qxx_OK) {
                printf(" qspi erase err at sector %d!!!\r\n", i);
                return -1;
            }
            printf(" qspi erase sector %d success!!!\r\n", i);
        }
        /******************************************************************************************/
				
        if (QSPI_W25Qxx_WriteBuffer(sdram_data_ptr2, QSPI_APP2_BASE_ADDRESS, remaining_bytes2) != QSPI_W25Qxx_OK) {
            return -1;
        }
    } else {
			 
        return -1; // ��������
    }

    return 0;
}

/* ��֤QSPI Flash�е�����
 * ������ѡ 1/2
 * ����ֵ 0 :success    -1 :err
 */
int verify_qspi_data(uint8_t which_app)
{
    if (which_app == 1) {
        uint8_t* sdram_data_ptr1 = (uint8_t*)SDRAM_TARGET1_ADDR;
        uint8_t* qspi_data_ptr1 = (uint8_t*)QSPI_APP1_MM_BASE_ADDRESS;
        uint32_t remaining_bytes1 = Papp_size->app1_size;

        if (memcmp(sdram_data_ptr1, qspi_data_ptr1, remaining_bytes1) != 0) {
            printf(" app1 qspi memorymap data check err!!!\r\n");
            return -1;
        }
        printf(" app1 qspi memorymap data check success!!!\r\n");
				return 0;
    } else if (which_app == 2) {
        uint8_t* sdram_data_ptr2 = (uint8_t*)SDRAM_TARGET2_ADDR;
        uint8_t* qspi_data_ptr2 = (uint8_t*)QSPI_APP2_MM_BASE_ADDRESS;
        uint32_t remaining_bytes2 = Papp_size->app2_size;

        if (memcmp(sdram_data_ptr2, qspi_data_ptr2, remaining_bytes2) != 0) {
            printf(" app2 qspi memorymap data check err!!!\r\n");
            return -1;
        }
        printf(" app2 qspi memorymap data check success!!!\r\n");
				return 0;
    } else {
        return -1; // ��������
    }

    return 0;
}

	


	

/*  ���qspi���Ƿ���app ����ֻ��� ǰ�˸��ֽ����Ƿ������Ƶ�sp��pc����  
     �ϵ��ĵ�һ�����ֽ�Ϊsp��ջ�׵�ַ��һ��Ϊ0x2000 xxxx   ����ʶ��0x20����ֽ�   
	   ������С�� QSPI�е����ݴ洢��ʽΪ0x xxxx 0020 ��������ȥ�����ĸ��ֽڣ��ҵ�spΪ0x24000000
	   �������жϻ���ַƫ�����ֽ�����Ϊ0x24����   pcһ����0x9000 xxxx  ������ַƫ�ư��ֽ�����
	   
	   *((base_add+4)==0x24)&&(*(base_add+8)==0x90) ���ж�Ϊapp����
*/ 
/* ���QSPI �� �Ƿ����app1 app2
   app1����  return   1 
   app2����  return   2
   ˫app���� return   3
   ��app���� return  -1
*/

/* ���QSPI �� app1 app2�Ƿ���� */
int check_qspi_app_exist(void){
 
  uint8_t app1_sp = AHEAD_SP;  
	uint8_t app1_pc = AHEAD_PC;
	uint8_t app2_sp = AHEAD_SP;  
	uint8_t app2_pc = AHEAD_PC;
	
	uint8_t app1_check_buffer[8];
	uint8_t app2_check_buffer[8];
	uint8_t app1_flag=0,app2_flag=0;  /* app���ڱ�־λ 0��������   1������ */
	

	QSPI_W25Qxx_ReadBuffer(app1_check_buffer, QSPI_APP1_BASE_ADDRESS, sizeof(app1_check_buffer));
	QSPI_W25Qxx_ReadBuffer(app2_check_buffer, QSPI_APP2_BASE_ADDRESS, sizeof(app2_check_buffer));
	
	/* ���app1�Ƿ���� */
	if((app1_check_buffer[3]==app1_sp)&&(app1_check_buffer[7]==app1_pc)){
		app1_flag = 1;
	}
	/* ���app2�Ƿ����*/
	if((app2_check_buffer[3]==app2_sp)&&(app2_check_buffer[7]==app2_pc)){
		app2_flag = 1;
  }
	
	 
	 if (app1_flag && app2_flag) {
    printf(" QSPI EXIST APP1 and APP2 \r\n"); /* ����app������ */
    return 3; //˫app����
    } else if (app1_flag) {
    printf(" QSPI EXIST APP1 \r\n"); /* ֻ��app1���� */
    return 1;
    } else if (app2_flag) {
    printf(" QSPI EXIST APP2 \r\n"); /* ֻ��app2���� */
    return 2;
    } else {
    printf(" No APP found in QSPI \r\n"); /* û��app���� */
    return -1;
    }
		
		return -1;
	
}








/* 
 * �ϵ�ϵͳ�������� 
 * δ��ȡ��������д�루�����ڵ�һ�ν����ò���д��QSPI��
 * ͬ��ϵͳ������ sys_parameter�ṹ��
 * ����ֵ��  �ɹ���ȡ���������أ�0    ���� -1
 */
int  sys_parameter_config (void) 
{
	/* buffer[0]��תʱ��  buffer[1]APPѡ��  buffer[2]������־λ   buffer[3]debug��־λ */
	uint8_t read_sys_parament_buffer[4];  
	
	/* ϵͳ�ϵ�Ĭ�ϲ��� */
	sys_parameter.jump_time=JUMP_TIME; /* 3s */
	sys_parameter.which_app_running = WHICH_APP;  /* 1:app1  2:app2*/
	sys_parameter.sys_parameter_flag =PARA_FLAG;  /* ���ò�����־λ1Ϊ���ڱ�־λ��0Ϊ�����ڱ�־λ */
	sys_parameter.debug_flag = DEBUG_FLAG;
	
	// ��ȡָ��ṹ����ֽ�ָ��
  uint8_t* psys_parameter = (uint8_t*)&sys_parameter;
	
	
	/* ���ϵͳ���ò��� ��ע���ú꿪�ؽ�ʹ��һ�Σ�ʹ�ú�Ӧ�رգ�*/
	#ifdef CLEAN_SYS_PARAMENT
	   if(QSPI_W25Qxx_SectorErase(SYS_PARAMENT_ADDRESS)!=QSPI_W25Qxx_OK){
			    printf(" Erase QSPI Sector parament err\r\n ");
		      return -1;
		 }else{
		       printf(" active Erase QSPI Sector parament success\r\n ");
			     printf(" warning : please close  #define CLEAN_SYS_PARAMENT\r\n ");
		      }
	#endif
	
	/* ��ȡ������־λ ȷ���Ƿ�������ò��� */
	if(QSPI_W25Qxx_ReadBuffer
		         (read_sys_parament_buffer, 
	              SYS_PARAMENT_ADDRESS, 
	                sizeof(read_sys_parament_buffer))
								    !=QSPI_W25Qxx_OK)
  {
		printf(" read QSPI system parament err\r\n ");
		return -1;
	}
	
//	for(uint8_t i=0;i<4;i++)	{
//	 printf(" read_sys_parament_buffer[%d]=%02x\r\n",i,read_sys_parament_buffer[i]);
//	 
//	}
	
	if(read_sys_parament_buffer[2]!=PARA_FLAG){ /* ������ϵͳ�������� */
		  printf(" not found system parament flag \r\n");
		
		 /* ���� ���һ������*/
		 if(QSPI_W25Qxx_SectorErase(SYS_PARAMENT_ADDRESS)!=QSPI_W25Qxx_OK){
			 printf(" Erase QSPI Sector parament err\r\n ");
		   return -1;
		 }
	 
	    //         д������       (�ṹ�����ò���)    д���ַ        ���ݴ�С(�ֽ�)
		 if(QSPI_W25Qxx_WriteBuffer(psys_parameter, SYS_PARAMENT_ADDRESS, sizeof(sys_parameter))!=QSPI_W25Qxx_OK){
			 printf(" Write QSPI Sector parament err\r\n ");
		   return -1;
		 }else{
		   printf(" will Write QSPI Sector parament success\r\n ");
		 }
		 
			 for(uint8_t i=0;i<4;i++)	{
		       printf(" sys_parament_buffer[%d]=%02x\r\n",i,psys_parameter[i]);
	    }
			
		}else{  /* ��ȡ��ϵͳ���ò��� */
	
	
		/* QSPI�ж�ȡ���Ĳ���ͬ�����ṹ��  ��������ֱ�ӷ��ʽṹ���ȡ����*/
					sys_parameter.jump_time=read_sys_parament_buffer[0];
				sys_parameter.which_app_running = read_sys_parament_buffer[1];  
			sys_parameter.sys_parameter_flag =read_sys_parament_buffer[2];  
		sys_parameter.debug_flag = read_sys_parament_buffer[3];
	
		/* ��ӡ���ò��� */	
		printf("**************************SYS   PARAMENT******************************************\r\n");
			printf("sys_parameter.jump_time = 0x%02x\r\n",sys_parameter.jump_time);
				printf("sys_parameter.which_app_running = 0x%02x\r\n",sys_parameter.which_app_running);
					printf("sys_parameter.sys_parameter_flag = 0x%02x\r\n",sys_parameter.sys_parameter_flag);
						printf("sys_parameter.debug_flag = 0x%02x\r\n",sys_parameter.debug_flag);
							printf("******************************************************************************\r\n\r\n");                                     
		            return 0;
	     }
		
	return 0;
  
}
	


/* ѡ��ֱ����ת��APP �����øú���֮ǰ���������ݺϷ��Լ�飩
 * ������ѡ1/2
 *
 * ʧ�ܷ��� -1
 */
int app_exist_which_app(uint8_t para_flag)
{
  
	 if(QSPI_W25Qxx_MemoryMappedMode()!=QSPI_W25Qxx_OK) {
	  printf(" enable QSPI memory map mode err\r\n");
		 return -1;
	 }
	 
	  printf(" enable QSPI memory map mode success\r\n");
	  display_logo();
	 if(para_flag==1){
	    jump_app(QSPI_APP1_MM_BASE_ADDRESS);
	 }
	 else if(para_flag==2){
	   jump_app(QSPI_APP2_MM_BASE_ADDRESS);
	 }else{
	 
	 /* ��д�������� */
	 }
   return -1;
}






/* �̰����� : 
 *
 * APP1��APP2֮���л�
 *
 */
 int HandleShortPress(void)
{ 

//  ����QSPI�ϵĹ̼�
//	if(QSPI_W25Qxx_BlockErase_32K(0)==QSPI_W25Qxx_OK){
//	
//	printf(" Erase QSPI Sector parament success\r\n ");
//	}
//	if(QSPI_W25Qxx_BlockErase_32K(0x400000)==QSPI_W25Qxx_OK){
//	
//	printf(" Erase QSPI Sector parament success\r\n ");
//	}
	
 
	BOOT_SYS_config* psys_parameter = &sys_parameter ;
	uint8_t read_buffer[4];
	
	
	/* �ж��Ƿ�������APP���� */
	int app_exist_state = check_qspi_app_exist();/* �ϵ���APP�Ƿ���� �Լ�У�� */
	
	/* ֻ��һ��APP�˳� */
	if(app_exist_state!=3){
	  printf(" err Only one app exists\r\n");
	  return -1;
	}
	/* sys parament ��ȡ�������� read_buffer */
  if(QSPI_W25Qxx_ReadBuffer
		                    (read_buffer, 
	                           SYS_PARAMENT_ADDRESS, 
	                                 sizeof(read_buffer) )
								                         !=QSPI_W25Qxx_OK)
      {
		   printf(" read QSPI parament err\r\n ");
		   return -1;
	    }else{
			 /* ����ϵͳ�������� ��д���ݲ�д�� */
			    if(QSPI_W25Qxx_SectorErase(SYS_PARAMENT_ADDRESS)!=QSPI_W25Qxx_OK){
			    printf(" Erase QSPI Sector parament err\r\n ");
		      return -1;
		     }
			/* ��д���� ��д�� read_buffer[1]  appѡ���ֽ�*/
				 
      if(read_buffer[1]==1){
		      read_buffer[1]=2;
		  }else if(read_buffer[1]==2){
		      read_buffer[1]=1;
		     }
			
			 if(QSPI_W25Qxx_WriteBuffer(read_buffer,SYS_PARAMENT_ADDRESS,sizeof(read_buffer))!=QSPI_W25Qxx_OK){
				  printf(" QSPI Write sys parament err\r\n ");
				 }
			 
			  /* ͬ�����ṹ�� */
				 psys_parameter->jump_time = read_buffer[0];
				 psys_parameter->which_app_running = read_buffer[1];
				 psys_parameter->sys_parameter_flag = read_buffer[2];
				 psys_parameter->debug_flag = read_buffer[3];
				 
				
				 app_exist_which_app(psys_parameter->which_app_running); /* �ʼ������Ϸ��ԣ�ֱ����ת */
	 		
      }
	
	 
	     printf(" system which app parament err: %02x",psys_parameter->which_app_running);
		   for( ; ; ){
		              LEDR_TOGGLE();
		              delay_ms(300);
		             }

}



	

/* �������� ��������SD���̼� */
int HandleLongPress(void)
{

  if(sys_parameter_config()!=0){ /* ��ȡ/д�� ϵͳ���� */
					 return -1; 
	        }
	
	uint8_t sys_which_app_update = psys_parameter->which_app_running;
	int sd_Firmware_check = Read_app_in_sd_card();
		
	/* ���SD�������й̼� �����QSPI���Ƿ���ڹ̼������� */
 	  if(sd_Firmware_check>=1&&sd_Firmware_check<=3){
		    printf("  Firmware found on SD card\r\n "); /* ��⵽�Ϸ��̼� */
			  printf(" long key : now will update firmware\r\n");
		}else{
        printf("  Firmware not found on SD card\r\n ");
		    return -1;
		} 
	


	
	switch(sys_which_app_update)
	{
	  case 1:
			    which_jump_to_app1_or_app2(sys_which_app_update); /* ��ת��app1ִ�� */
		      break;
		case 2:
			    which_jump_to_app1_or_app2(sys_which_app_update); /* ��ת��app2ִ�� */
			    break;
		default:
			    
			   for( ; ; ){
		   	            LEDR_TOGGLE();    /* ϵͳ�������� */
			              printf("  sys_parameter err: %02x",psys_parameter->which_app_running);
		                delay_ms(300);
		               }
	        break;

	}
	
	return -1;
}
	
	




 
/* ���sd����app�Ƿ���� 
 * return 1   app1�����ҺϷ�
 * return 2   app2�����ҺϷ�
 * return 3   ˫App�����ҺϷ�
 * return 4   δ�ҵ�APP
*/
int Read_app_in_sd_card(void)
{
//	uint8_t app1_sp = AHEAD_SP;  
//	uint8_t app1_pc = AHEAD_PC;
//	uint8_t app2_sp = AHEAD_SP;  
//	uint8_t app2_pc = AHEAD_PC;
//	
//	uint8_t app1_check_buffer[8];
//	uint8_t app2_check_buffer[8];
//	
//	uint8_t app1_name_flag=0,app2_name_flag=0;    /* app�ļ������ڱ�־λ    0:������  1:����*/
//	uint8_t app1_check_flag=0,app2_check_flag=0;  /* app�ļ�У��Ϸ���־λ                  */
//	
//	int bytesRead=0; /* sd��ȡ�ֽ���*/
//	int res;
//	
//	mount_sd(); /* ����ʧ��ֹͣ*/

//	 /* ����sd����app1�ļ����Ƿ���� */
//	 res = f_stat(APP1_FILE_NAME, &fileInfo);
//				if (res != FR_OK){
//				      printf(" APP1 not found in SD card \r\n"); 
//				}else{
//				      app1_name_flag=1; 
//				     }  
//	 /* ����sd����app2�ļ����Ƿ���� */		
//	 res = f_stat(APP2_FILE_NAME, &fileInfo);
//				if (res != FR_OK){
//				      printf(" APP2 not found in SD card \r\n");	
//				}else{
//              app2_name_flag=1;
//				     }
//	
//				if(!(app1_name_flag+app2_name_flag)){
//             return 4;  /* APP�������� */
//				}					
//				
//			
//				
//				
//	
//	if(app1_name_flag) /* ��ȡ��APP1�ļ��� ���app1���ݺϷ��� */
//	{
//	      /* ��ȡapp1ǰ�˸��ֽڵ������� */
//				bytesRead = readFileContent(APP1_FILE_NAME, app1_check_buffer, sizeof(app1_check_buffer), 0);
//				if (bytesRead != sizeof(app1_check_buffer))/* ��ȡ�ֽ�����һ��*/
//				{
//						printf("  Error reading the APP1 check file\r\n");
//					  
//				}else{
//				           
//				        if((app1_check_buffer[3]==app1_sp)&&(app1_check_buffer[7]==app1_pc)){ /* У��app1�Ϸ��� */
//		                app1_check_flag = 1; /* APP1 �����ҺϷ� */
//		            }else{
//								      printf("  APP1 exists but is invalid\r\n");  /* APP1���ڵ����Ϸ� */
//								}
//								
//	           }			
//	}
//	
//	if(app2_name_flag) /* ��ȡ��APP2�ļ��� ���app���ݺϷ��� */
//	{
//		       /* ��ȡapp2ǰ�˸��ֽڵ������� */
//				bytesRead = readFileContent(APP2_FILE_NAME, app2_check_buffer, sizeof(app2_check_buffer), 0);
//				if (bytesRead != sizeof(app2_check_buffer))/* ��ȡ�ֽ�����һ��*/
//				{
//						printf("  Error reading the APP2 check file\r\n");
//					  
//				}else{
//				        /* У��app2�Ϸ��� */
//				        if((app2_check_buffer[3]==app2_sp)&&(app2_check_buffer[7]==app2_pc)){
//		                app2_check_flag = 1; /*APP2 �����ҺϷ�*/
//		            }else{
//								      printf("  APP2 exists but is invalid\r\n"); 
//								     }									
//								
//	           }			
//	     
//	} 
//	
//	
//	 
//	 if(app1_check_flag&&app2_check_flag) {
//		 printf("  SD check APP1 and APP2 success\r\n" ); /* ����app�������ҺϷ� */
//		 return 3; 
//	 }else if(app2_check_flag){
//		 printf("  SD check  APP2 success\r\n");/* ֻ��app2�Ϸ� */
//	   return 2;
//	 }else if(app1_check_flag){
//		 printf("  SD check  APP1 success\r\n"); /* ֻ��app1�Ϸ�*/
//	   return 1;
//	 }else{
//		 
//	    /* ���ڲ��Ϸ�APP */
//	 }
//	 
	 return 0; 
//	 
}




void CheckButtonPress(void)
{
    static uint32_t pressTime = 0;        // ���ڱ��水ť������ʱ��ʱ��ı���
    static uint8_t buttonState = 0;       // ���ڱ��水ť״̬�ı�����0��ʾ�ͷţ�1��ʾ���£�
    static uint8_t longPressDetected = 0; // ���ڸ����Ƿ��⵽�����ı���

    if (READ_KEY() == GPIO_PIN_RESET)  // ��ť������
    {
        if (buttonState == 0)          // ��ť�ոձ�����
        {
            buttonState = 1;           // ���°�ť״̬Ϊ����
            pressTime = get_tick_ms(); // ��¼��ť������ʱ��ʱ��
            longPressDetected = 0;     // ���ó�������־
        }
    }
    else  // ��ť���ͷ�
    {
        if (buttonState == 1)  // ��ť�ոձ��ͷ�
        {
            buttonState = 0;  // ���°�ť״̬Ϊ�ͷ�
            uint32_t releaseTime = get_tick_ms();  // ��ȡ��ť���ͷ�ʱ��ʱ��
            uint32_t duration = releaseTime - pressTime;  // ���㰴ť�����µĳ���ʱ��

            if (duration >= LONG_PRESS_TIME)
            {
                HandleLongPress();// ��⵽����
                longPressDetected = 1;  // ���ñ�־��ָʾ�Ѽ�⵽����
            }
            else if (duration >= SHORT_PRESS_TIME && !longPressDetected)
            {
                HandleShortPress();// ��⵽�̰�
            }
        }
    }

    delay_ms(1);  // ȥ����ʱ
}



/* ���ݲ���ѡ����һ��app����  
 * ������   1/2
 */
int which_jump_to_app1_or_app2(uint8_t para_flag)
{
	
	  printf("  get system parament = 0x%0x\r\n",para_flag);
	
   if(para_flag==1){  /* ��ת��app1 */
		 
	  /* ������������Щ ����  �ҵı�����δ��ȡ�����ò����Ͳ���ʼ����Щ���� */
		 
//		     bsp_sd_init(); /* ����SD ��ʧ�ܻ�ִֹͣ�� */
		     SDRAM_InitSequence(); /* ��ʼ��sdram */  
		     
					if(read_sd_data_to_sdram(1)==0){ /* �Ƚ�sd��sdram����һ�� */
						 if(write_sdram_data_to_qspi_flash(1)==0){ /* ��sdram������д��qspi �����ڴ�ӳ��ģʽ �Ƚ���sdram����һ�� */
							     QSPI_W25Qxx_MemoryMappedMode(); 
					         verify_qspi_data(1);
							     printf(" Update APP1 success!!!\r\n");
								   display_logo();
	                 jump_app(QSPI_APP1_MM_BASE_ADDRESS);
						    }
					    /*SDRAM �� QSPI���ݲ�һ�� */
							printf("APP1 SDRAM compare QSPI data different err\r\n");
						}
		        /*SD �� SDRAM���ݲ�һ�� */
		      printf("APP1 SD compare SDRAM data different err\r\n");
						
						
	   }else if(para_flag==2){ /* ��ת��app2���� */
					
//					  bsp_sd_init(); /* ����SD ��ʧ�ܻ�ִֹͣ�� */
						SDRAM_InitSequence(); /* ��ʼ��sdram */
		 
							if(read_sd_data_to_sdram(2)==0){ /* �Ƚ�sd��sdram����һ�� */
								 if(write_sdram_data_to_qspi_flash(2)==0){ /* ��sdram������д��qspi �����ڴ�ӳ��ģʽ �Ƚ���sdram����һ�� */
									  QSPI_W25Qxx_MemoryMappedMode(); 
					          verify_qspi_data(1);
									  printf(" Update APP2 success!!!\r\n");
				            display_logo();
	                  jump_app(QSPI_APP2_MM_BASE_ADDRESS);
						    }
				      /*SDRAM �� QSPI���ݲ�һ�� */
						 printf(" APP2 SDRAM compare QSPI data different err\r\n");
						}
		        /*SD �� SDRAM���ݲ�һ�� */
            printf(" APP2 SD compare SDRAM data different err\r\n");

		 
	 }else{/* δ��ȡ�����ò��� */
		 
			
	    /* ���� δ��ȡ�����������QSPI */
		 printf(" system parament err or qspi not init : jump to app stop\r\n ");
	   }
    return -1;
}



/* ����Read_app_in_sd_card����ֵ �����ڵ�APPд��QSPI
 *
 */
void Check_the_app_in_the_sd_card(void){
   
	       int read_sd=0;
	    	 read_sd = Read_app_in_sd_card(); /*���sd�����Ƿ����APP����������ʾ����sd�� ������ʶ���м���APP*/
		   
		     if(read_sd>=1&&read_sd<=3){
		            SDRAM_InitSequence();/* ��ʼ��SDRAM */
					      QSPI_W25Qxx_Init(); /* ��ʼ��QSPI*/
		           } 
		 
		     if(read_sd==1){  
				  /* SD�д���APP1���� */
					printf(" Update APP1 from sd card\r\n");
          read_sd_data_to_sdram(1);
					write_sdram_data_to_qspi_flash(1);
					QSPI_W25Qxx_MemoryMappedMode();
          verify_qspi_data(2);					 
					jump_app(QSPI_APP1_MM_BASE_ADDRESS);
					
				 }else if(read_sd==2){/* SD�д���APP2���� */
					 printf(" Update APP2 from sd card\r\n");
					 read_sd_data_to_sdram(2);
					 write_sdram_data_to_qspi_flash(2);
					 QSPI_W25Qxx_MemoryMappedMode(); 
					 verify_qspi_data(2);
					 jump_app(QSPI_APP2_MM_BASE_ADDRESS);
					 
				 }else if(read_sd==3){/* SD�д���APP1 �� APP2���� */
					 printf(" Update APP1 and APP2 from SD card to QSPI\r\n");
					 read_sd_data_to_sdram(1);
					 read_sd_data_to_sdram(2);
					 write_sdram_data_to_qspi_flash(1);
					 write_sdram_data_to_qspi_flash(2);
					 QSPI_W25Qxx_MemoryMappedMode(); 
					 verify_qspi_data(1); /* ����У�� */ 
					 verify_qspi_data(2);
					 jump_app(QSPI_APP1_MM_BASE_ADDRESS);/* ��һ�μ���Ĭ��APP1 */
				 }else{
				        /* δ��⵽SD���д���APP */
					      for( ; ; )
					      {
					       printf(" APP not found  please add firmware\r\n");
						     delay_ms(500);
                }
				 
				      }


}





	

/* ��ȡϵͳ���� which app ��ѡһ���� */
int Dual_app_exist_which_app(void)
{
	 BOOT_SYS_config * psys_parameter = &sys_parameter;	
	
	 if(sys_parameter_config()!=0){
					 return -1; /* ��ȡ/д�� ϵͳ����ʧ��*/
	        }
            if((psys_parameter->which_app_running==1)||(psys_parameter->which_app_running==2))
						{
						 app_exist_which_app(psys_parameter->which_app_running);/* ��ȡ���ò���ѡ������app */
						}else{ /* which_app_running��������*/
                  for( ; ; ) {   
															LEDR_TOGGLE();
															printf(" sys which app parament err %d\r\n",psys_parameter->which_app_running);
															delay_ms(300);
			                       }
                 } 
    return -1;
}


	
 /*  ����SD��������ˢ�뵽QSPI APP1 or APP2 ��ַ����  */
 int read_app_and_jump_running(void)
 {
	 
	 	if(QSPI_W25Qxx_Init()!=QSPI_W25Qxx_OK){
	     printf(" W25Q64 init err \r\n");
		   return -1;
	    }
		
		
		if(sys_parameter_config()!=0){ /* ��ȡ/д�� ϵͳ���� */
			 printf(" W25Q64 init err \r\n");
			 return -1; 
	    }
		
	 printf(" current sys parament = %d\r\n",psys_parameter->which_app_running);
	
		
		
	/* �ϵ絹��ʱ��δ������������������   �̰����л�APP  ����(������Ч)��ǿ�Ƹ���APP(�����SD��) */	
		
	 key_init();
	
	 printf(" If no operation is performed within %ds, the system will jump\r\n",JUMP_TIME);
	 for(int i=JUMP_TIME*1000 ;i>=0 ;i--) 
	 {
		  if(i%1000==0)   printf(" %ds later jump  \r\n", i/1000);  /* ��ӡ����ʱ */ 
     
      CheckButtonPress();/* ��ⰴ���Ƿ���  �����д��� 1ms ��ʱ */
	 }
		
		
	 
	 int app_exist_state = check_qspi_app_exist();/* �ϵ���APP�Ƿ���� �Լ�У�� */


	 switch(app_exist_state){
		case 1: 
						app_exist_which_app(1);  /* QSPI��ֻ��APP1���� ֱ������ */
						break;
		
		case 2: 
			      app_exist_which_app(2);  /* QSPI��ֻ��APP2���� ֱ������ */
					  break;
		
		case 3:
			      Dual_app_exist_which_app(); /* QSPI��˫APP���� ���ݲ���ѡ������ ������ͨ���������� ���м��书�� */
			      break;    			
		
		default:
			      Check_the_app_in_the_sd_card(); /* QSPI��δ��⵽APP   ���SD�����Ƿ����APP  */
			      break;
   }
		
     for( ; ; ){
		   
		  /* ���е�������� */
		 }
 
}


