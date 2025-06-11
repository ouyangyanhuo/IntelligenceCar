#include "headfile.h"

SERVO_PID_PARAMETERS Servo_pid_t;           //舵机PID
SERVO_CONTROL Servo_t;
extern uint8 outtrack_flag;                 //出赛道标志
extern long ElectromaError_Out_Value;		//两边两个横电感的差比和

/**
* @brief    控制参数舵机初始化
* @return   void
**/

void Servo_Init(void) {
    // 舵机PID
    Servo_pid_t.Dir_Kp = 2.0;
    Servo_pid_t.Dir_Kd = 0;
}

/**
* @brief	舵机控制
* @return   void
*/
void Car_Control(void) {
    // 转向舵机控制
    if (0 == outtrack_flag) {
        Servo_t.Out_Direction = PID_Servo(ElectromaError_Out_Value, &Servo_pid_t);
    }
    else if (1 == outtrack_flag) {
        Servo_t.Out_Direction = PID_Servo(0, &Servo_pid_t); 
    }
    Servo_t.Pwm_Servo = SERVO_MIDDLE + Servo_t.Out_Direction;
    //SERVO_MIDDLE 是舵机中值
    Servo_Ctrl(Servo_t.Pwm_Servo);
}

/**
* @brief    （位置式）舵机PID
* @param    duty            占空比大小
* @return   int             PID结果
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
* @brief    驱动舵机
* @param    duty            占空比大小
* @return   void
**/
void Servo_Ctrl(uint16 Duty) {
    Duty < SERVO_RIGHT ? Duty = SERVO_RIGHT : Duty;
    Duty > SERVO_LEFT ? Duty = SERVO_LEFT : Duty;
    pwm_duty(PWMB_CH3_P33, Duty);
}
//SERVO_RIGHT SERVO_LEFT是舵机左右限位

/**
*
* @brief    所有功能初始化
* @return   void
**/
void Servo_Init_2(void) {
    Servo_Init();
    Car_Control();
}