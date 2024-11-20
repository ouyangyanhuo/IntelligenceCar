#include "headfile.h"
//һ���ܳ�ʼ������

/* �� main ����ѭ����ĳ�ʼ�� */
void System_Int_main(void) {
	lcd_init();							//LCD��Ļ��ʼ��
	Motor_Command_Init();				//�����ʼ��
	pwm_init(PWMB_CH3_P33, 50, 0);		//�����ʼ��
	Adinductance_init();				//��г�ʼ��
	pit_timer_ms(TIM_1, 4);				//4msִ�ж�ʱ��1�ж�
}
/* �� main ����ѭ���ڵĳ�ʼ�� */
void System_Int_while(void) {
	if (P50 == 0) {
		UI_count++;
		lcd_clear(WHITE);
	}
	if (P51 == 0) {
		if (UI_count == 0) {
			lcd_showstr(0, 4, "ERROR");		//��С����
		}
		else {
			UI_count--;
			lcd_clear(WHITE);
		};
	}
	Electroma_UI();
}
/* �� �ж�ϵͳ �еĳ�ʼ�� */
void System_Int_isr(void) {
	Motor_PWM_Final_Control();	//�������
	Servo_Init();				//PID��ʼ��
	Car_Control();				//��������
	encoderValue_get();			//���������ݶ�ȡ
}