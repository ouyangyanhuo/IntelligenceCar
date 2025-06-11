// ******************************************/
// * 车模仅一个电机
// * 接下来统一称右电机
// ******************************************/

#include "headfile.h"

extern uint8 outtrack_flag;				//出赛道标志

/**
  * @brief   电机初始化。初始化PWM，控制信息，PID
  */
void Motor_Command_Init(void)
{
	pwm_init(PWMA_CH3P_P24, 17000, 0); //频率17000HZ   占空比为0
	pwm_init(PWMA_CH4P_P26, 17000, 0); //频率17000HZ   占空比为
}

/**
  * @brief   闭环计算电机需要输出的PWM值
  */
void Motor_PWM_Final_Control()
{
	static float Round_PWM_R;
	if (outtrack_flag == 1) {			//出赛道停车防止撞坏电感和损坏电机
	
		Motor_Right_Command(0);			//停车
	}
	else if (outtrack_flag == 0){		//未出赛道
	
		Motor_Right_Command(2000);		//1500->2000
	}
}

/**
  * @brief  右电机控制
  * @param  右电机固定的PWM
  * @addition 无编码器 无法做到闭环，仅写保护程序
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
	// PWM小于零时，保护控制
	else if (PWM_Right < 0)
	{
		pwm_init(PWMA_CH3P_P24, 17000, PWM_DUTY_MAX);
		pwm_duty(PWMA_CH4P_P26, -(int)PWM_Right);
	}
}