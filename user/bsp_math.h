#ifndef __bsp_math_H
#define __bsp_math_H

#include "stm32f10x.h"

#define  Size          50
//采样周期为40ms
#define  Period        20

//另一种采样时间 20 40DATA_SIZE 50 25
void Rate_Calculate(uint16_t data);

#endif
