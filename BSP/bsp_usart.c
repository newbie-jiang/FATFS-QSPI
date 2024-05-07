#include "bsp_usart.h" 

 


extern UART_HandleTypeDef huart4;





void uart4_init(void)
{
  
 //hal���Ѿ����˳�ʼ��	
	
}





/*****************************************************
*function:	д�ַ��ļ�����
*param1:	������ַ�
*param2:	�ļ�ָ��
*return:	����ַ���ASCII��
******************************************************/
 int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart4, (uint8_t*)&ch, 1,0xff);
	return ch;
}





/*****************************************************
*function:	���ַ��ļ�����
*param1:	�ļ�ָ��
*return:	��ȡ�ַ���ASCII��
******************************************************/
 int fgetc(FILE *f)
{
    uint8_t ch = 0;
    HAL_UART_Receive(&huart4, (uint8_t*)&ch, 1, 10);
    return (int)ch;
}


/*
 * ������´��룬����Ҫ�ڹ��������й�ѡUse MicroLIB
*/

//#pragma import(__use_no_semihosting)
// 
//struct __FILE
//{
//	int a;
//};
// 
//FILE __stdout;
//FILE __stdin;
//void _sys_exit(int x)
//{
//	
//}




void bsp_debug_uart4_init(void)
{
  uart4_init();
	
	printf("-----------------boot+app experiment-------------------------\r\n");
	
}




