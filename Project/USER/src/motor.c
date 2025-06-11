// ******************************************/
// * ��ģ��һ�����
// * ������ͳһ���ҵ��
// ******************************************/

#include "headfile.h"

extern uint8 outtrack_flag;				//��������־

/**
  * @brief   �����ʼ������ʼ��PWM��������Ϣ��PID
  */
void Motor_Command_Init(void)
{
	pwm_init(PWMA_CH3P_P24, 17000, 0); //Ƶ��17000HZ   ռ�ձ�Ϊ0
	pwm_init(PWMA_CH4P_P26, 17000, 0); //Ƶ��17000HZ   ռ�ձ�Ϊ
}

/**
  * @brief   �ջ���������Ҫ�����PWMֵ
  */
void Motor_PWM_Final_Control()
{
	static float Round_PWM_R;
	if (outtrack_flag == 1) {			//������ͣ����ֹײ����к��𻵵��
	
		Motor_Right_Command(0);			//ͣ��
	}
	else if (outtrack_flag == 0){		//δ������
	
		Motor_Right_Command(2000);		//1500->2000
	}
}

/**
  * @brief  �ҵ������
  * @param  �ҵ���̶���PWM
  * @addition �ޱ����� �޷������ջ�����д��������
  */

void Motor_Right_Command(float PWM_Right){
	if (PWM_Right >= 0)
	{
		pwm_init(PWMA_CH3P_P24, 17000, 0);
		pwm_duty(PWMA_CH4P_P26, (int)PWM_Right);
	}
	else if (PWM_Right > 3000) {
		pwm_init(PWMA_CH3P_P24, 17000, 0);
		pwm_duty(PWMA_CH4P_P26, 2500);
	}
	// PWMС����ʱ����������
	else if (PWM_Right < 0)
	{
		pwm_init(PWMA_CH3P_P24, 17000, PWM_DUTY_MAX);
		pwm_duty(PWMA_CH4P_P26, -(int)PWM_Right);
	}
}