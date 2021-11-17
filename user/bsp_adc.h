/**
 *@file      bsp_adc.h
 *@brief     ��װADC(GPIO)��DMA��Ӧͨ������ʼ��ʱ�ӣ���������
 *@details   PC0�����ڰԵ�����ӵ��Ƿ�������Ĭ�ϱ�����
             PC0������ָ��������ӵ���SPI FLASH��Ƭѡ��Ĭ�ϱ�����
             ����PC0��ADCת��ͨ����ʱ�򣬽�����ܻ������
 *@author    ���
 *@version   V1.0.0
 *@date      2021-07-30
 */
#ifndef __bsp_adc_H
#define __bsp_adc_H

#include "stm32f10x.h"

#define  ADC_APBxClock_Cmd         RCC_APB2PeriphClockCmd
#define  ADC_CLK                   RCC_APB2Periph_ADC1

#define  ADC_GPIO_APBxClock_Cmd    RCC_APB2PeriphClockCmd
#define  ADC_GPIO_CLK              RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC
#define  ADC_PORT                  GPIOC
#define  ADC_PORT1                 GPIOA

/**ת����������ͨ������*/
#define  NUMCHANNEL                6
#define  ADC_PIN1                  GPIO_Pin_0
#define  ADC_CHANNEL1              ADC_Channel_10
#define  ADC_PIN2                  GPIO_Pin_1
#define  ADC_CHANNEL2              ADC_Channel_11
#define  ADC_PIN3                  GPIO_Pin_0
#define  ADC_CHANNEL3              ADC_Channel_0
#define  ADC_PIN4                  GPIO_Pin_1
#define  ADC_CHANNEL4              ADC_Channel_1
#define  ADC_PIN5                  GPIO_Pin_2
#define  ADC_CHANNEL5              ADC_Channel_2
#define  ADC_PIN6                  GPIO_Pin_3
#define  ADC_CHANNEL6              ADC_Channel_3

/**ADC1��ӦDMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����*/
#define  ADC_x                     ADC1
#define  ADC_DMA_CHANNEL           DMA1_Channel1
#define  ADC_DMA_CLK               RCC_AHBPeriph_DMA1

void ADCx_Init(void);

#endif
