#include "headfile.h"

/*
代码声明
ADCN_enum adcn 参数是指接口
*/

long AD_Bias = 0, AD_Bias_last = 0;    //差比和算法后的偏差值
int16 adc_deal_last[4];					//储存电感采集值
//可能是long类型，但会出现编译警告
uint16 Left_Adc = 0, Right_Adc = 0;		//电感值
uint16 Left_Adc2 = 0, Right_Adc2 = 0;		//电感值
uint16 Middle_Adc = 0;					//中间电感拟合

int16 adc_max[4] = { 300,300,300,300 };	//归一化电感最大值，左一右一左二右二
long ElectromaError_Out_Value = 0;		//两边两个横电感的差比和

int16 debug_left, debug_right, debug_left2, debug_right2, debug_middle; //Debug用
/**
  * @brief  电感采集初始化函数
  * @param   无
  * @param	 无
  * @retval  无
  */
void Adinductance_init(void) //ADC初始化
{
	adc_init(ADC_P06, ADC_SYSclk_DIV_2);   //左一电感 L1
	adc_init(ADC_P15, ADC_SYSclk_DIV_2);   //右一电感 L7
	adc_init(ADC_P11, ADC_SYSclk_DIV_2);   //左二电感 L2
	adc_init(ADC_P14, ADC_SYSclk_DIV_2);   //右二电感 L6
}

/**
  * @brief   取极值滤波函数
  * @param   读取到的电感数组
  * @param   无
  * @retval  返回去除极值后的平均值
  */
int16 ADC_Del_MaxMin_Average_Filter(int16* ADC) //去掉最大值和最小值（传进来的是数组，存储的是电感值）
{
	uint8 i;
	int16 max, min, average = 0;  //定义极值以及和
	uint16 sum = 0;

	max = ADC[0]; //初始化最小值和最大值
	min = ADC[0];

	for (i = 0; i < sizeof(ADC); i++) // sizeof(ADC)是传进来的电感数组的长度
	{
		if (max < ADC[i])	max = ADC[i];  //找出最大的电感
		if (min > ADC[i])	min = ADC[i];  //找出最小的电感
		sum += ADC[i];  //累加
	}

	average = (sum - max - min) / (sizeof(ADC) - 2);    //电感的累加值减去最大值和最小值 再求平均
	return average;       //将去极值的值传回原函数
}

//接下来是正片部分

/**
  * @brief   电感值的最后处理函数，包括读取、去极值、求平均值、 归一化、差比和
  * @param   无
  * @param   无
  * @retval  无
  */
void ADC_Final_Read_Deal() {
	int16 AD_Bias_Temp1, AD_Bias_Temp2;
	uint8 i;
	//FILTER_N 定义在 inductance.h 头文件中，为滤波深度
	int16 filter_buf_L1[FILTER_N];	//左一电感存储数组
	int16 filter_buf_R1[FILTER_N];	//右一电感储存数组
	int16 filter_buf_L2[FILTER_N];	//左二电感存储数组
	int16 filter_buf_R2[FILTER_N];	//右二电感储存数组

	//--------采样--------------
	for (i = 0; i < FILTER_N; i++)   //采值，采样5次 
	{
		filter_buf_L1[i] = adc_once(ADC_P06, ADC_12BIT);  //左一电感数组
		filter_buf_R1[i] = adc_once(ADC_P15, ADC_12BIT);  //右一电感数组
		filter_buf_L2[i] = adc_once(ADC_P11, ADC_12BIT);  //左二电感数组
		filter_buf_R2[i] = adc_once(ADC_P14, ADC_12BIT);  //右二电感数组
	}

	//调试变量，前期调试电感显示用
	debug_left = filter_buf_L1[3];
	debug_right = filter_buf_R1[3];
	debug_left2 = filter_buf_L2[3];
	debug_right2 = filter_buf_R2[3];
	debug_middle = filter_buf_L2[3]+ filter_buf_R2[3];

	//--------去极值求平均---------
	adc_deal_last[LEFT_1] = ADC_Del_MaxMin_Average_Filter(filter_buf_L1);  //左一电感最终值      
	adc_deal_last[RIGHT_1] = ADC_Del_MaxMin_Average_Filter(filter_buf_R1); //右一电感最终值
	adc_deal_last[LEFT_2] = ADC_Del_MaxMin_Average_Filter(filter_buf_L2);  //左二电感最终值   
	adc_deal_last[RIGHT_2] = ADC_Del_MaxMin_Average_Filter(filter_buf_R2); //右二电感最终值

	//归一化电感值
	Left_Adc = (adc_deal_last[LEFT_1] * 100) / adc_max[0];
	Right_Adc = (adc_deal_last[RIGHT_1] * 100) / adc_max[1];
	Left_Adc2 = (adc_deal_last[LEFT_2] * 100) / adc_max[2];
	Right_Adc2 = (adc_deal_last[RIGHT_2] * 100) / adc_max[3];

	//拟合中间电感
	Middle_Adc = Left_Adc2 + Right_Adc2;
	
	//电感限幅处理
	Left_Adc = ADC_Limit(Left_Adc, ADC_MAX, ADC_MIN);
	Right_Adc = ADC_Limit(Right_Adc, ADC_MAX, ADC_MIN);
	Left_Adc2 = ADC_Limit(Left_Adc2, ADC_MAX, ADC_MIN);
	Right_Adc2 = ADC_Limit(Right_Adc2, ADC_MAX, ADC_MIN);
	Middle_Adc = ADC_Limit(Middle_Adc, ADC_MAX, ADC_MIN);

	//四电感融合求差值
	AD_Bias_Temp1 = ((Left_Adc + Left_Adc2) - (Right_Adc + Right_Adc2)) * 100 / ((Left_Adc + Left_Adc2) + (Right_Adc + Right_Adc2));
	AD_Bias_Temp2 = ((Right_Adc + Right_Adc2) - (Left_Adc + Left_Adc2)) * 100 / ((Right_Adc + Right_Adc2) + (Left_Adc + Left_Adc2));
	//右转小 左转大
	AD_Bias = AD_Bias_Temp1 - AD_Bias_Temp2;
	ElectromaError_Out_Value = AD_Bias_Temp2*2;

}


/**
  * @brief  电感归一化限幅函数,见电感最终值限幅在0-100范围内
  * @param   输入的ADC值
  * @param   限幅最大值
  * @retval  最小值
  */
int16 ADC_Limit(int16 in_adc, int8 max, int8 min)
{
	uint16 Adc_Input;
	Adc_Input = in_adc;
	if (Adc_Input >= max)       Adc_Input = max;
	else if (Adc_Input <= min)  Adc_Input = min;
	return Adc_Input;
}