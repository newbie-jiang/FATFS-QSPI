#include "bsp_sd.h" 



/////*******************************fatfs*************************************/
//FATFS fs;                 // Work area (file system object) for logical drive
//FIL fil;                  // file objects
//uint32_t byteswritten;                /* File write counts */
//uint32_t bytesread;                   /* File read counts */
//uint8_t wtext[] = "This is STM32H750XB working with FatFs"; /* File write buffer */
//uint8_t rtext[100];                     /* File read buffers */
//char filename[] = "0:/mytest_2023_07_21.txt";

/////**************************************************************************/




//typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

///* ˽�к궨�� ----------------------------------------------------------------*/
//#define BLOCK_SIZE            512         // SD�����С     
//#define NUMBER_OF_BLOCKS      8           // ���Կ�����(С��15)
//#define WRITE_READ_ADDRESS    0x00002000  // ���Զ�д��ַ

///* ˽�б��� ------------------------------------------------------------------*/
//uint32_t Buffer_Block_Tx[BLOCK_SIZE*NUMBER_OF_BLOCKS]; // д���ݻ���
//uint32_t Buffer_Block_Rx[BLOCK_SIZE*NUMBER_OF_BLOCKS] = {1,2}; // �����ݻ���
//HAL_StatusTypeDef sd_status;    // HAL�⺯������SD����������ֵ���������
//TestStatus test_status;           // ���ݲ��Խ��

///* ��չ���� ------------------------------------------------------------------*/
///* ˽�к���ԭ�� --------------------------------------------------------------*/
//void Fill_Buffer(uint32_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
//TestStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength);
//TestStatus eBuffercmp(uint32_t* pBuffer, uint32_t BufferLength);

//void SD_EraseTest(void);
//void SD_Write_Read_Test(void);




//void get_sd_informatization(void)
//{
//	 
//   printf("Micro SD Card Test...\r\n");
//  /* ���SD���Ƿ��������������ݴ���ģʽ�Ĵ���״̬�� */
//  if(HAL_SD_GetCardState(&hsd1) == HAL_SD_CARD_TRANSFER)
//  {      
//    printf("Initialize SD card successfully!\r\n");
//    // ��ӡSD��������Ϣ
//    printf(" SD card information! \r\n");
//		/*ʵ�ʴ洢���� = LogBlockNbr * LogBlockSize*/
//		printf(" LogBlockNbr   : %d \r\n", hsd1.SdCard.LogBlockNbr);	// �߼�������
//	  printf(" LogBlockSize  : %d \r\n", hsd1.SdCard.LogBlockSize); // �߼����С(�ֽ�)
//    printf(" Card Log Capacity  : %llu byte\r\n", (unsigned long long)hsd1.SdCard.BlockSize * hsd1.SdCard.BlockNbr);// ��ʾ����(�ֽ�)
//		printf(" Card Log Capacity  : %llu M\r\n", ((unsigned long long)hsd1.SdCard.BlockSize * hsd1.SdCard.BlockNbr)/(1024*1024));// ��ʾ����(M)
//		printf(" CardBlockNbr  : %d \r\n", hsd1.SdCard.BlockNbr);   // ���������
//    printf(" CardBlockSize : %d \r\n", hsd1.SdCard.BlockSize);   // ������С
//		printf(" Card physical Capacity  : %llu M\r\n", ((unsigned long long)hsd1.SdCard.BlockSize * hsd1.SdCard.BlockNbr)/(1024*1024));// ��ʾ����(�ֽ�)

//    printf(" RCA           : %d \r\n", hsd1.SdCard.RelCardAdd);  // ����Ե�ַ
//    printf(" CardType      : %d \r\n", hsd1.SdCard.CardType);    // ������
//    // ��ȡ����ӡSD����CID��Ϣ
//    HAL_SD_CardCIDTypeDef sdcard_cid;
//    HAL_SD_GetCardCID(&hsd1,&sdcard_cid);
//    printf(" ManufacturerID: %d \r\n",sdcard_cid.ManufacturerID); /*��ȡsd��������*/
//  }
//  else
//  {
//    printf("SD card init fail!\r\n" );
//  }

//}




