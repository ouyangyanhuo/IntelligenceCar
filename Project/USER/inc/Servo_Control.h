#ifndef __SERVO_CONTROL_H_
#define __SERVO_CONTROL_H_


//���PID
typedef struct {
	float Dir_Kp;
	float Dir_Kd;
}SERVO_PID_PARAMETERS;

typedef struct {
	uint16 Out_Direction;			// ����������
	uint16 Pwm_Servo;				// ������
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

/* �����λ */
// �������λ
#define SERVO_LEFT       			(1050)
// �����ֵ
#define SERVO_MIDDLE     			(925)
// �������λ
#define SERVO_RIGHT     			(800)
// �������ת���Ĳ�ֵ�������ͺţ����˺Ͷ���۳��й�
#define SERVO_DELTA					(SERVO_LEFT - SERVO_RIGHT)


/***  ���뻷������ ***/
#define IN_ISLAND_ENCODER_MAX		500
#define OUT_ISLAND_ENCODER_MAX		1200

/***  ���뻷������ ***/

/***  ���뻷������ ***/
#define ENCODER_ISLAND_INTO			3000
#define ENCODER_ISLAND_OUT			10500
/***  ���뻷������ ***/

void Servo_Init(void);
void Car_Control(void);
uint16 PID_Servo(float error, SERVO_PID_PARAMETERS* Servo_pid_t);
void Servo_Ctrl(uint16 Duty);

#endif
