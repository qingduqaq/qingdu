/**
 *@file      main.c
 *@brief     ADC��ͨ���ɼ���ѹ������DMAֱ�Ӵ洢����ȡ�ķ�ʽ������ɼ���ѹֵ
 *@details   ʵ�ֿ������ϵ�λ����ѹ�Ĳɼ���ͨ�����ڴ�ӡ�� PC �˴��ڵ�������,
             ȡ���˵�ͨ��ʵ��ʹ���жϷ���Ķ�ȡ������ʵ���ϣ���ͨ��ADC�ɼ�
						 һ��ʹ��DMAֱ�Ӵ洢����ȡ��ʽ���Ӹ�Ч����
 *@author    ���
 *@version   V1.0.0
 *@date      2021-07-30
 */
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_tim.h"
#include "bsp_filter.h"
#include "bsp_math.h"

/**��ADC_ConvertValue��Ϊ�ⲿ���������Ѿ���bsp_adc.c�ļ�������*/
extern __IO uint16_t ADC_ConvertValue[NUMCHANNEL];
int ADC_ConvertValueLocal[NUMCHANNEL]; //float

/**������ʱ����*/
void delay(__IO uint32_t nCount)
{
	while(--nCount);
}

/**
 *@brief     ��������ʵ��ADC��ѹ�ɼ���ͨ�����ڽ��ɼ����ݴ�ӡ����λ����
 *@attention ADCx_Init()��������Ҫ����while(1)ѭ���ڲ���ADC����������ת��ģʽ,
             ��һֱ��ȡADC�ɼ���ѹֵ������ѭ���ڲ�����ռ��CPUʱ��
 *@param     ��
 *@retval    ��
 */
int main(void)
{
	int a1, a2, a3, a4;
	/**��ʼ��Usart��ADC*/
	Usart_Config();
	ADCx_Init();
	CURRENT_TIM_Init();
	/**��ѹת����ģ���ѹ����ADCת������һ��12λ������ֵ��ADC��ѹ���뷶ΧΪ0~3.3V,
  12λ�����̶�Ӧ�ĵ�ѹֵ��Ϊ3.3V����ʵ�ʵ�ѹֵX�����ֵY�Ĺ�ϵΪ��X = Y * 2^12 / 3.3*/
	while(1)
	{
		//10ms��һ������
		if(tim3_count >= 20)
		{ 	
		  ADC_ConvertValueLocal[2] = (int16_t)ADC_ConvertValue[2];
			//ADC_ConvertValueLocal[2] = (uint16_t)ADC_ConvertValueLocal[2];
			a1 = ADC_ConvertValueLocal[2] / 1000;
		  a2 = ADC_ConvertValueLocal[2] % 1000 / 100;
		  a3 = ADC_ConvertValueLocal[2] % 1000 % 100 / 10;
		  a4 = ADC_ConvertValueLocal[2] % 1000 % 100 % 10;
			Rate_Calculate(ADC_ConvertValueLocal[2]);
			printf("AAA%d%d%d%d", a1, a2, a3, a4);//����0~3.3���ԣ������������λ����
			//SG_Filter(ADC_ConvertValueLocal[2]);
			tim3_count = 0;
		}
	}
}
