#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "headfile.h"

/* 霍尔传感器引脚 */
#define HALL_PIN P44
/* 停车路程积分 */
#define HALL_Stop_Mileage_Points 1300

typedef struct {
	uint16 Pwm_MotorR;				// 舵机输出
}MOTOR_CONTROL;

extern MOTOR_CONTROL Motor_t;

void Motor_Command_Init(void);
void Motor_PWM_Final_Control(void);
void Motor_Right_Command(float PWM_Right);

#endif