//void SD_EraseTest(void)
//{
//	/* ��1������ΪSD���������2������Ϊ������ʼ��ַ����3������Ϊ����������ַ */
//   sd_status=HAL_SD_Erase(&hsd1,WRITE_READ_ADDRESS,WRITE_READ_ADDRESS+NUMBER_OF_BLOCKS*4);
//   printf("erase status:%d\r\n",sd_status);
//	 HAL_Delay(100);
//  if (sd_status == HAL_OK)
//  {	
//    /* ��ȡ�ող��������� */
//    sd_status = HAL_SD_ReadBlocks(&hsd1,(uint8_t *)Buffer_Block_Rx,WRITE_READ_ADDRESS,NUMBER_OF_BLOCKS,0xffff);
//    printf("erase read status:%d\r\n",sd_status);
//    /* �Ѳ�������������Ա� */
//    test_status = eBuffercmp(Buffer_Block_Rx,BLOCK_SIZE*NUMBER_OF_BLOCKS);

//    if(test_status == PASSED)
//      printf("���������Գɹ���\r\n" ); 
//    else	  
//      printf("���������ɹ������ݳ���\r\n" );      
//  }
//  else
//  {
//    printf("����������ʧ�ܣ�����SD��֧�ֲ�����ֻҪ��д����ͨ������\r\n" );
//  }
//}

///**
//  * ��������: SD����д����
//  * �������: ��
//  * �� �� ֵ: ��
//  * ˵    ��: ��
//  */
//void SD_Write_Read_Test(void)
//{  
//	int i,j = 0;
//  /* ������ݵ�д���� */
//  Fill_Buffer(Buffer_Block_Tx,BLOCK_SIZE*NUMBER_OF_BLOCKS, 0x32F1);
//  
//  /* ��SD��д������ */
//  sd_status = HAL_SD_WriteBlocks(&hsd1,(uint8_t *)Buffer_Block_Tx,WRITE_READ_ADDRESS,NUMBER_OF_BLOCKS,0xffff);
//  printf("write status:%d\r\n",sd_status);
//  HAL_Delay(200);
// // HAL_Delay(500);
//  /* ��SD����ȡ���� */
//  sd_status = HAL_SD_ReadBlocks(&hsd1,(uint8_t *)Buffer_Block_Rx,WRITE_READ_ADDRESS,NUMBER_OF_BLOCKS,0xffff);
//  printf("read status:%d\r\n",sd_status);
//  
//  /* �Ƚ����� */
//  test_status = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE*NUMBER_OF_BLOCKS/4);	//�Ƚ�
//  if(test_status == PASSED)
//	{
//    printf("����д���Գɹ���\r\n" );
//		
//		for(i=0;i<BLOCK_SIZE*NUMBER_OF_BLOCKS/4;i++)
//		{
//			if(j==8)
//			{
//				printf("\r\n");
//				j=0;
//			}
//			
//			printf("%08x   ",Buffer_Block_Rx[i]);
//			j++;
//		}
//		printf("\r\n");
//	}
//  else  
//  	printf("����д����ʧ�ܣ�\r\n " );  
//}


///**
//  * ��������: �ڻ���������д����
//  * �������: pBuffer��Ҫ���Ļ�����
//  *           BufferLength��Ҫ���Ĵ�С
//  *           Offset�����ڻ������ĵ�һ��ֵ 
//  * �� �� ֵ: ��
//  * ˵    ��: ��
//  */
//void Fill_Buffer(uint32_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
//{
//  uint32_t index = 0;
//  /* ������� */
//  for (index = 0; index < BufferLength; index++ )
//  {
//    pBuffer[index] = index + Offset;
//  }
//}

///**
//  * ��������: �Ƚ������������е������Ƿ����
//  * �������: pBuffer1��Ҫ�ȽϵĻ�����1��ָ��
//  *           pBuffer2��Ҫ�ȽϵĻ�����2��ָ��
//  *           BufferLength������������
//  * �� �� ֵ: PASSED�����
//  *           FAILED������
//  * ˵    ��: ��
//  */
//TestStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength)
//{
//  while (BufferLength--)
//  {
//    if(BufferLength%50==0)
//    {
//      printf("buf:0x%08X - 0x%08X\r\n",*pBuffer1,*pBuffer2);
//    }
//    if (*pBuffer1 != *pBuffer2)
//    {
//      return FAILED;
//    }
//    pBuffer1++;
//    pBuffer2++;
//  }
//  return PASSED;
//}

