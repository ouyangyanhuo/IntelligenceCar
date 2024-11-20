#include "headfile.h"

extern uint8 UI_count, time_count;
extern long Left_Adc, Right_Adc, Left_Adc2, Right_Adc2, Middle_Adc;		//归一化电感值
extern long AD_Bias, AD_Bias_last;										//电感偏差值
extern uint16 Time_outtrack_cnt;										//出赛道计数
extern uint16 servo_angle;												//LCD时钟，舵机打角测试
extern uint8 outtrack_flag;												//出赛道Flag

extern int16 temp_right_pluse;											//编码
extern uint16 IN_island_encoder, OUT_island_encoder;
extern int16 debug_left, debug_right, debug_left2, debug_right2, debug_middle;	//电感 Debug用
extern uint16 OUT_accelerate_encoder;								//出环岛加速编码器值
/*电感UI控制*/
void Electroma_UI(void) {
	encoderValue_get();
	if (UI_count == 0) {
		lcd_showstr(0, 3, "HNTOU SMART CAR");
	}
	else if (UI_count == 1){
		stable_UI();

		lcd_showstr(0,2,"L11 = ");					//L1归一化
		lcd_showuint16(8 * 5,2, Left_Adc);

		lcd_showstr(0,3,"L21 = ");					//L2归一化
		lcd_showuint16(8 * 5,3, Left_Adc2);

		lcd_showstr(0,4,"L61 = ");					//L6归一化
		lcd_showuint16(8 * 5,4, Right_Adc2);

		lcd_showstr(0,5,"L71 = ");					//L7归一化
		lcd_showuint16(8 * 5,5, Right_Adc);

		lcd_showstr(0, 6, "MD1 = ");					//L7归一化
		lcd_showuint16(8 * 5, 6, Middle_Adc);

		lcd_showstr(0, 7, "Dif = ");					//左右电感偏差值
		lcd_showuint16(8 * 5, 7, AD_Bias);


	}
	else if (UI_count == 2) {
		stable_UI();

		lcd_showstr(0, 2, "MD2 = ");					//拟合中间电感
		lcd_showuint16(8 * 5, 2, debug_middle);

		lcd_showstr(0, 3, "L12 = ");					//L1原始值
		lcd_showuint16(8 * 5, 3, debug_left);

		lcd_showstr(0, 4, "L21 = ");					//L2原始值
		lcd_showuint16(8 * 5, 4, debug_left2);

		lcd_showstr(0, 5, "L62 = ");					//L6原始值
		lcd_showuint16(8 * 5, 5, debug_right2);

		lcd_showstr(0, 6, "L72 = ");					//L7原始值
		lcd_showuint16(8 * 5, 6, debug_right);
	}
	else if (UI_count == 3)	{
		stable_UI();

		lcd_showstr(0, 2, "LAD = ");					//环岛预判断
		lcd_showuint16(8 * 5, 2, Island_t.into_flag);

		lcd_showstr(0, 3, "OUT = ");					//出岛判断
		lcd_showuint16(8 * 5, 3, Island_t.out_flag);

		lcd_showstr(0, 4, "COD = ");					//编码器数值
		lcd_showuint16(8 * 5, 4, temp_right_pluse);
		
		lcd_showstr(0, 5, "IN = ");						//入环编码器值
		lcd_showuint16(8 * 5, 5, IN_island_encoder);

		lcd_showstr(0, 6, "OT = ");						//出环编码器值
		lcd_showuint16(8 * 5, 6, OUT_island_encoder);

		lcd_showstr(0, 7, "AC = ");						//出环编码器值
		lcd_showuint16(8 * 5, 7, OUT_accelerate_encoder);
	}
	else
	{
		lcd_showstr(0, 3, "TOOBIG!");
		lcd_showuint16(8 * 5, 4, UI_count);
	}
}

void stable_UI(void) {
	lcd_showint16(0, 0, time_count++);				//每0.5秒计时器+1
	lcd_showstr(0, 1, "FLAG= ");						//出赛道检测
	lcd_showuint16(8 * 5, 1, outtrack_flag);
}