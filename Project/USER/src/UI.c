#include "headfile.h"

extern uint8 UI_count, time_count;
extern long Left_Adc, Right_Adc, Left_Adc2, Right_Adc2, Middle_Adc;		//��һ�����ֵ
extern long AD_Bias, AD_Bias_last;										//���ƫ��ֵ
extern uint16 Time_outtrack_cnt;										//����������
extern uint16 servo_angle;												//LCDʱ�ӣ������ǲ���
extern uint8 outtrack_flag;												//������Flag
extern uint8 into_island_flag, out_island_flag;							//������־λ
extern uint16 temp_right_pluse;											//����
extern uint16 IN_island_encoder, OUT_island_encoder;
extern int16 debug_left, debug_right, debug_left2, debug_right2, debug_middle;	//��� Debug��
extern uint16 OUT_accelerate_encoder;								//���������ٱ�����ֵ

extern uint8 out_island_accelerate,ZHIZOU;
/*���UI����*/
void Electroma_UI(void) {
	encoderValue_get();
	if (UI_count == 0) {
		lcd_showstr(0, 3, "HNTOU SMART CAR");
	}
	else if (UI_count == 1){
		lcd_showstr(0, 0, "FLAG= ");						//���������
		lcd_showuint16(8 * 5, 0, outtrack_flag);

		lcd_showstr(0,1,"L11 = ");					//L1��һ��
		lcd_showuint16(8 * 5,1, Left_Adc);

		lcd_showstr(0,2,"L21 = ");					//L2��һ��
		lcd_showuint16(8 * 5,2, Left_Adc2);

		lcd_showstr(0,3,"L61 = ");					//L6��һ��
		lcd_showuint16(8 * 5,3, Right_Adc2);

		lcd_showstr(0,4,"L71 = ");					//L7��һ��
		lcd_showuint16(8 * 5,4, Right_Adc);

		lcd_showstr(0, 5, "MD1 = ");					//L7��һ��
		lcd_showuint16(8 * 5, 5, Middle_Adc);

		lcd_showstr(0, 6, "Dif = ");					//���ҵ��ƫ��ֵ
		lcd_showuint16(8 * 5, 6, AD_Bias);


	}
	else if (UI_count == 2) {
		lcd_showstr(0, 0, "MD2 = ");					//����м���
		lcd_showuint16(8 * 5, 0, debug_middle);

		lcd_showstr(0, 1, "L12 = ");					//L1ԭʼֵ
		lcd_showuint16(8 * 5, 1, debug_left);

		lcd_showstr(0, 2, "L21 = ");					//L2ԭʼֵ
		lcd_showuint16(8 * 5, 2, debug_left2);

		lcd_showstr(0, 3, "L62 = ");					//L6ԭʼֵ
		lcd_showuint16(8 * 5, 3, debug_right2);

		lcd_showstr(0, 4, "L72 = ");					//L7ԭʼֵ
		lcd_showuint16(8 * 5, 4, debug_right);

		lcd_showstr(0, 5, "PWM = ");					//L7ԭʼֵ
		lcd_showuint16(8 * 5, 5, Motor_t.Pwm_MotorR);
	}
	else if (UI_count == 3) {
		lcd_showstr(0, 0, "LAD = ");					//����Ԥ�ж�
		lcd_showuint16(8 * 5, 0, into_island_flag);

		lcd_showstr(0, 1, "OUT = ");					//�����ж�
		lcd_showuint16(8 * 5, 1, out_island_flag);

		lcd_showstr(0, 2, "COD = ");					//��������ֵ
		lcd_showuint16(8 * 5, 2, temp_right_pluse);

		lcd_showstr(0, 3, "IN = ");						//�뻷������ֵ
		lcd_showuint16(8 * 5, 3, IN_island_encoder);

		lcd_showstr(0, 4, "OT = ");						//����������ֵ
		lcd_showuint16(8 * 5, 4, OUT_island_encoder);

		lcd_showstr(0, 5, "AC = ");						//����������ֵ
		lcd_showuint16(8 * 5, 5, OUT_accelerate_encoder);

		lcd_showstr(0, 6, "ZHI = ");						//ֱ��
		lcd_showuint16(8 * 5, 6, ZHIZOU);

		lcd_showstr(0, 7, "CHI = ");						//ֱ��
		lcd_showuint16(8 * 5, 7, out_island_accelerate);
	}
	else
	{
		lcd_showstr(0, 3, "TOOBIG!");
		lcd_showuint16(8 * 5, 4, UI_count);
	}
}