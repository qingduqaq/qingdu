/**
 *@file      bsp_timer.c
 *@brief     初始化通用定时器的嵌套向量中断控制器配置和工作模式
 *@attention STM32F103C8T6没有基本定时器，此文件采用通用定时器TIM3
 *@author    青渡
 *@version   V1.0.0
 *@date      2021-09-01
 */
#include "bsp_tim.h"

uint32_t tim3_count = 0;
/**
 *@brief     初始化嵌套向量中断控制器NVIC
 *@attention 分频后输出的时钟频率为1000Hz
 *@param     无
 *@retval    无
 */
static void TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/**配置NVIC为优先级组1*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/**将通用定时器TIM3作为中断源*/
	NVIC_InitStructure.NVIC_IRQChannel = CURRENT_TIM_IRQ;
	/**配置抢占优先级为0*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/**配置子优先级为1*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/**使能中断通道*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/**按照上述配置初始化嵌套向量中断控制器NVIC*/
	NVIC_Init(&NVIC_InitStructure);
}

/**
 *@brief     配置基本定时器的工作模式
 *@attention 内部时钟CK_INT为72MHZ，驱动定时器的时钟被预分频数分频后的频率为72/（1+BASIC_TIM_Prescaler） = 1MHZ
 *@param     无
 *@retval    无
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	/**开启定时器时钟，时钟频率为72MHZ*/	
	CURRENT_TIM_APBxClock_FUN(CURRENT_TIM_CLK, ENABLE);
	TIM_DeInit(CURRENT_TIM);	
	/**设置自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断*/
	TIM_TimeBaseStructure.TIM_Period = CURRENT_TIM_Period;
	/**设置时钟预分频数*/
	TIM_TimeBaseStructure.TIM_Prescaler = CURRENT_TIM_Prescaler;
	/**设置时钟分割为0*/
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	/**设置计数模式为向上计数*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/**按照上述配置初始化定时器*/
	TIM_TimeBaseInit(CURRENT_TIM, &TIM_TimeBaseStructure);
	/**清除定时器中断标志位*/
	TIM_ClearFlag(CURRENT_TIM, TIM_FLAG_Update);
	/**开启定时器中断*/
	TIM_ITConfig(CURRENT_TIM, TIM_IT_Update, ENABLE);
	/**使能定时器*/
	TIM_Cmd(CURRENT_TIM, ENABLE);
}
	
/**
 *@brief     集成通用定时器的初始化配置
 *@attention 无
 *@param     无
 *@retval    无
 */
void CURRENT_TIM_Init(void)
{
  TIM_NVIC_Config();
	TIM_Mode_Config();
}
