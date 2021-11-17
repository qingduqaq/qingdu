/**
 *@file      bsp_adc.h
 *@brief     封装ADC(GPIO)和DMA相应通道，初始化时钟，声明函数
 *@details   PC0引脚在霸道里面接的是蜂鸣器，默认被拉低
             PC0引脚在指南者里面接的是SPI FLASH的片选，默认被拉高
             所以PC0做ADC转换通道的时候，结果可能会有误差
 *@author    青渡
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

/**转换（采样）通道个数*/
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

/**ADC1对应DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能*/
#define  ADC_x                     ADC1
#define  ADC_DMA_CHANNEL           DMA1_Channel1
#define  ADC_DMA_CLK               RCC_AHBPeriph_DMA1

void ADCx_Init(void);

#endif
