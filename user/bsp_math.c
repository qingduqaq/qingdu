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
	
//��25Hzѭ��������
void Rate_Calculate(uint16_t data)
{
	static uint8_t index; //��ǰ�����±�
  static uint8_t pulseflag;	
	static uint16_t Input_Data[Size]; //������������
	static uint16_t predata, redata; //һ�����������ڶ�ȡ��һ��ֵ����ǰֵ 
	uint16_t Maxdata, Mindata; //���ֵ����Сֵ
	static uint16_t Middata; //ƽ��ֵ
	static uint16_t range = 1024; //������
	static uint8_t prepulse, pulse; //����ָ��ֵ
	static uint16_t time1, time2, time;
	uint16_t IBI, BIM;
	
	predata = redata; //������һ�ε�ֵ
	redata = data; //��ȡ��ǰֵ
  if(redata - predata < range) //�˳�ͻ�������źŸ���
	{
		Input_Data[index] = redata; //��仺������
		index++;
	}
	if(index >= Size)
	{
		index = 0; //��������
	  Maxdata = MaxElement(Input_Data); 
		Mindata = MinElement(Input_Data);
		Middata = (Maxdata + Mindata) / 2; //���²ο���ֵ
		range = (Maxdata - Mindata) / 2; //����ͻ����ֵ
		//printf("��ǰ%d ", Middata);
	}
	
	prepulse = pulse; //���浱ǰ����״̬
	pulse = (redata > Middata) ? 1 : 0; //����ֵ�����м�ֵΪ��Ч����
	if(prepulse == 0 && pulse == 1) //����⵽������Ч����ʱΪһ����Ч��������
	{
		pulseflag++;
		pulseflag = pulseflag % 2;
		//printf("����ѭ��:%d", pulseflag);
		if(pulseflag == 1) //��⵽��һ����Ч���忪ʼ��ʱ
		{
			time1 = time;
		}
		if(pulseflag == 0)
		{
			time2 = time;
			time = 0; //��ռ�ʱ
		  if(time1 < time2)
			{
				IBI = (time2 - time1) * Period; //����������Ч�����ʱ����
				BIM = 60000 / IBI; //1������60000ms
				/**����BIM���/���ֵ*/
				if(BIM > 200)
				{
					BIM = 200;
				}
				if(BIM < 30)
				{
					BIM = 30;
				}
				printf("��ǰ����ֵΪ%d ", BIM);
			}
		}
	}
	time++;
}
