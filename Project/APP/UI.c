#include "headfile.h"

extern uint8 UI_count, into_island_flag, out_island_flag;
/*电感UI控制*/
void Electroma_UI(void) {
	encoderValue_get();
	if (UI_count == 0) {
		lcd_showstr(0, 3, "HNTOU SMART CAR");
	}
	else if (UI_count == 4) {
		lcd_showstr(0, 0, "LAD = ");					//环岛预判断
		lcd_showuint16(8 * 5, 0, into_island_flag);

		lcd_showstr(0, 1, "OUT = ");					//出岛判断
		lcd_showuint16(8 * 5, 1, out_island_flag);

		lcd_showstr(0, 2, "PWM = ");					//L7原始值
		lcd_showuint16(8 * 5, 2, Motor_t.Pwm_MotorR);
	}
	else
	{
		lcd_showstr(0, 3, "TOOBIG!");
		lcd_showuint16(8 * 5, 4, UI_count);
	}
}