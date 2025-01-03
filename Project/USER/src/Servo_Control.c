#include "headfile.h"

SERVO_PID_PARAMETERS Servo_pid_t;           //舵机PID
SERVO_CONTROL Servo_t;

extern uint8 outtrack_flag;                             //出赛道标志
extern long ElectromaError_Out_Value;                   //两边两个横电感的差比和
extern uint8 out_island_flag, into_island_flag;         //环岛标志位
extern uint16 IN_island_encoder, OUT_island_encoder;    //出入环岛编码计数值

extern uint16 encoder_into_land, encoder_out_land;
extern uint8 ZHIZOU;

extern uint8 out_island_accelerate;
/**
* @brief    控制参数舵机初始化
* @return   void
**/

void Servo_Init(void) {
    // 舵机PID
    Servo_pid_t.Dir_Kp = 0.0177578665f;
    Servo_pid_t.Dir_Kd = 1.354568f;
}
/*
* 0.0174 1.395
* 0.0176 1.385
* 0.0179 1.395
*/
/**
* @brief	舵机控制
* @return   void
*/

void Car_Control(void) {
    float Direction_error_get = 0;
    // 转向舵机控制
    if (0 == outtrack_flag) {
        if (into_island_flag == 1 && out_island_flag == 0 && IN_island_encoder > encoder_into_land) {                                //入环岛
            Servo_t.Out_Direction = -160;           //打固定角度
            gpio_mode(P3_2, GPO_PP);			    // 将P3.2设置为推挽输出
        }
        else if (out_island_flag == 1 && OUT_island_encoder > encoder_out_land)  //出环岛
        {
            Servo_t.Out_Direction = -160;          //打固定角度
            gpio_mode(P3_2, GPIO);			// 将P3.2设置为弱上拉
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
    s_Steer_Error = (Servo_pid_t->Dir_Kp * error + Servo_pid_t->Dir_Kp * error * fabs(error) + Servo_pid_t->Dir_Kd * (error - s_Last_Steer_Error));
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