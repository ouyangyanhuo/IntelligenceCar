#include "headfile.h"

uint8 UI_count = 0, time_count = 0;
extern uint16 Left_Adc, Right_Adc, Left_Adc2, Right_Adc2, Middle_Adc;		//��һ�����ֵ
extern long AD_Bias, AD_Bias_last;										//���ƫ��ֵ
extern uint16 Time_outtrack_cnt;										//����������
extern uint16 servo_angle;												//LCDʱ�ӣ������ǲ���
extern uint8 outtrack_flag;												//������Flag

extern int16 debug_left, debug_right, debug_left2, debug_right2, debug_middle;	//��� Debug��

/*������⼰��ʼ��Ļ*/
void UI_Control() {
	if (P50 == 0) {
		UI_count++;
	}
	if (P51 == 0) {
		if (UI_count == 0){
			lcd_showstr(0, 3, "ERROR");		//��С����
		}
		else UI_count--;
	}
	if (UI_count == 0) {
		lcd_showstr(0, 3, "HNTOU SMART CAR");
		lcd_showstr(0, 4, "ID = ");
		lcd_showuint16(8 * 5, 4, UI_count);
	}
}
/*���UI����*/
void Electroma_UI() {
	if (UI_count == 1){
		//lcd_showint16(0, 0, time_count++);				//ÿ0.5���ʱ��+1��������Main����

		lcd_showstr(0,1,"FLAG = ");					//����м���
		lcd_showuint16(8 * 5,1, outtrack_flag);

		lcd_showstr(0,2,"L11 = ");					//L1��һ��
		lcd_showuint16(8 * 5,2, Left_Adc);

		lcd_showstr(0,3,"L21 = ");					//L2��һ��
		lcd_showuint16(8 * 5,3, Right_Adc);

		lcd_showstr(0,4,"L61 = ");					//L6��һ��
		lcd_showuint16(8 * 5,4, Right_Adc2);

		lcd_showstr(0,5,"L71 = ");					//L7��һ��
		lcd_showuint16(8 * 5,5, Left_Adc2);

		lcd_showstr(0,6, "Dif = ");					//���ҵ��ƫ��ֵ
		lcd_showuint16(8 * 5,6, AD_Bias);

		lcd_showstr(0,7, "OUT = ");					//������������ʾ
		lcd_showuint16(8 * 5,7, Time_outtrack_cnt);

		delay_ms(500);
	}
	else if (UI_count == 2) {
		lcd_showint16(0, 0, time_count++);				//ÿ0.5���ʱ��+1

		lcd_showstr(0, 1, "MD2 = ");					//����м���
		lcd_showuint16(8 * 5, 1, debug_middle);

		lcd_showstr(0, 2, "L12 = ");					//L1ԭʼֵ
		lcd_showuint16(8 * 5, 2, debug_left);

		lcd_showstr(0, 3, "L21 = ");					//L2ԭʼֵ
		lcd_showuint16(8 * 5, 3, debug_right);

		lcd_showstr(0, 4, "L62 = ");					//L6ԭʼֵ
		lcd_showuint16(8 * 5, 4, debug_right2);

		lcd_showstr(0, 5, "L72 = ");					//L7ԭʼֵ
		lcd_showuint16(8 * 5, 5, debug_left2);
	}
}
/*������*/
void Servo_UI() {
	if (UI_count == 3) {
		lcd_showint16(0, 0, time_count++);				//ÿ0.5���ʱ��+1
		lcd_showstr(0, 1, "ANGLE = ");					//������������ʾ
		lcd_showuint16(8 * 5, 1, servo_angle);
	}

	//��ť���ض����ǣ�����Ϊ 2
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