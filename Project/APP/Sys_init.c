#include "headfile.h"
//一个总初始化程序

/* 在 main 函数循环外的初始化 */
void System_Int_main(void) {
	lcd_init();							//LCD屏幕初始化
	Motor_Command_Init();				//电机初始化
	pwm_init(PWMB_CH3_P33, 50, 0);		//舵机初始化
	Adinductance_init();				//电感初始化
	pit_timer_ms(TIM_1, 4);				//4ms执行定时器1中断
}
/* 在 main 函数循环内的初始化 */
void System_Int_while(void) {
	if (P50 == 0) {
		UI_count++;
		lcd_clear(WHITE);
	}
	if (P51 == 0) {
		if (UI_count == 0) {
			lcd_showstr(0, 4, "ERROR");		//减小保护
		}
		else {
			UI_count--;
			lcd_clear(WHITE);
		};
	}
	Electroma_UI();
}
/* 在 中断系统 中的初始化 */
void System_Int_isr(void) {
	Motor_PWM_Final_Control();	//电机控制
	Servo_Init();				//PID初始化
	Car_Control();				//车辆控制
	encoderValue_get();			//编码器数据读取
}