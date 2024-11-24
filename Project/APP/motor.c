// ******************************************/
// * 车模仅一个电机
// * 接下来统一称右电机
// ******************************************/

#include "headfile.h"

extern uint8 outtrack_flag;				//出赛道标志
extern uint8 out_island_accelerate;
extern uint8 GO_accelerate_flag;
extern int16 temp_right_pluse;							//右电机反馈值

uint8 HALL_Stop_flag = 0;				//霍尔检测 0 为未检测到 1 为已检测到
uint16 HALL_Stop_encoder = 0;
MOTOR_CONTROL Motor_t;

/**
  * @brief   电机初始化。初始化PWM，控制信息，PID
  */
void Motor_Command_Init(void)
{
	pwm_init(PWMA_CH3P_P24, 17000, 0); //频率17000HZ   占空比为0
	pwm_init(PWMA_CH4P_P26, 17000, 0); //频率17000HZ   占空比为0
}
/**
  * @brief   闭环计算电机需要输出的PWM值
  */
void Motor_PWM_Final_Control(void)
{
	if (HALL_PIN == 0) HALL_Stop_flag = 1;		//霍尔检测

	//霍尔传感器为 0 正常驱动系统
	if (0 == HALL_Stop_flag) {
		if (0 == outtrack_flag) {
			if (GO_accelerate_flag == 1) Motor_t.Pwm_MotorR = 3900;
			else if (out_island_accelerate == 1) Motor_t.Pwm_MotorR = 3200;
			else Motor_t.Pwm_MotorR = 2800;
		}
		//出赛道
		else if (1 == outtrack_flag) Motor_t.Pwm_MotorR = 0;
	}
	//霍尔传感器为 1 停车驱动逻辑
	else if (1 == HALL_Stop_flag) {
		// 路程积分记录
		if (1 == HALL_Stop_flag && HALL_Stop_encoder < HALL_Stop_Mileage_Points+1) HALL_Stop_encoder++;
		//反转减速
		if (HALL_Stop_encoder < HALL_Stop_Mileage_Points) Motor_t.Pwm_MotorR = -1500;
		//停车
		else Motor_t.Pwm_MotorR = 0;
	}

	Motor_Right_Command(Motor_t.Pwm_MotorR);     // 电机驱动

}
/**
  * @brief  右电机控制
  * @param  右电机固定的PWM
  */

void Motor_Right_Command(float PWM_Right){

	pwm_init(PWMA_CH3P_P24, 17000, 0);
	if (PWM_Right >= 0)
	{
		pwm_duty(PWMA_CH3P_P24, (int)PWM_Right);
		pwm_duty(PWMA_CH4P_P26, 0);
	}
	//过速保护
	else if (PWM_Right > 6000) {
		pwm_duty(PWMA_CH3P_P24, 6000);
		pwm_duty(PWMA_CH4P_P26, 0);
	}
	// PWM小于零时,反转
	else if (PWM_Right < 0)
	{
		pwm_duty(PWMA_CH3P_P24, 0);
		pwm_duty(PWMA_CH4P_P26, -(int)PWM_Right);
	}
		// PWM小于零时,反转
	else if (PWM_Right < 6000)
	{
		pwm_duty(PWMA_CH3P_P24, 0);
		pwm_duty(PWMA_CH4P_P26, 6000);
	}
}