///**
//  * ��������: ��黺�����������Ƿ�Ϊ0xff��0
//  * �������: pBuffer��Ҫ�ȽϵĻ�������ָ��
//  *           BufferLength������������
//  * �� �� ֵ: PASSED��������������ȫΪ0xff��0
//  *           FAILED��������������������һ����Ϊ0xff��0 
//  * ˵    ��: ��
//  */
//TestStatus eBuffercmp(uint32_t* pBuffer, uint32_t BufferLength)
//{
//  while (BufferLength--)
//  {
//    /* SD��������Ŀ���ֵΪ0xff��0 */
//    if ((*pBuffer != 0xFFFFFFFF) && (*pBuffer != 0))
//    {
//      return FAILED;
//    }
//    pBuffer++;
//  }
//  return PASSED;
//}




//void fatfs_test(void)
//{

//printf("\r\n ****** FatFs Example ******\r\n\r\n");
//    /*##-1- Register the file system object to the FatFs module ##############*/
//	//f_mkfs("1:",FM_ANY,0,fatbuf,FF_MAX_SS);
//	retSD = f_mount(&fs, SDPath, 1);
//	if(retSD)
//	{
//			printf(" mount error : %d \r\n",retSD);
//			Error_Handler();
//	}
//	else
//			printf(" mount sucess!!! \r\n");
//	/*##-2- Create and Open new text file objects with write access ######*/
//	retSD = f_open(&fil, filename, FA_CREATE_NEW | FA_WRITE);
//	if(retSD)
//			printf(" open file error : %d\r\n",retSD);
//	else
//			printf(" open file sucess!!! \r\n");
//	/*##-3- Write data to the text files ###############################*/
//	retSD = f_write(&fil, wtext, sizeof(wtext), (void *)&byteswritten);
//	if(retSD)
//			printf(" write file error : %d\r\n",retSD);
//	else
//	{
//			printf(" write file sucess!!! \r\n");
//			printf(" write Data : %s\r\n",wtext);
//	}
//	/*##-4- Close the open text files ################################*/
//	retSD = f_close(&fil);
//	if(retSD)
//			printf(" close error : %d\r\n",retSD);
//	else
//			printf(" close sucess!!! \r\n");
//	/*##-5- Open the text files object with read access ##############*/
//	retSD = f_open(&fil, filename, FA_READ);
//	if(retSD)
//			printf(" open file error : %d\r\n",retSD);
//	else
//			printf(" open file sucess!!! \r\n");
//	/*##-6- Read data from the text files ##########################*/
//	retSD = f_read(&fil, rtext, sizeof(rtext), (UINT*)&bytesread);
//	if(retSD)
//			printf(" read error!!! %d\r\n",retSD);
//	else
//	{
//			printf(" read sucess!!! \r\n");
//			printf(" read Data : %s\r\n",rtext);
//	}
//	/*##-7- Close the open text files ############################*/
//	retSD = f_close(&fil);
//	if(retSD) 
//			printf(" close error!!! %d\r\n",retSD);
//	else
//			printf(" close sucess!!! \r\n");

//}



///*******************************************************************************************************************/
///*******************************************************************************************************************/
///**************************************************SD����������*****************************************************/
///*******************************************************************************************************************/
///*******************************************************************************************************************/

//FIL file;      // �ļ�����
//FRESULT fres;  // FatFs�����ķ�������
//FILINFO fno;
//FATFS fs;       // �ļ�ϵͳ����


///*************************����SD***************************************/
//int mount_sd(void)
//{
//	retSD = f_mount(&fs, SDPath, 1);
//	if(retSD)
//	{
//		   #ifdef  FATFS_DEBUG
//			   printf("  mount error : %d \r\n",retSD);
//			 #endif
//			
//			Error_Handler();
//	}
//	else
//	{
//			
//	     #ifdef  FATFS_DEBUG
//			   printf("  mount sd success!!! \r\n");
//			 #endif
//	}
//	
//	return 0;
//	
//}

///*************************��ʽ��SD��***************************************/
///*�ڵ���f_mkfs֮ǰ��ȷ��û�ж�Ŀ�����������й���
//��ʽ��֮��ͨ����ж����������ȷ�����е�ϵͳ��Դ�õ���ȷ���ͷ�*/

////�����������������С
//#define  FF_MAX_SS  512

//FRESULT mkfs_sdcard(void) {

