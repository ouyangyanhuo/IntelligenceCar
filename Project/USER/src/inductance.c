#include "headfile.h"

/*
��������
ADCN_enum adcn ������ָ�ӿ�
*/

long AD_Bias = 0, AD_Bias_last = 0;    //��Ⱥ��㷨���ƫ��ֵ
int16 adc_deal_last[4];					//�����вɼ�ֵ
//������long���ͣ�������ֱ��뾯��
uint16 Left_Adc = 0, Right_Adc = 0;		//���ֵ
uint16 Left_Adc2 = 0, Right_Adc2 = 0;		//���ֵ
uint16 Middle_Adc = 0;					//�м������

int16 adc_max[4] = { 300,300,300,300 };	//��һ��������ֵ����һ��һ����Ҷ�
long ElectromaError_Out_Value = 0;		//�����������еĲ�Ⱥ�

int16 debug_left, debug_right, debug_left2, debug_right2, debug_middle; //Debug��
/**
  * @brief  ��вɼ���ʼ������
  * @param   ��
  * @param	 ��
  * @retval  ��
  */
void Adinductance_init(void) //ADC��ʼ��
{
	adc_init(ADC_P06, ADC_SYSclk_DIV_2);   //��һ��� L1
	adc_init(ADC_P15, ADC_SYSclk_DIV_2);   //��һ��� L7
	adc_init(ADC_P11, ADC_SYSclk_DIV_2);   //������ L2
	adc_init(ADC_P14, ADC_SYSclk_DIV_2);   //�Ҷ���� L6
}

/**
  * @brief   ȡ��ֵ�˲�����
  * @param   ��ȡ���ĵ������
  * @param   ��
  * @retval  ����ȥ����ֵ���ƽ��ֵ
  */
int16 ADC_Del_MaxMin_Average_Filter(int16* ADC) //ȥ�����ֵ����Сֵ���������������飬�洢���ǵ��ֵ��
{
	uint8 i;
	int16 max, min, average = 0;  //���弫ֵ�Լ���
	uint16 sum = 0;

	max = ADC[0]; //��ʼ����Сֵ�����ֵ
	min = ADC[0];

	for (i = 0; i < sizeof(ADC); i++) // sizeof(ADC)�Ǵ������ĵ������ĳ���
	{
		if (max < ADC[i])	max = ADC[i];  //�ҳ����ĵ��
		if (min > ADC[i])	min = ADC[i];  //�ҳ���С�ĵ��
		sum += ADC[i];  //�ۼ�
	}

	average = (sum - max - min) / (sizeof(ADC) - 2);    //��е��ۼ�ֵ��ȥ���ֵ����Сֵ ����ƽ��
	return average;       //��ȥ��ֵ��ֵ����ԭ����
}

//����������Ƭ����

/**
  * @brief   ���ֵ�����������������ȡ��ȥ��ֵ����ƽ��ֵ�� ��һ������Ⱥ�
  * @param   ��
  * @param   ��
  * @retval  ��
  */
void ADC_Final_Read_Deal() {
	int16 AD_Bias_Temp1, AD_Bias_Temp2;
	uint8 i;
	//FILTER_N ������ inductance.h ͷ�ļ��У�Ϊ�˲����
	int16 filter_buf_L1[FILTER_N];	//��һ��д洢����
	int16 filter_buf_R1[FILTER_N];	//��һ��д�������
	int16 filter_buf_L2[FILTER_N];	//�����д洢����
	int16 filter_buf_R2[FILTER_N];	//�Ҷ���д�������

	//--------����--------------
	for (i = 0; i < FILTER_N; i++)   //��ֵ������5�� 
	{
		filter_buf_L1[i] = adc_once(ADC_P06, ADC_12BIT);  //��һ�������
		filter_buf_R1[i] = adc_once(ADC_P15, ADC_12BIT);  //��һ�������
		filter_buf_L2[i] = adc_once(ADC_P11, ADC_12BIT);  //����������
		filter_buf_R2[i] = adc_once(ADC_P14, ADC_12BIT);  //�Ҷ��������
	}

	//���Ա�����ǰ�ڵ��Ե����ʾ��
	debug_left = filter_buf_L1[3];
	debug_right = filter_buf_R1[3];
	debug_left2 = filter_buf_L2[3];
	debug_right2 = filter_buf_R2[3];
	debug_middle = filter_buf_L2[3]+ filter_buf_R2[3];

	//--------ȥ��ֵ��ƽ��---------
	adc_deal_last[LEFT_1] = ADC_Del_MaxMin_Average_Filter(filter_buf_L1);  //��һ�������ֵ      
	adc_deal_last[RIGHT_1] = ADC_Del_MaxMin_Average_Filter(filter_buf_R1); //��һ�������ֵ
	adc_deal_last[LEFT_2] = ADC_Del_MaxMin_Average_Filter(filter_buf_L2);  //����������ֵ   
	adc_deal_last[RIGHT_2] = ADC_Del_MaxMin_Average_Filter(filter_buf_R2); //�Ҷ��������ֵ

	//��һ�����ֵ
	Left_Adc = (adc_deal_last[LEFT_1] * 100) / adc_max[0];
	Right_Adc = (adc_deal_last[RIGHT_1] * 100) / adc_max[1];
	Left_Adc2 = (adc_deal_last[LEFT_2] * 100) / adc_max[2];
	Right_Adc2 = (adc_deal_last[RIGHT_2] * 100) / adc_max[3];

	//����м���
	Middle_Adc = Left_Adc2 + Right_Adc2;
	
	//����޷�����
	Left_Adc = ADC_Limit(Left_Adc, ADC_MAX, ADC_MIN);
	Right_Adc = ADC_Limit(Right_Adc, ADC_MAX, ADC_MIN);
	Left_Adc2 = ADC_Limit(Left_Adc2, ADC_MAX, ADC_MIN);
	Right_Adc2 = ADC_Limit(Right_Adc2, ADC_MAX, ADC_MIN);
	Middle_Adc = ADC_Limit(Middle_Adc, ADC_MAX, ADC_MIN);

	//�ĵ���ں����ֵ
	AD_Bias_Temp1 = ((Left_Adc + Left_Adc2) - (Right_Adc + Right_Adc2)) * 100 / ((Left_Adc + Left_Adc2) + (Right_Adc + Right_Adc2));
	AD_Bias_Temp2 = ((Right_Adc + Right_Adc2) - (Left_Adc + Left_Adc2)) * 100 / ((Right_Adc + Right_Adc2) + (Left_Adc + Left_Adc2));
	//��תС ��ת��
	AD_Bias = AD_Bias_Temp1 - AD_Bias_Temp2;
	ElectromaError_Out_Value = AD_Bias_Temp2*2;

}


/**
  * @brief  ��й�һ���޷�����,���������ֵ�޷���0-100��Χ��
  * @param   �����ADCֵ
  * @param   �޷����ֵ
  * @retval  ��Сֵ
  */
int16 ADC_Limit(int16 in_adc, int8 max, int8 min)
{
	uint16 Adc_Input;
	Adc_Input = in_adc;
	if (Adc_Input >= max)       Adc_Input = max;
	else if (Adc_Input <= min)  Adc_Input = min;
	return Adc_Input;
}