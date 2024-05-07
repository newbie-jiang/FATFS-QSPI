	#include "bsp_sdram.h"



	/* USER CODE BEGIN 0 */
	static FMC_SDRAM_CommandTypeDef Command;

	#define sdramHandle hsdram1
	#define SDRAM_TIMEOUT                    ((uint32_t)0xFFFF)  //���峬ʱʱ��
	 
	 
	 

	 void SDRAM_InitSequence(void)
	{
		uint32_t tmpr = 0;

		/* Step 1 ----------------------------------------------------------------*/
		/* ������������ṩ��SDRAM��ʱ�� */
		Command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE; //ʱ������ʹ��
		Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;     //Ŀ��SDRAM�洢����
		Command.AutoRefreshNumber = 1;
		Command.ModeRegisterDefinition = 0;
		/* ������������ */
		HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

		/* Step 2: ��ʱ100us */ 
		
		HAL_Delay(1);
		
		/* Step 3 ----------------------------------------------------------------*/
		/* ������������е�bankԤ��� */ 
		Command.CommandMode = FMC_SDRAM_CMD_PALL;    //Ԥ�������
		Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;    //Ŀ��SDRAM�洢����
		Command.AutoRefreshNumber = 1;
		Command.ModeRegisterDefinition = 0;
		/* ������������ */
		HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);   

		/* Step 4 ----------------------------------------------------------------*/
		/* ��������Զ�ˢ�� */   
		Command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;  //�Զ�ˢ������
		Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
		Command.AutoRefreshNumber = 4;  //������ˢ�´��� 
		Command.ModeRegisterDefinition = 0;  
		/* ������������ */
		HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

		/* Step 5 ----------------------------------------------------------------*/
		/* ����sdram�Ĵ������� */
		tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |  //����ͻ������:1(������1/2/4/8)
						 SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |  //����ͻ������:����(����������/���)
						 SDRAM_MODEREG_CAS_LATENCY_2           |   //����CASֵ:3(������2/3)
						 SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //���ò���ģʽ:0,��׼ģʽ
						 SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;    //����ͻ��дģʽ:1,�������

		/* �����������SDRAM�Ĵ��� */
		Command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;  //����ģʽ�Ĵ�������
		Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
		Command.AutoRefreshNumber = 1;
		Command.ModeRegisterDefinition = tmpr;
		/* ������������ */
		HAL_SDRAM_SendCommand(&sdramHandle, &Command, SDRAM_TIMEOUT);

		/* Step 6 ----------------------------------------------------------------*/

		/* ������ˢ������ */
		
				//ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��:
		//COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
			//����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=200/2=100Mhz,����Ϊ8192(2^13).
		//����,COUNT=64*1000*100/8192-20=761
		HAL_SDRAM_ProgramRefreshRate(&sdramHandle, 761); 
	}


	//��ָ����ַ(WriteAddr+Bank5_SDRAM_ADDR)��ʼ,����д��n���ֽ�.
	//pBuffer:�ֽ�ָ��
	//WriteAddr:Ҫд��ĵ�ַ
	//n:Ҫд����ֽ���
	void FMC_SDRAM_WriteBuffer(uint8_t *pBuffer,uint32_t WriteAddr,uint32_t n)
	{
		for(;n!=0;n--)
		{
			*(__IO uint8_t*)(SDRAM_BANK_ADDR+WriteAddr)=*pBuffer;
			WriteAddr++;
			pBuffer++;
		}
	}

	//��ָ����ַ((WriteAddr+Bank5_SDRAM_ADDR))��ʼ,��������n���ֽ�.
	//pBuffer:�ֽ�ָ��
	//ReadAddr:Ҫ��������ʼ��ַ
	//n:Ҫд����ֽ���
	void FMC_SDRAM_ReadBuffer(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t n)
	{
		for(;n!=0;n--)
		{
			*pBuffer++=*(__IO uint8_t*)(SDRAM_BANK_ADDR+ReadAddr);
			ReadAddr++;
		}
	}




	void fsmc_sdram_test(void)
	{  
		__IO uint32_t i=0;  	  
		__IO uint32_t temp=0;	   
		__IO uint32_t sval=0;	//�ڵ�ַ0����������	  				   
		
		//ÿ��16K�ֽ�,д��һ������,�ܹ�д��2048������,�պ���32M�ֽ�
		for(i=0;i<32*1024*1024;i+=16*1024)
		{
			*(__IO uint32_t*)(SDRAM_BANK_ADDR+i)=temp; 
			temp++;
		}
		//���ζ���֮ǰд�������,����У��		  
		for(i=0;i<32*1024*1024;i+=16*1024) 
		{	
				temp=*(__IO uint32_t*)(SDRAM_BANK_ADDR+i);    //SDRAM_BANK_ADDR   BACK0�Ļ���ַ
			if(i==0)sval=temp;
			else if(temp<=sval)break;//�������������һ��Ҫ�ȵ�һ�ζ��������ݴ�.	   		   
			printf("SDRAM Capacity:%dKB\r\n",(uint16_t)(temp-sval+1)*16);//��ӡSDRAM����
		}					 
	}






	/* ��ʼ����У�� */
	int bsp_sdram_init(void)
	{
		__IO uint32_t i=0;  	  
		__IO uint32_t temp=0;	   
		__IO uint32_t read_val=0;	//���ڶ�ȡSDRAM�е�����
		__IO uint8_t success_flag=1; //��־λ�����ڼ�������Ƿ�һ��
		
		SDRAM_InitSequence();/*��ʼ��sdram*/

		//ÿ��16K�ֽ�,д��һ������,�ܹ�д��2048������,�պ���32M�ֽ�
		for(i=0;i<32*1024*1024;i+=16*1024)
		{
			*(__IO uint32_t*)(SDRAM_BANK_ADDR+i)=temp; 
			temp++;
		}

		//����tempֵ
		temp=0;

		//���ζ���֮ǰд�������,����У��		  
		for(i=0;i<32*1024*1024;i+=16*1024) 
		{	
				read_val=*(__IO uint32_t*)(SDRAM_BANK_ADDR+i);    //SDRAM_BANK_ADDR   BACK0�Ļ���ַ
			if(read_val != temp)
			{
				success_flag=0; //�����ȡ��������д������ݲ�ƥ�䣬���ñ�־λΪ0
				break;
			}
			temp++;
		}	
		

			if(success_flag)
			{
				printf("  checkout sdram : success!!!\r\n");
				return 0;
			}
			else
			{
				printf("  checkout sdram : err!!!\r\n");
				return -1;
			}		
	}


