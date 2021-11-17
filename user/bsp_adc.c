/**
 *@file      bsp_adc.c
 *@brief     ��ʼ��DMA�洢��ֱ�Ӷ�ȡ��GPIO����ѹ�ɼ���
 *@details   ��
 *@author    ���
 *@version   V1.0.0
 *@date      2021-07-30
 */
#include "bsp_adc.h"

__IO uint16_t ADC_ConvertValue[NUMCHANNEL] = {0};

/**
 *@brief     ��ʼ����ѹ�ɼ�GPIO�˿ڵ�����
 *@attention ����ADC�ɼ���IO���벻���������踴�ã������Բɼ���ѹ�����ֵ�ϵ�Ӱ��
 *@param     ��
 *@retval    ��
 */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/**����GPIO�˿�ʱ��*/
	ADC_GPIO_APBxClock_Cmd(ADC_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin = ADC_PIN1 | ADC_PIN2;
	/**��GPIO����ģʽ��Ϊģ������ģʽ*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADC_PIN3 | ADC_PIN4 | ADC_PIN5 | ADC_PIN6;
	/**��GPIO����ģʽ��Ϊģ������ģʽ*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_PORT1, &GPIO_InitStructure);
}

/**
 *@brief     ��ʼ��ADC��DMA����ģʽ����
 *@attention ��
 *@param     ��
 *@retval    ��
 */
static void ADCx_Mode_Conifg(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/**����DMA��ADC��ʱ��*/
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	ADC_APBxClock_Cmd(ADC_CLK, ENABLE);
	/**��DMAͨ�����üĴ�������ΪĬ��ֵ*/
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	/**�����ַΪ�������ݼĴ���ADC_DR*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)( & (ADC_x ->DR));
	/**�洢����ַΪADC_ConvertValue*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertValue;
	/**���ݶ�ȡ����Ϊ���赽�洢��*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/**���ݻ�������СΪ�洢���Ĵ�СNUMCHANNEL*/
	DMA_InitStructure.DMA_BufferSize = NUMCHANNEL;
	/**����Ĵ���ADC_DR��ֵַ���ֲ��䣬��ַ���õ���*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	/**�洢����ַ����*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/**�������ݴ�СΪ���֣��������ֽ�*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	/**�ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	/**��DMA����ģʽ����Ϊѭ������ģʽ*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	/**DMA����ͨ�����ȼ�Ϊ��*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/**��ȡ���ݷ����Ǵ����赽�洢�������Խ��ô洢�����洢��ģʽ*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	/**�����������ó�ʼ��DMA*/
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	/**ʹ��DMAͨ��*/	
	DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
	
	/**��ADC����ģʽ��Ϊ��������ģʽ*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	/**�ڶ�ͨ��ADC��ѹ�ɼ��п���ɨ��ģʽ���ڵ�ͨ����ѹ�ɼ��н���*/
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	/**��������ת��ģʽ*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	/**�����ⲿ����ת����������������ķ�ʽ*/
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	/**ת������Ҷ���*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	/**ת��ͨ������ΪNUMCHANNEL*/
	ADC_InitStructure.ADC_NbrOfChannel = NUMCHANNEL;
	/**�����������ó�ʼ��ADC*/
	ADC_Init(ADC_x, &ADC_InitStructure);
	/**����ADCʱ��ΪPCLK2��8��Ƶ����9MHZ*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	/**����ADCͨ��ת��˳��Ͳ���ʱ�䣬���в���ʱ��Ϊ55.5������*/
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
	
	/**ʹ��DMAֱ�Ӵ洢����ȡ������ʹ���ж�*/
	ADC_DMACmd(ADC_x, ENABLE);
	/**ʹ��ADC1*/
	ADC_Cmd(ADC_x, ENABLE);
	/**����ADC������ʼת��*/
	ADC_ResetCalibration(ADC_x);
	/**�ȴ��������*/
	while(ADC_GetResetCalibrationStatus(ADC_x));
	/**������ѡ����ADC��У׼����*/
	ADC_StartCalibration(ADC_x);
	/**�ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC_x));
	/**����ADC�������ת��*/
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

/**
 *@brief     ADC���ܳ�ʼ������
 *@attention ��������������ʼ������
 *@param     ��
 *@retval    ��
 */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Conifg();
}
