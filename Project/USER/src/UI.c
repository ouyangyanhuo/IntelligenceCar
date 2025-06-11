#include "headfile.h"

uint8 UI_count = 0, time_count = 0;
extern uint16 Left_Adc, Right_Adc, Left_Adc2, Right_Adc2, Middle_Adc;		//归一化电感值
extern long AD_Bias, AD_Bias_last;										//电感偏差值
extern uint16 Time_outtrack_cnt;										//出赛道计数
extern uint16 servo_angle;												//LCD时钟，舵机打角测试
extern uint8 outtrack_flag;												//出赛道Flag

extern int16 debug_left, debug_right, debug_left2, debug_right2, debug_middle;	//电感 Debug用

/*按键检测及初始屏幕*/
void UI_Control() {
	if (P50 == 0) {
		UI_count++;
	}
	if (P51 == 0) {
		if (UI_count == 0){
			lcd_showstr(0, 3, "ERROR");		//减小保护
		}
		else UI_count--;
	}
	if (UI_count == 0) {
		lcd_showstr(0, 3, "HNTOU SMART CAR");
		lcd_showstr(0, 4, "ID = ");
		lcd_showuint16(8 * 5, 4, UI_count);
	}
}
/*电感UI控制*/
void Electroma_UI() {
	if (UI_count == 1){
		//lcd_showint16(0, 0, time_count++);				//每0.5秒计时器+1，移入了Main函数

		lcd_showstr(0,1,"FLAG = ");					//拟合中间电感
		lcd_showuint16(8 * 5,1, outtrack_flag);

		lcd_showstr(0,2,"L11 = ");					//L1归一化
		lcd_showuint16(8 * 5,2, Left_Adc);

		lcd_showstr(0,3,"L21 = ");					//L2归一化
		lcd_showuint16(8 * 5,3, Right_Adc);

		lcd_showstr(0,4,"L61 = ");					//L6归一化
		lcd_showuint16(8 * 5,4, Right_Adc2);

		lcd_showstr(0,5,"L71 = ");					//L7归一化
		lcd_showuint16(8 * 5,5, Left_Adc2);

		lcd_showstr(0,6, "Dif = ");					//左右电感偏差值
		lcd_showuint16(8 * 5,6, AD_Bias);

		lcd_showstr(0,7, "OUT = ");					//出赛道计数显示
		lcd_showuint16(8 * 5,7, Time_outtrack_cnt);

		delay_ms(500);
	}
	else if (UI_count == 2) {
		lcd_showint16(0, 0, time_count++);				//每0.5秒计时器+1

		lcd_showstr(0, 1, "MD2 = ");					//拟合中间电感
		lcd_showuint16(8 * 5, 1, debug_middle);

		lcd_showstr(0, 2, "L12 = ");					//L1原始值
		lcd_showuint16(8 * 5, 2, debug_left);

		lcd_showstr(0, 3, "L21 = ");					//L2原始值
		lcd_showuint16(8 * 5, 3, debug_right);

		lcd_showstr(0, 4, "L62 = ");					//L6原始值
		lcd_showuint16(8 * 5, 4, debug_right2);

		lcd_showstr(0, 5, "L72 = ");					//L7原始值
		lcd_showuint16(8 * 5, 5, debug_left2);
	}
}
/*舵机打角*/
void Servo_UI() {
	if (UI_count == 3) {
		lcd_showint16(0, 0, time_count++);				//每0.5秒计时器+1
		lcd_showstr(0, 1, "ANGLE = ");					//出赛道计数显示
		lcd_showuint16(8 * 5, 1, servo_angle);
	}

	//按钮调控舵机打角，步进为 2
	if (P16 == 0) {
		servo_angle += 2;
	}
	if (P17 == 0) {
		servo_angle -= 2;
	}
}
void UI_Init() {
	UI_Control();
	Electroma_UI();
	Servo_UI();
}