//    BYTE work[FF_MAX_SS];  // Working buffer for f_mkfs

//    fres = f_mount(&fs, "", 0);  // �����豸
//    if (fres != FR_OK) {
//			#ifdef  FATFS_DEBUG
//			printf("ERR SD Mounting failed:%d\r\n",fres);
//			#endif
//			
//        return fres;  // ���ش���
//    }

//    fres = f_mkfs("", FM_FAT32, 0, work, sizeof(work));  // Format as FAT32
//    if (fres != FR_OK) {
//			#ifdef  FATFS_DEBUG
//			printf("ERR SD Formatting failed:%d\r\n",fres);
//			#endif
//        return fres;  //��ʽ��ʧ��
//    }

//    f_mount(0, "", 0);  // ��ʽ����Ĭ��ж������
//		#ifdef  FATFS_DEBUG
//		printf("SD Formatting ok\r\n");
//		#endif
//    return FR_OK;
//}



///**********************���Ŀ¼*****************************/
//static uint8_t Check_whether_the_directory_exists(const char* path)
//{
//    
//    fres = f_stat(path, &fno);
//    if (fres == FR_OK && (fno.fattrib & AM_DIR)) {
//        return 1;  // Ŀ¼����
//    }
//    return 0;  // Ŀ¼������
//}

///************************����Ŀ¼***********************************/
// uint8_t createDirectory(const char* path) {
//    if (!Check_whether_the_directory_exists(path)) {
//        fres = f_mkdir(path);
//        if(fres != FR_OK) {
//            //���������ʾ������Ϣ
//					#ifdef  FATFS_DEBUG
//					printf("ERR create Directory :%d\r\n",fres);
//					#endif
//					    return 1;/*���ش�����Ϣ*/
//        } else {
//            // Ŀ¼�ɹ�����
//					#ifdef  FATFS_DEBUG
//					    printf("create Directory ok\r\n");
//					#endif
//						  return 0;
//        }
//    } else {
//        // Ŀ¼�Ѿ�����
//			#ifdef  FATFS_DEBUG
//		     printf("Directory already exists\r\n");
//			#endif
//		     return 0;
//    }
//}

///*****************************����Ŀ¼********************************/
//uint8_t use_createDirectory(void)
//{
//  createDirectory("/system");
//	return 0;
//}





///************************����ļ��Ƿ����******************************/
//static uint8_t createFileIfNotExist(const char* folderPath, const char* filename) {
//    char fullPath[256];  // �����ļ�·�����Ȳ�����256�����Ը���ʵ����Ҫ������С

//    // �ж��Ƿ�Ϊ��Ŀ¼
//    if (folderPath && *folderPath) {
//        sprintf(fullPath, "%s/%s", folderPath, filename);
//    } else {
//        sprintf(fullPath, "%s", filename); // ֱ��ʹ���ļ�������Ϊ���ڸ�Ŀ¼
//    }

//    // ����ļ��Ƿ����
//    fres = f_stat(fullPath, &fno);
//    if (fres == FR_OK) {
//        // �ļ�����
//        return 1;
//    } else if (fres == FR_NO_FILE) {
//        // �ļ������ڣ����Դ���
//        fres = f_open(&file, fullPath, FA_WRITE | FA_CREATE_NEW);
//        if (fres == FR_OK) {
//            f_close(&file);
//            return 0;  // �ļ��ѳɹ�����
//        } else {
//            return 1;  // �����ļ�ʱ��������
//        }
//    } else {
//        // ��������
//        return 1;
//    }
//}

///*************************�����ļ�*************************/
// uint8_t create_a_file(const char* folderPath, const char* filename) {
//    uint8_t result = createFileIfNotExist(folderPath, filename);

//    if (result == 1) {
//        // �ļ��Ѵ���
//        #ifdef FATFS_DEBUG
//            printf("File already exists: %d\r\n", fres);
//        #endif
//    } else if (result == 0) {
//        // �ļ��ѳɹ�����
//        #ifdef FATFS_DEBUG
//            printf("File created successfully: %d\r\n", fres);
//        #endif
//    } else {
//        // ��������
//        #ifdef FATFS_DEBUG
//            printf("Error creating file: %d\r\n", fres);
//        #endif
//    }

//    return 0;
//}

