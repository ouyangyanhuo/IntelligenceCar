#include "headfile.h"

SERVO_PID_PARAMETERS Servo_pid_t;           //���PID
SERVO_CONTROL Servo_t;
ISLAND_RESOLVE Island_t;

extern uint8 outtrack_flag;                             //��������־
extern long ElectromaError_Out_Value;                   //�����������еĲ�Ⱥ�
extern uint8 out_island_flag, into_island_flag;         //������־λ
extern uint16 IN_island_encoder, OUT_island_encoder;    //���뻷���������ֵ

extern uint8 ZHIZOU;

extern uint8 out_island_accelerate;
/**
* @brief    ���Ʋ��������ʼ��
* @return   void
**/

void Servo_Init(void) {
    // ���PID
    Servo_pid_t.Dir_Kp = 1.74f;
    Servo_pid_t.Dir_Kd = 1.39f;
    //����������ʼ��
    Island_t.into_flag = 0;
    Island_t.out_flag = 0;
    Island_t.nolonger = 0;
    Island_t.in_cnt = 0;
}
/*
1800Duty 0.88 0.6   ��1700�Ż�
*/

/**
* @brief	�������
* @return   void
*/
void Car_Control(void) {
    float Direction_error_get = 0;
    // ת��������
    if (0 == outtrack_flag) {
        if (Island_t.into_flag == 1 && Island_t.out_flag == 0 && IN_island_encoder > ENCODER_ISLAND_INTO) {                                //�뻷��
            Servo_t.Out_Direction = -160;           //��̶��Ƕ�
            gpio_mode(P3_2, GPO_PP);			    // ��P3.2����Ϊ�������
        }
        else if (Island_t.out_flag == 1 && OUT_island_encoder > ENCODER_ISLAND_OUT)  //������
        {
            Servo_t.Out_Direction = -160;          //��̶��Ƕ�
        }
        /*else if (out_island_accelerate == 1 && ZHIZOU == 0) {
            Servo_t.Out_Direction = 0;
        }*/
        else {
            Servo_t.Out_Direction = PID_Servo(ElectromaError_Out_Value, &Servo_pid_t);
        }
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