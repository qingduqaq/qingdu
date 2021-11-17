/**
 *@file      bsp_usart.c
 *@brief     ��ʼ������ͨ���жϣ���д���ִ���ͨ��ģʽ����
 *@attention һ�ֲ��ô����жϷ�ʽ����ͨ�ţ�����һ�ֲ���ֱ�ӵ��ÿ⺯����ʽͨ��
 *@author    ���
 *@version   V1.0.0
 *@date      2021-09-01
 */
#include "bsp_usart.h"

//uint8_t TxCount = 0;
//uint8_t Count = 0;

/**���ڷ��ͻ�����*/
uint8_t TxBuff;	
/**���ڽ��ջ�����*/
__IO uint8_t RxBuffer[50];

/**
 *@brief     ����Ƕ�������жϿ�����NVIC
 *@attention static voidֻ�ڴ��ļ�����Ч
 *@param     ��
 *@retval    �� 
 */
static void NVIC_Configuration()
{
	/**ʵ����NVIC�ṹ��*/
	NVIC_InitTypeDef NVIC_InitStructure;
	/**Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/**����USART1Ϊ�ж��ź�Դ*/
	NVIC_InitStructure.NVIC_IRQChannel = USART_IRQ;
	/**������ռ���ȼ�Ϊ2*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	/**���ôμ����ȼ�Ϊ0*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/**����ʹ���жϿ���*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/**����������ʵ����NVIC�ṹ��*/
	NVIC_Init(&NVIC_InitStructure);
}

/**
 *@brief     ��GPIO���õ�USART�ϣ�����USART����������ʹ��USART�����ж�
 *@details   ��
 *@param     ��
 *@retval    ��
 */
void Usart_Config(void)
{
	/**ʵ����GPIO��USART�ṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/**����ʱ��*/
  USART_GPIO_APBxClkCmd(USART_GPIO_CLK, ENABLE);
	USART_APBxClkCmd(USART_CLK, ENABLE);
	
	/**��USART Tx(��������������ţ���GPIO����Ϊ���츴��ģʽ����������Ϊ50MHz*/
	GPIO_InitStructure.GPIO_Pin = USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_PORT, &GPIO_InitStructure);
	
	/**��USART Rx(����������������)��GPIO����Ϊ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin = USART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_RX_PORT, &GPIO_InitStructure);
	
	/**���ò�����*/
	USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
	/**����֡�����ֳ�Ϊ8bit*/
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/**����ֹͣλΪ1*/
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/**����У��λΪ��*/
	USART_InitStructure.USART_Parity = USART_Parity_No;
	/**����Ӳ��������*/
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/**���ù���ģʽ���շ�һ��*/
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/**��ɴ��ڵĳ�ʼ������*/
	USART_Init(USARTx, &USART_InitStructure);
	
	NVIC_Configuration();
	/**ʹ�ܴ��ڽ����ж�*/
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	/**ʹ�ܴ���*/
	USART_Cmd(USARTx, ENABLE);
	/**CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	�����������1���ֽ��޷���ȷ���ͳ�ȥ������,���������ɱ�־λ*/
	USART_ClearFlag(USARTx, USART_FLAG_TC);
}

/**
 *@brief     ��HEX����ʽ���U8������
 *@attention ʹ�ܴ����жϽ����ݷ��͸���λ������������ֹ��Ӧ�жϱ�־λ
 *@param     data  8λ��������
 *@retval    ��
 */
void PrintHexU8(uint8_t data)
{
	TxBuff = data;
	/**(!(DEBUG_USARTx->CR1 & USART_CR1_TXEIE))*/	
	if(USART_GetFlagStatus(USARTx, USART_FLAG_TXE))
	{
	  USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
	}
}

/**
 *@brief     ��HEX����ʽ���S16������
 *@attention �ڷ��ͺ��������ȴ�������ɺ���
 *@param     num  16λ��������
 *@retval    ��
 */
void PrintHexS16(int16_t num)
{
	/**Ϊ������������ݣ�ÿ������������䶼Ӧ�ò��������䣬�ȴ��������*/
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	/**�ȷ��͸�8λ���ٷ��͵�8λ*/
	PrintHexU8((uint8_t)(num & 0xff00) >> 8);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8((uint8_t)(num & 0x00ff));
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8(' ');
}

/**
 *@brief     ���ַ�����ʽ���S8������
 *@attention ��
 *@param     num  8λ��������
 *@retval    ��
 */
void PrintS8(int8_t num)
{
	uint8_t bai,shi,ge;
	/**��Ҫ���͵�numС��0ʱȡ������ӡ����*/
	if(num < 0)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		PrintHexU8('-');
		num = -num;
	}
	else
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		PrintHexU8(' ');
	}
	bai = num / 100;
	shi = num % 100 / 10;
	ge = num % 10;
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0' + bai);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0' + shi);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0' + ge);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8(' ');
}

/**
 *@brief     ���ַ�����ʽ���u8������
 *@attention ÿ������printhexu8����ֱ��Ӧ�ò���while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)��
						 ��ʾ�ȴ����ͼĴ���������Ϊ�գ�����һ�������Ѿ�������ɣ����Խ����µ�һ�����ݷ���
 *@param     num  �����͵�u8������
 *@retval    ��
 */
