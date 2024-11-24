// ******************************************/
// * ��ģ��һ�����
// * ������ͳһ���ҵ��
// ******************************************/

#include "headfile.h"

extern uint8 outtrack_flag;				//��������־
extern uint8 out_island_accelerate;
extern uint8 GO_accelerate_flag;
extern int16 temp_right_pluse;							//�ҵ������ֵ

uint8 HALL_Stop_flag = 0;				//������� 0 Ϊδ��⵽ 1 Ϊ�Ѽ�⵽
uint16 HALL_Stop_encoder = 0;
MOTOR_CONTROL Motor_t;

/**
  * @brief   �����ʼ������ʼ��PWM��������Ϣ��PID
  */
void Motor_Command_Init(void)
{
	pwm_init(PWMA_CH3P_P24, 17000, 0); //Ƶ��17000HZ   ռ�ձ�Ϊ0
	pwm_init(PWMA_CH4P_P26, 17000, 0); //Ƶ��17000HZ   ռ�ձ�Ϊ0
}
/**
  * @brief   �ջ���������Ҫ�����PWMֵ
  */
void Motor_PWM_Final_Control(void)
{
	if (HALL_PIN == 0) HALL_Stop_flag = 1;		//�������

	//����������Ϊ 0 ��������ϵͳ
	if (0 == HALL_Stop_flag) {
		if (0 == outtrack_flag) {
			if (GO_accelerate_flag == 1) Motor_t.Pwm_MotorR = 3900;
			else if (out_island_accelerate == 1) Motor_t.Pwm_MotorR = 3200;
			else Motor_t.Pwm_MotorR = 2800;
		}
		//������
		else if (1 == outtrack_flag) Motor_t.Pwm_MotorR = 0;
	}
	//����������Ϊ 1 ͣ�������߼�
	else if (1 == HALL_Stop_flag) {
		// ·�̻��ּ�¼
		if (1 == HALL_Stop_flag && HALL_Stop_encoder < HALL_Stop_Mileage_Points+1) HALL_Stop_encoder++;
		//��ת����
		if (HALL_Stop_encoder < HALL_Stop_Mileage_Points) Motor_t.Pwm_MotorR = -1500;
		//ͣ��
		else Motor_t.Pwm_MotorR = 0;
	}

	Motor_Right_Command(Motor_t.Pwm_MotorR);     // �������

}
/**
  * @brief  �ҵ������
  * @param  �ҵ���̶���PWM
  */

void Motor_Right_Command(float PWM_Right){

	pwm_init(PWMA_CH3P_P24, 17000, 0);
	if (PWM_Right >= 0)
	{
		pwm_duty(PWMA_CH3P_P24, (int)PWM_Right);
		pwm_duty(PWMA_CH4P_P26, 0);
	}
	//���ٱ���
	else if (PWM_Right > 6000) {
		pwm_duty(PWMA_CH3P_P24, 6000);
		pwm_duty(PWMA_CH4P_P26, 0);
	}
	// PWMС����ʱ,��ת
	else if (PWM_Right < 0)
	{
		pwm_duty(PWMA_CH3P_P24, 0);
		pwm_duty(PWMA_CH4P_P26, -(int)PWM_Right);
	}
		// PWMС����ʱ,��ת
	else if (PWM_Right < 6000)
	{
		pwm_duty(PWMA_CH3P_P24, 0);
		pwm_duty(PWMA_CH4P_P26, 6000);
	}
}
