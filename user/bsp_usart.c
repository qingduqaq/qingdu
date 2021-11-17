/**
 *@file      bsp_usart.c
 *@brief     初始化串口通信中断，编写各种串口通信模式函数
 *@attention 一种采用串口中断方式进行通信，另外一种采用直接调用库函数方式通信
 *@author    青渡
 *@version   V1.0.0
 *@date      2021-09-01
 */
#include "bsp_usart.h"

//uint8_t TxCount = 0;
//uint8_t Count = 0;

/**串口发送缓冲区*/
uint8_t TxBuff;	
/**串口接收缓冲区*/
__IO uint8_t RxBuffer[50];

/**
 *@brief     配置嵌套向量中断控制器NVIC
 *@attention static void只在此文件内生效
 *@param     无
 *@retval    无 
 */
static void NVIC_Configuration()
{
	/**实例化NVIC结构体*/
	NVIC_InitTypeDef NVIC_InitStructure;
	/**嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/**配置USART1为中断信号源*/
	NVIC_InitStructure.NVIC_IRQChannel = USART_IRQ;
	/**配置抢占优先级为2*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	/**配置次级优先级为0*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/**开启使能中断控制*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/**以上述配置实例化NVIC结构体*/
	NVIC_Init(&NVIC_InitStructure);
}

/**
 *@brief     将GPIO复用到USART上，配置USART工作参数，使能USART接收中断
 *@details   无
 *@param     无
 *@retval    无
 */
void Usart_Config(void)
{
	/**实例化GPIO、USART结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/**开启时钟*/
  USART_GPIO_APBxClkCmd(USART_GPIO_CLK, ENABLE);
	USART_APBxClkCmd(USART_CLK, ENABLE);
	
	/**将USART Tx(发送数据输出引脚）的GPIO配置为推挽复用模式，引脚速率为50MHz*/
	GPIO_InitStructure.GPIO_Pin = USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_PORT, &GPIO_InitStructure);
	
	/**将USART Rx(接收数据输入引脚)的GPIO配置为浮空输入模式*/
	GPIO_InitStructure.GPIO_Pin = USART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_RX_PORT, &GPIO_InitStructure);
	
	/**配置波特率*/
	USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
	/**配置帧数据字长为8bit*/
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/**配置停止位为1*/
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/**配置校验位为无*/
	USART_InitStructure.USART_Parity = USART_Parity_No;
	/**配置硬件流控制*/
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/**配置工作模式，收发一起*/
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/**完成串口的初始化配置*/
	USART_Init(USARTx, &USART_InitStructure);
	
	NVIC_Configuration();
	/**使能串口接收中断*/
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	/**使能串口*/
	USART_Cmd(USARTx, ENABLE);
	/**CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
	如下语句解决第1个字节无法正确发送出去的问题,清除发送完成标志位*/
	USART_ClearFlag(USARTx, USART_FLAG_TC);
}

/**
 *@brief     以HEX的形式输出U8型数据
 *@attention 使能串口中断将数据发送给上位机，发送完后禁止相应中断标志位
 *@param     data  8位整型数据
 *@retval    无
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
 *@brief     以HEX的形式输出S16型数据
 *@attention 在发送函数间插入等待发送完成函数
 *@param     num  16位整型数据
 *@retval    无
 */
void PrintHexS16(int16_t num)
{
	/**为了连续输出数据，每两条输出函数间都应该插入此条语句，等待发送完成*/
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	/**先发送高8位，再发送低8位*/
	PrintHexU8((uint8_t)(num & 0xff00) >> 8);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8((uint8_t)(num & 0x00ff));
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8(' ');
}

/**
 *@brief     以字符的形式输出S8型数据
 *@attention 无
 *@param     num  8位整型数据
 *@retval    无
 */
void PrintS8(int8_t num)
{
	uint8_t bai,shi,ge;
	/**当要发送的num小于0时取反并打印负号*/
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
 *@brief     以字符的形式输出u8型数据
 *@attention 每隔两个printhexu8函数直接应该插入while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)，
						 表示等待发送寄存器的数据为空，即上一组数据已经发送完成，可以进行新的一轮数据发送
 *@param     num  待发送的u8型数据
 *@retval    无
 */
void PrintU8(uint8_t num)
{
	uint8_t  bai,shi,ge;
	bai = num / 100;
	shi = num % 100 / 10;
	ge = num % 10;
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	/**hex类型与字符型（ascii）的转换*/
	PrintHexU8('0'+bai);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+shi);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8('0'+ge);
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	PrintHexU8(' ');
}

/**
 *@brief     以字符的形式输出S16型数据
 *@attention 无
 *@param     num  16位整型数据
 *@retval    无
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
 *@brief     以字符的形式输出U16型数据
 *@attention 输入形参必为非负整数，不用进行正负判断
 *@param     num  16位无符号整数
 *@retval    无
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
 *@brief     输出字符串
 *@attention 无
 *@param     s  char型数据
 *@retval    无
 */
void PrintString(char *s)
{
	/**当对应指针地址所指内容为非空时*/
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
 *@brief     发送一个字节
 *@details   直接调用库函数完成数据发送，无需使能中断
 *@param     pUSARTx:选择USART或UART外围设备。此参数可以是以下值之一：
             USART1、USART2、USART3、UART4或UART5。
 *@param     ch:发送的字节
 *@retval    无
 */
void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx, ch);
	/**标志位TXE：传输数据寄存器空标志*/
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/**
 *@brief     发送字符串
 *@details   直接调用库函数完成数据发送，无需使能中断
 *@param     pUSARTx:选择USART或UART外围设备。此参数可以是以下值之一：
             USART1、USART2、USART3、UART4或UART5
 *@param     str:指向某一字符串的首地址
 *@retval    无
 */
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
	unsigned int i = 0;
	/**在字符串中'\0'用作字符串的结束标志，
	*(str + i) 是所指地址(str+i)的内容*/
	while(*(str + i) != '\0')
	{
		  Usart_SendByte(pUSARTx, *(str + i));
		  ++i;
	}
	/**标志位TC：传输完成标志，上述while循环已经判断是否传输空字符*/
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

/**
 *@brief     发送一个16位数
 *@details   直接调用库函数完成数据发送，无需使能中断，好像此函数不能用
 *@param     pUSARTx:选择USART或UART外围设备。此参数可以是以下值之一：
             USART1、USART2、USART3、UART4或UART5
 *@param     ch:16位数
 *@retval    无
 */
void Usart_SendHalfWord(USART_TypeDef * pUSARTx, uint16_t ch)
{
  uint8_t temp_h, temp_l;
	/**取出高八位*/
  temp_h = (ch & 0xFF00) >> 8;
	/**取出低八位*/
	temp_l = ch&0xFF;
	
	/**发送高八位*/
	USART_SendData(pUSARTx, temp_h);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
 
	/**发送低八位*/
	USART_SendData(pUSARTx, temp_l);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/**
 *@brief     在stm32上实现将一串数据打印在上位机上（以重定向的方式通过串口USART发送出去），
						 重定向后的printf函数：文件流-->串口USART
 *@attention 在调用本函数对printf函数重定向之前，先要对USART进行配置
 *@param     ch  要发送的数据	
 *@param     *f  文件指针
 *@retval    要发送的字符
 */
int fputc(int ch, FILE *f)
{
	/**发送一个字节数据到串口*/
	USART_SendData(USARTx, (uint8_t)ch);
	/**等待发送完毕*/
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	return(ch);
}

/**重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数*/
int fgetc(FILE *f)
{
	/**等待串口输入数据*/
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USARTx);
}
