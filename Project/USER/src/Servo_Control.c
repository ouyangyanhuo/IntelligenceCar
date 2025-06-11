#include "headfile.h"

SERVO_PID_PARAMETERS Servo_pid_t;           //���PID
SERVO_CONTROL Servo_t;
extern uint8 outtrack_flag;                 //��������־
extern long ElectromaError_Out_Value;		//�����������еĲ�Ⱥ�

/**
* @brief    ���Ʋ��������ʼ��
* @return   void
**/

void Servo_Init(void) {
    // ���PID
    Servo_pid_t.Dir_Kp = 2.0;
    Servo_pid_t.Dir_Kd = 0;
}

/**
* @brief	�������
* @return   void
*/
void Car_Control(void) {
    // ת��������
    if (0 == outtrack_flag) {
        Servo_t.Out_Direction = PID_Servo(ElectromaError_Out_Value, &Servo_pid_t);
    }
    else if (1 == outtrack_flag) {
        Servo_t.Out_Direction = PID_Servo(0, &Servo_pid_t); 
    }
    Servo_t.Pwm_Servo = SERVO_MIDDLE + Servo_t.Out_Direction;
    //SERVO_MIDDLE �Ƕ����ֵ
    Servo_Ctrl(Servo_t.Pwm_Servo);
}

/**
* @brief    ��λ��ʽ�����PID
* @param    duty            ռ�ձȴ�С
* @return   int             PID���
**/
uint16 PID_Servo(float error, SERVO_PID_PARAMETERS* Servo_pid_t) {
    static float s_Steer_Error, s_Last_Steer_Error;
    s_Steer_Error = (Servo_pid_t->Dir_Kp * error + Servo_pid_t->Dir_Kd * (error - s_Last_Steer_Error));
    s_Last_Steer_Error = error;
    s_Steer_Error < -(SERVO_DELTA / 2) ? s_Steer_Error = -(SERVO_DELTA / 2) : s_Steer_Error;
    s_Steer_Error > (SERVO_DELTA / 2) ? s_Steer_Error = (SERVO_DELTA / 2) : s_Steer_Error;
    return (s_Steer_Error);
}

/**
* @brief    �������
* @param    duty            ռ�ձȴ�С
* @return   void
**/
void Servo_Ctrl(uint16 Duty) {
    Duty < SERVO_RIGHT ? Duty = SERVO_RIGHT : Duty;
    Duty > SERVO_LEFT ? Duty = SERVO_LEFT : Duty;
    pwm_duty(PWMB_CH3_P33, Duty);
}
//SERVO_RIGHT SERVO_LEFT�Ƕ��������λ

/**
*
* @brief    ���й��ܳ�ʼ��
* @return   void
**/
void Servo_Init_2(void) {
    Servo_Init();
    Car_Control();
}