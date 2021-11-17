/**
 *@file      bsp_usart.h
 *@brief     封装USART和GPIO引脚，宏定义中断，声明一系列函数
 *@details   包含标准输入输出头文件stdio.h，能重写printf函数和scanf函数，用到串口中断，所以应该
						 开启AFIO时钟
 *@author    青渡
 *@version   V1.0.0
 *@date      2021-09-01
 */
#ifndef __bsp_usart_H
#define __bsp_usart_H

#include "stm32f10x.h"
#include "stdio.h"

extern uint8_t TxBuff;
extern __IO uint8_t RxBuffer[50];

/**宏定义串口USART1*/
#define  USARTx                 USART1
#define  USART_CLK              RCC_APB2Periph_USART1
#define  USART_APBxClkCmd       RCC_APB2PeriphClockCmd
#define  USART_BAUDRATE         115200

/**USART GPIO引脚宏定义*/
#define  USART_GPIO_CLK         RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO
#define  USART_GPIO_APBxClkCmd  RCC_APB2PeriphClockCmd
#define  USART_TX_PORT          GPIOA
#define  USART_TX_PIN           GPIO_Pin_9
#define  USART_RX_PORT          GPIOA
#define  USART_RX_PIN           GPIO_Pin_10

#define  USART_IRQ              USART1_IRQn
#define  USART_IRQHandler       USART1_IRQHandler

void PrintHexU8(uint8_t data);
void PrintHexS16(int16_t num);
void PrintS8(int8_t num);
void PrintU8(uint8_t num);
void PrintS16(int16_t num);
void PrintU16(uint16_t num);
void PrintString(char *s);
void Usart_Config(void);
void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString(USART_TypeDef * pUSARTx, char*str);
void Usart_SendHalfWord(USART_TypeDef * pUSARTx, uint16_t ch);

#endif