///**************************�����ļ�************************************/
//uint8_t use_create_a_file(void)
//{
//  // �ڸ�Ŀ¼����
//  // create_a_file("", "myfile.txt");
//  // �������ļ��д���
//  // create_a_file("system", "myfile.txt");
//    create_a_file("system", "file1.txt");
//	  create_a_file("system", "file2.txt");
//	  return 0;
//}

///*************************д�ļ�����*************************************/

//uint8_t writeToFile(const char *filePath, const void *buffer, uint32_t length) {
//  
//    UINT bytesWritten;

//    // ���ļ�
//    fres = f_open(&file, filePath, FA_WRITE | FA_OPEN_ALWAYS);
//    if (fres != FR_OK) {
//        // �ļ���ʧ��
//        return 1;
//    }

//    // ��дָ���Ƶ��ļ�ĩβ���Ա����ļ�ĩβ׷������
//    f_lseek(&file, f_size(&file));

//    // д������
//    fres = f_write(&file, buffer, length, &bytesWritten);

//    // �ر��ļ�
//    f_close(&file);

//    // ����Ƿ��������ݶ���д��
//    if (fres == FR_OK && bytesWritten == length) {
//        #ifdef FATFS_DEBUG
//            printf("File write length ok: %d\r\n", fres);
//        #endif
//			 return 0; // �ɹ�
//    } else {
//			 #ifdef FATFS_DEBUG
//            printf("File write length err: %d\r\n", fres);
//        #endif
//			return 1; // д�����
//    }
//}


///**************************д���ļ�************************************/
//uint8_t use_writeToFile(void) {
//   
//	 int buffer[256];
//	 for(int i=0;i<=255;i++)
//	{
//	  buffer[i]=i;
//	}

//   writeToFile("system/file1.txt", buffer, sizeof(buffer));

//    return 0;  
//}



///**
// * ��ȡ�ļ������ݡ�
// * @param filePath Ҫ��ȡ���ļ���·����
// * @param buffer ��Ŷ�ȡ���ݵĻ�������
// * @param bufferSize �������Ĵ�С��
// * @param offset �����ַ���ʼ��ȡ��
// * @return ʵ�ʶ�ȡ���ֽ�������-1��ʾ����
// */
//int readFileContent(const char* filePath, void* buffer, int bufferSize, DWORD offset) {

//    UINT bytesRead;
//    FIL file;
//    FRESULT fres;

//    fres = f_open(&file, filePath, FA_READ);
//    if (fres != FR_OK) {
//        // �ļ���ʧ��
//        return -1;
//    }

//    // ���ö�ȡ�Ŀ�ʼλ��
//    if (f_lseek(&file, offset) != FR_OK) {
//        // ������
//        f_close(&file);
//        return -1;
//    }

//    fres = f_read(&file, buffer, bufferSize, &bytesRead);
//    if (fres != FR_OK) {
//        // ��ȡ�ļ�ʧ��
//        f_close(&file);
//        return -1;
//    }

//    f_close(&file);  // �ǵùر��ļ�

//    #ifdef FATFS_DEBUG
////    printf("  bytesRead num=%d\r\n", bytesRead);  // ʵ�ʶ�ȡ�ֽ���
//    #endif

//    return bytesRead;  // ����ʵ�ʶ�ȡ���ֽ���
//}


///*ʹ�ö��ļ�����*/
//uint8_t use_readFileContent(void)
//{
//  int buffer[512];
//	memset(buffer, 0, sizeof(buffer));/*��ջ�����*/
//	
//  uint32_t bytesRead = readFileContent("system/file1.txt", buffer, sizeof(buffer), 0); // �ӵ�0���ֽڿ�ʼ��ȡ

//	
//	
//	for(int i=0;i<512;i++)
//	printf("read sd buffer[%d]:%x\r\n",i,buffer[i]);
//	
//	return 0;

//}


///* sd�����ԣ������ļ� �������ļ��� д�ļ������ļ� */
//void sd_application(void)
//{
//	 mkfs_sdcard();//��ʽ��sd��
//	 mount_sd();/*����sd��*/
//   use_createDirectory();/*�����ļ���*/
//   use_create_a_file();/*�����ļ�*/ 
//   use_writeToFile();/*д�ļ�*/
//   use_readFileContent();/*���ļ�*/
//}


//void bsp_sd_init(void)
//{
//   mount_sd();/*����sd��*/
//}



