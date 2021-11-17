/**
 *@file      bsp_adc.c
 *@brief     初始化DMA存储器直接读取，GPIO，电压采集等
 *@details   无
 *@author    青渡
 *@version   V1.0.0
 *@date      2021-07-30
 */
#include "bsp_adc.h"

__IO uint16_t ADC_ConvertValue[NUMCHANNEL] = {0};

/**
 *@brief     初始化电压采集GPIO端口的配置
 *@attention 用作ADC采集的IO必须不被其他外设复用，否则会对采集电压造成数值上的影响
 *@param     无
 *@retval    无
 */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/**开启GPIO端口时钟*/
	ADC_GPIO_APBxClock_Cmd(ADC_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin = ADC_PIN1 | ADC_PIN2;
	/**将GPIO工作模式设为模拟输入模式*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADC_PIN3 | ADC_PIN4 | ADC_PIN5 | ADC_PIN6;
	/**将GPIO工作模式设为模拟输入模式*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_PORT1, &GPIO_InitStructure);
}

/**
 *@brief     初始化ADC、DMA工作模式配置
 *@attention 无
 *@param     无
 *@retval    无
 */
static void ADCx_Mode_Conifg(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/**开启DMA和ADC的时钟*/
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	ADC_APBxClock_Cmd(ADC_CLK, ENABLE);
	/**将DMA通道配置寄存器重设为默认值*/
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	/**外设地址为规则数据寄存器ADC_DR*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)( & (ADC_x ->DR));
	/**存储器地址为ADC_ConvertValue*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertValue;
	/**数据读取方向为外设到存储器*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/**数据缓冲区大小为存储器的大小NUMCHANNEL*/
	DMA_InitStructure.DMA_BufferSize = NUMCHANNEL;
	/**外设寄存器ADC_DR地址值保持不变，地址不用递增*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	/**存储器地址递增*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/**外设数据大小为半字，即两个字节*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	/**内存数据大小也为半字，跟外设数据大小相同*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	/**将DMA工作模式设置为循环传输模式*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	/**DMA传输通道优先级为高*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/**读取数据方向是从外设到存储器，所以禁用存储器到存储器模式*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	/**按照上述配置初始化DMA*/
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	/**使能DMA通道*/	
	DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
	
	/**将ADC工作模式设为独立工作模式*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	/**在多通道ADC电压采集中开启扫描模式，在单通道电压采集中禁用*/
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	/**开启连接转换模式*/
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	/**不用外部触发转换，采用软件开启的方式*/
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	/**转换结果右对齐*/
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	/**转换通道个数为NUMCHANNEL*/
	ADC_InitStructure.ADC_NbrOfChannel = NUMCHANNEL;
	/**按照上述配置初始化ADC*/
	ADC_Init(ADC_x, &ADC_InitStructure);
	/**配置ADC时钟为PCLK2的8分频，即9MHZ*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	/**配置ADC通道转换顺序和采样时间，其中采样时间为55.5个周期*/
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
	
	/**使能DMA直接存储器读取，不用使能中断*/
	ADC_DMACmd(ADC_x, ENABLE);
	/**使能ADC1*/
	ADC_Cmd(ADC_x, ENABLE);
	/**开启ADC，并开始转换*/
	ADC_ResetCalibration(ADC_x);
	/**等待重置完成*/
	while(ADC_GetResetCalibrationStatus(ADC_x));
	/**启动所选定的ADC的校准过程*/
	ADC_StartCalibration(ADC_x);
	/**等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC_x));
	/**启用ADC软件触发转换*/
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

/**
 *@brief     ADC功能初始化配置
 *@attention 集成上述两个初始化函数
 *@param     无
 *@retval    无
 */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Conifg();
}
