/**
 *@file      main.c
 *@brief     ADC多通道采集电压，采用DMA直接存储器读取的方式从外设采集电压值
 *@details   实现开发板上电位器电压的采集并通过串口打印至 PC 端串口调试助手,
             取代了单通道实验使用中断服务的读取方法。实际上，多通道ADC采集
						 一般使用DMA直接存储器读取方式更加高效方便
 *@author    青渡
 *@version   V1.0.0
 *@date      2021-07-30
 */
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_adc.h"
#include "bsp_tim.h"
#include "bsp_filter.h"
#include "bsp_math.h"

/**将ADC_ConvertValue设为外部变量，其已经在bsp_adc.c文件中声明*/
extern __IO uint16_t ADC_ConvertValue[NUMCHANNEL];
int ADC_ConvertValueLocal[NUMCHANNEL]; //float

/**基本延时函数*/
void delay(__IO uint32_t nCount)
{
	while(--nCount);
}

/**
 *@brief     主函数，实现ADC电压采集并通过串口将采集数据打印在上位机上
 *@attention ADCx_Init()函数不需要放在while(1)循环内部，ADC开启了连续转换模式,
             会一直读取ADC采集电压值，放在循环内部还会占用CPU时序
 *@param     无
 *@retval    无
 */
int main(void)
{
	int a1, a2, a3, a4;
	/**初始化Usart和ADC*/
	Usart_Config();
	ADCx_Init();
	CURRENT_TIM_Init();
	/**电压转换：模拟电压经过ADC转换后，是一个12位的数字值，ADC电压输入范围为0~3.3V,
  12位满量程对应的电压值即为3.3V，即实际电压值X与测量值Y的关系为：X = Y * 2^12 / 3.3*/
	while(1)
	{
		//10ms传一个数据
		if(tim3_count >= 20)
		{ 	
		  ADC_ConvertValueLocal[2] = (int16_t)ADC_ConvertValue[2];
			//ADC_ConvertValueLocal[2] = (uint16_t)ADC_ConvertValueLocal[2];
			a1 = ADC_ConvertValueLocal[2] / 1000;
		  a2 = ADC_ConvertValueLocal[2] % 1000 / 100;
		  a3 = ADC_ConvertValueLocal[2] % 1000 % 100 / 10;
		  a4 = ADC_ConvertValueLocal[2] % 1000 % 100 % 10;
			Rate_Calculate(ADC_ConvertValueLocal[2]);
			printf("AAA%d%d%d%d", a1, a2, a3, a4);//化成0~3.3试试？或许可以在上位机做
			//SG_Filter(ADC_ConvertValueLocal[2]);
			tim3_count = 0;
		}
	}
}
