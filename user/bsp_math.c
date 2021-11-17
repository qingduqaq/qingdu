#include "bsp_math.h"
#include "bsp_usart.h"

static uint16_t MaxElement(uint16_t data[])
{
	uint8_t i;
	uint16_t max = data[0];
	for(i = 1; i < Size; i++)
	{
		if(data[i] >= max)
		{
			max = data[i];
		}
	}
	return max;
}

static uint16_t MinElement(uint16_t data[])
{
	uint8_t i;
	uint16_t min = data[0];
	for(i = 1; i < Size; i++)
	{
		if(data[i] <= min)
		{
			min = data[i];
		}
	}
	return min;
}
	
//在25Hz循环中运行
void Rate_Calculate(uint16_t data)
{
	static uint8_t index; //当前数组下标
  static uint8_t pulseflag;	
	static uint16_t Input_Data[Size]; //缓存数据数组
	static uint16_t predata, redata; //一个波形周期内读取上一次值、当前值 
	uint16_t Maxdata, Mindata; //最大值，最小值
	static uint16_t Middata; //平均值
	static uint16_t range = 1024; //，幅度
	static uint8_t prepulse, pulse; //衡量指标值
	static uint16_t time1, time2, time;
	uint16_t IBI, BIM;
	
	predata = redata; //保存上一次的值
	redata = data; //读取当前值
  if(redata - predata < range) //滤除突变噪声信号干扰
	{
		Input_Data[index] = redata; //填充缓存数组
		index++;
	}
	if(index >= Size)
	{
		index = 0; //覆盖数组
	  Maxdata = MaxElement(Input_Data); 
		Mindata = MinElement(Input_Data);
		Middata = (Maxdata + Mindata) / 2; //更新参考阈值
		range = (Maxdata - Mindata) / 2; //更新突变阈值
		//printf("当前%d ", Middata);
	}
	
	prepulse = pulse; //保存当前脉冲状态
	pulse = (redata > Middata) ? 1 : 0; //采样值大于中间值为有效脉冲
	if(prepulse == 0 && pulse == 1) //当检测到两个有效脉冲时为一个有效心率周期
	{
		pulseflag++;
		pulseflag = pulseflag % 2;
		//printf("进入循环:%d", pulseflag);
		if(pulseflag == 1) //检测到第一个有效脉冲开始计时
		{
			time1 = time;
		}
		if(pulseflag == 0)
		{
			time2 = time;
			time = 0; //清空计时
		  if(time1 < time2)
			{
				IBI = (time2 - time1) * Period; //计算两个有效脉冲的时间间隔
				BIM = 60000 / IBI; //1分钟有60000ms
				/**限制BIM最高/最低值*/
				if(BIM > 200)
				{
					BIM = 200;
				}
				if(BIM < 30)
				{
					BIM = 30;
				}
				printf("当前心率值为%d ", BIM);
			}
		}
	}
	time++;
}
