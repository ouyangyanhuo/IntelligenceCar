#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "headfile.h"

/* �������������� */
#define HALL_PIN P44
/* ͣ��·�̻��� */
#define HALL_Stop_Mileage_Points 1300

typedef struct {
	uint16 Pwm_MotorR;				// ������
}MOTOR_CONTROL;

extern MOTOR_CONTROL Motor_t;

void Motor_Command_Init(void);
void Motor_PWM_Final_Control(void);
void Motor_Right_Command(float PWM_Right);

#endif