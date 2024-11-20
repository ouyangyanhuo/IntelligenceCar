#ifndef __SERVO_CONTROL_H_
#define __SERVO_CONTROL_H_


//舵机PID
typedef struct {
	float Dir_Kp;
	float Dir_Kd;
}SERVO_PID_PARAMETERS;

typedef struct {
	uint16 Out_Direction;			// 舵机方向环输出
	uint16 Pwm_Servo;				// 舵机输出
}SERVO_CONTROL;

typedef struct {
	uint8 in_cnt;
	uint8 into_flag;
	uint8 out_flag;
	uint8 nolonger;
}ISLAND_RESOLVE;

extern ISLAND_RESOLVE Island_t;
extern SERVO_PID_PARAMETERS Servo_pid_t;
extern SERVO_CONTROL Servo_t;

/* 舵机限位 */
// 舵机左限位
#define SERVO_LEFT       			(1050)
// 舵机中值
#define SERVO_MIDDLE     			(925)
// 舵机右限位
#define SERVO_RIGHT     			(800)
// 舵机左右转动的差值，与舵机型号，拉杆和舵机臂长有关
#define SERVO_DELTA					(SERVO_LEFT - SERVO_RIGHT)


/***  出入环岛编码 ***/
#define IN_ISLAND_ENCODER_MAX		500
#define OUT_ISLAND_ENCODER_MAX		1200

/***  出入环岛编码 ***/

/***  出入环岛积分 ***/
#define ENCODER_ISLAND_INTO			3000
#define ENCODER_ISLAND_OUT			10500
/***  出入环岛积分 ***/

void Servo_Init(void);
void Car_Control(void);
uint16 PID_Servo(float error, SERVO_PID_PARAMETERS* Servo_pid_t);
void Servo_Ctrl(uint16 Duty);

#endif
