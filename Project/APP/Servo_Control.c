#include "headfile.h"

SERVO_PID_PARAMETERS Servo_pid_t;           //���PID
SERVO_CONTROL Servo_t;

extern uint8 outtrack_flag;                             //��������־
extern long ElectromaError_Out_Value;                   //�����������еĲ�Ⱥ�
extern uint8 out_island_flag, into_island_flag;         //������־λ
extern uint16 OUT_island_encoder;    //���뻷���������ֵ
extern long Left_Adc, Right_Adc, Left_Adc2, Right_Adc2, Middle_Adc;		//��һ�����ֵ

extern const uint16 encoder_out_land;

extern uint8 out_island_accelerate;
/**
* @brief    ���Ʋ��������ʼ��
* @return   void
**/

void Servo_Init(void) {
    // ���PID
    if (Left_Adc>60&&Right_Adc>60&& Left_Adc2 == 0 && Right_Adc2 ==0) Servo_pid_t.Dir_Kp = 0.010955f;
    else Servo_pid_t.Dir_Kp = 0.006955f;
    //Servo_pid_t.Dir_Kp = 0.016955f;
    //Servo_pid_t.Dir_Kp = 0.02555f;
    Servo_pid_t.Dir_Kd = 1.376568f;
    //Servo_pid_t.Dir_Kd = 3;
}
/*
* 0.0174 1.395
* 0.0176 1.385
* 0.0179 1.395
*/
/**
* @brief	�������
* @return   void
*/

void Car_Control(void) {
    // ת��������
    if (0 == outtrack_flag) {
        if (into_island_flag == 2) Servo_t.Out_Direction = -150;
        else if (out_island_flag = 1 && OUT_island_encoder > encoder_out_land) Servo_t.Out_Direction = -150;
        else Servo_t.Out_Direction = PID_Servo(ElectromaError_Out_Value, &Servo_pid_t);
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
    s_Steer_Error = (Servo_pid_t->Dir_Kp * error + Servo_pid_t->Dir_Kp * error * fabs(error) + Servo_pid_t->Dir_Kd * (error - s_Last_Steer_Error));
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