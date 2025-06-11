#include "headfile.h"

extern uint8 UI_count, into_island_flag, out_island_flag;
/*���UI����*/
void Electroma_UI(void) {
	encoderValue_get();
	if (UI_count == 0) {
		lcd_showstr(0, 3, "HNTOU SMART CAR");
	}
	else if (UI_count == 4) {
		lcd_showstr(0, 0, "LAD = ");					//����Ԥ�ж�
		lcd_showuint16(8 * 5, 0, into_island_flag);

		lcd_showstr(0, 1, "OUT = ");					//�����ж�
		lcd_showuint16(8 * 5, 1, out_island_flag);

		lcd_showstr(0, 2, "PWM = ");					//L7ԭʼֵ
		lcd_showuint16(8 * 5, 2, Motor_t.Pwm_MotorR);
	}
	else
	{
		lcd_showstr(0, 3, "TOOBIG!");
		lcd_showuint16(8 * 5, 4, UI_count);
	}
}