void PrintU8(uint8_t num)
{
	uint8_t  bai,shi,ge;
	bai = num / 100;
	shi = num % 100 / 10;
	ge = num % 10;
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	/**hex�������ַ��ͣ�ascii����ת��*/
	PrintHexU8('0'+bai);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+shi);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+ge);
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8(' ');
}

/**
 *@brief     ���ַ�����ʽ���S16������
 *@attention ��
 *@param     num  16λ��������
 *@retval    ��
 */
void PrintS16(int16_t num)
{
	uint8_t wan,qian,bai,shi,ge;
	if(num < 0)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		PrintHexU8('-');
		num = -num;
	}
	else
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		PrintHexU8(' ');
	}
	wan = num % 100000 / 10000;
	qian = num % 10000 / 1000;
	bai = num % 1000 / 100;
	shi = num % 100 / 10;
	ge = num % 10;
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+wan);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+qian);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+bai);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+shi);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+ge);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8(' ');
}

/**
 *@brief     ���ַ�����ʽ���U16������
 *@attention �����βα�Ϊ�Ǹ����������ý��������ж�
 *@param     num  16λ�޷�������
 *@retval    ��
 */
void PrintU16(uint16_t num)
{
	uint8_t wan,qian,bai,shi,ge;
	wan = num % 100000 / 10000;
	qian = num % 10000 / 1000;
	bai = num % 1000 / 100;
	shi = num % 100 / 10;
	ge = num % 10;
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+wan);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+qian);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+bai);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+shi);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+ge);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8(' ');
}

/**
 *@brief     ����ַ���
 *@attention ��
 *@param     s  char������
 *@retval    ��
 */
void PrintString(char *s)
{
	/**����Ӧָ���ַ��ָ����Ϊ�ǿ�ʱ*/
	while(*s != '\0')
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		PrintHexU8(*s);
		s++;
	}
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8(' ');
}

/**
 *@brief     ����һ���ֽ�
 *@details   ֱ�ӵ��ÿ⺯��������ݷ��ͣ�����ʹ���ж�
 *@param     pUSARTx:ѡ��USART��UART��Χ�豸���˲�������������ֵ֮һ��
             USART1��USART2��USART3��UART4��UART5��
 *@param     ch:���͵��ֽ�
 *@retval    ��
 */
void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx, ch);
	/**��־λTXE���������ݼĴ����ձ�־*/
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/**
 *@brief     �����ַ���
 *@details   ֱ�ӵ��ÿ⺯��������ݷ��ͣ�����ʹ���ж�
 *@param     pUSARTx:ѡ��USART��UART��Χ�豸���˲�������������ֵ֮һ��
             USART1��USART2��USART3��UART4��UART5
 *@param     str:ָ��ĳһ�ַ������׵�ַ
 *@retval    ��
 */
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
	unsigned int i = 0;
	/**���ַ�����'\0'�����ַ����Ľ�����־��
	*(str + i) ����ָ��ַ(str+i)������*/
	while(*(str + i) != '\0')
	{
		  Usart_SendByte(pUSARTx, *(str + i));
		  ++i;
	}
	/**��־λTC��������ɱ�־������whileѭ���Ѿ��ж��Ƿ�����ַ�*/
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

/**
 *@brief     ����һ��16λ��
 *@details   ֱ�ӵ��ÿ⺯��������ݷ��ͣ�����ʹ���жϣ�����˺���������
 *@param     pUSARTx:ѡ��USART��UART��Χ�豸���˲�������������ֵ֮һ��
             USART1��USART2��USART3��UART4��UART5
 *@param     ch:16λ��
 *@retval    ��
 */
void Usart_SendHalfWord(USART_TypeDef * pUSARTx, uint16_t ch)
{
  uint8_t temp_h, temp_l;
	/**ȡ���߰�λ*/
  temp_h = (ch & 0xFF00) >> 8;
	/**ȡ���Ͱ�λ*/
	temp_l = ch&0xFF;
	
	/**���͸߰�λ*/
	USART_SendData(pUSARTx, temp_h);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
 
	/**���͵Ͱ�λ*/
	USART_SendData(pUSARTx, temp_l);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/**
 *@brief     ��stm32��ʵ�ֽ�һ�����ݴ�ӡ����λ���ϣ����ض���ķ�ʽͨ������USART���ͳ�ȥ����
						 �ض�����printf�������ļ���-->����USART
 *@attention �ڵ��ñ�������printf�����ض���֮ǰ����Ҫ��USART��������
 *@param     ch  Ҫ���͵�����	
 *@param     *f  �ļ�ָ��
 *@retval    Ҫ���͵��ַ�
 */
int fputc(int ch, FILE *f)
{
	/**����һ���ֽ����ݵ�����*/
	USART_SendData(USARTx, (uint8_t)ch);
	/**�ȴ��������*/
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	return(ch);
}

/**�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���*/
int fgetc(FILE *f)
{
	/**�ȴ�������������*/
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USARTx);
}
