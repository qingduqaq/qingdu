/**
 *@file      bsp_timer.h
 *@brief     封装通用定时器的各种参数，时钟，中断服务函数
 *@details   无
 *@author    青渡
 *@version   V1.0.0
 *@date      2021-09-01
 */
#ifndef __bsp_timer_H
#define __bsp_timer_H

#include "stm32f10x.h"

extern uint32_t tim3_count;

#define  CURRENT_TIM                     TIM3
#define  CURRENT_TIM_APBxClock_FUN       RCC_APB1PeriphClockCmd
#define  CURRENT_TIM_CLK                 RCC_APB1Periph_TIM3
#define  CURRENT_TIM_Period              1000
#define  CURRENT_TIM_Prescaler           71
#define  CURRENT_TIM_IRQ                 TIM3_IRQn
#define  CURRENT_TIM_IRQHandler          TIM3_IRQHandler

void CURRENT_TIM_Init(void);

#endif
