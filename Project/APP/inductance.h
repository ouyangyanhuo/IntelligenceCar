#ifndef __INDUCTANCE_H_
#define __INDUCTANCE_H_

#include "headfile.h"

#define FILTER_N 10 //滤波深度

//定义几个数组下标，便于直观理解每一个电感值
#define LEFT_1   0
#define RIGHT_1  1
#define LEFT_2  2
#define RIGHT_2  3

//定义归一化电感的阈值
#define ADC_MAX  100
#define ADC_MIN  0

void Adinductance_init(void);
int16 ADC_Del_MaxMin_Average_Filter(int16* ADC);
void ADC_Final_Read_Deal();
int16 ADC_Limit(int16 in_adc, int8 max, int8 min);

#endif