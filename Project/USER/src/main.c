/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		main
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ790875685)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2023-07-27

 ********************************************************************************************************************/
#include "headfile.h"
#define P33 PWMB_CH3_P33
#define BEEP P32

// �����ں�Ƶ�ʵ��趨�����Բ鿴board.h�ļ�
// ��board_init��,�Ѿ���P54��������Ϊ��λ
// �����Ҫʹ��P54����,������board.c�ļ��е�board_init()������ɾ��SET_P54_RESRT����

/*    	ÿ��һ�� �ù�У��             ����ΰ� �ȹ�ʡ��        */
/* \\ \\ \\ \\ \\ \\ \\ || || || || || || // // // // // // // //
\\ \\ \\ \\ \\ \\ \\        _ooOoo_          // // // // // // //
\\ \\ \\ \\ \\ \\          o8888888o            // // // // // //
\\ \\ \\ \\ \\             88" . "88               // // // // //
\\ \\ \\ \\                (| -_- |)                  // // // //
\\ \\ \\                   O\  =  /O                     // // //
\\ \\                   ____/`---'\____                     // //
\\                    .'  \\|     |//  `.                      //
==                   /  \\|||  :  |||//  \                     ==
==                  /  _||||| -:- |||||-  \                    ==
==                  |   | \\\  -  /// |   |                    ==
==                  | \_|  ''\---/''  |   |                    ==
==                  \  .-\__  `-`  ___/-. /                    ==
==                ___`. .'  /--.--\  `. . ___                  ==
==              ."" '<  `.___\_<|>_/___.'  >'"".               ==
==            | | :  `- \`.;`\ _ /`;.`/ - ` : | |              \\
//            \  \ `-.   \_ __\ /__ _/   .-` /  /              \\
//      ========`-.____`-.___\_____/___.-`____.-'========      \\
//                           `=---='                           \\
// //   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  \\ \\
// // //      ���汣��      ����BUG      �����޸�			 \\ \\ \\
// // // // // // || || || || || || || || || || \\ \\ \\ \\ \\ */

uint8 UI_count = 0, time_count = 0;
void main()
{
		DisableGlobalIRQ();								//�ر����ж�

		clock_init(SYSTEM_CLOCK_52M);					// ��ʼ��ϵͳƵ��,��ɾ���˾���롣
		board_init();									// ��ʼ���Ĵ���,��ɾ���˾���롣

    // �˴���д�û����� ���������ʼ�������
 
		lcd_init();							//LCD��Ļ��ʼ��
		Motor_Command_Init();				//�����ʼ��
		pwm_init(P33,50,0);					//�����ʼ��
		Servo_Init();						//���������ʼ��
		Adinductance_init();				//��г�ʼ��
		pit_timer_ms(TIM_1, 2);				//2msִ�ж�ʱ��1�ж�
		EnableGlobalIRQ();					//�������ж�

		

    // �˴���д�û����� ���������ʼ�������
	
    while(1)
    {
		// �˴���д��Ҫѭ��ִ�еĴ���
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

		delay_ms(1);
    // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

