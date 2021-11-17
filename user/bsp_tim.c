/**
 *@file      bsp_timer.c
 *@brief     ��ʼ��ͨ�ö�ʱ����Ƕ�������жϿ��������ú͹���ģʽ
 *@attention STM32F103C8T6û�л�����ʱ�������ļ�����ͨ�ö�ʱ��TIM3
 *@author    ���
 *@version   V1.0.0
 *@date      2021-09-01
 */
#include "bsp_tim.h"

uint32_t tim3_count = 0;
/**
 *@brief     ��ʼ��Ƕ�������жϿ�����NVIC
 *@attention ��Ƶ�������ʱ��Ƶ��Ϊ1000Hz
 *@param     ��
 *@retval    ��
 */
static void TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/**����NVICΪ���ȼ���1*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/**��ͨ�ö�ʱ��TIM3��Ϊ�ж�Դ*/
	NVIC_InitStructure.NVIC_IRQChannel = CURRENT_TIM_IRQ;
	/**������ռ���ȼ�Ϊ0*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/**���������ȼ�Ϊ1*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/**ʹ���ж�ͨ��*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/**�����������ó�ʼ��Ƕ�������жϿ�����NVIC*/
	NVIC_Init(&NVIC_InitStructure);
}

/**
 *@brief     ���û�����ʱ���Ĺ���ģʽ
 *@attention �ڲ�ʱ��CK_INTΪ72MHZ��������ʱ����ʱ�ӱ�Ԥ��Ƶ����Ƶ���Ƶ��Ϊ72/��1+BASIC_TIM_Prescaler�� = 1MHZ
 *@param     ��
 *@retval    ��
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	/**������ʱ��ʱ�ӣ�ʱ��Ƶ��Ϊ72MHZ*/	
	CURRENT_TIM_APBxClock_FUN(CURRENT_TIM_CLK, ENABLE);
	TIM_DeInit(CURRENT_TIM);	
	/**�����Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�*/
	TIM_TimeBaseStructure.TIM_Period = CURRENT_TIM_Period;
	/**����ʱ��Ԥ��Ƶ��*/
	TIM_TimeBaseStructure.TIM_Prescaler = CURRENT_TIM_Prescaler;
	/**����ʱ�ӷָ�Ϊ0*/
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	/**���ü���ģʽΪ���ϼ���*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/**�����������ó�ʼ����ʱ��*/
	TIM_TimeBaseInit(CURRENT_TIM, &TIM_TimeBaseStructure);
	/**�����ʱ���жϱ�־λ*/
	TIM_ClearFlag(CURRENT_TIM, TIM_FLAG_Update);
	/**������ʱ���ж�*/
	TIM_ITConfig(CURRENT_TIM, TIM_IT_Update, ENABLE);
	/**ʹ�ܶ�ʱ��*/
	TIM_Cmd(CURRENT_TIM, ENABLE);
}
	
/**
 *@brief     ����ͨ�ö�ʱ���ĳ�ʼ������
 *@attention ��
 *@param     ��
 *@retval    ��
 */
void CURRENT_TIM_Init(void)
{
  TIM_NVIC_Config();
	TIM_Mode_Config();
}
