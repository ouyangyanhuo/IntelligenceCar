///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,��ɿƼ�
// * All rights reserved.
// * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897(����)  ��Ⱥ��824575535
// *
// * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
// * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
// *
// * @file       		isr
// * @company	   		�ɶ���ɿƼ����޹�˾
// * @author     		��ɿƼ�(QQ790875685)
// * @version    		�鿴doc��version�ļ� �汾˵��
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC32F12K
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/

// ********************************************************************************************************************/
// * ���ж�ϵͳ���õ�TM1��TM2
// * TM1ΪԪ�ش���
// * TM2ΪƵ�ʴ���
// ********************************************************************************************************************/

#include "headfile.h"

//UART1�ж�
void UART1_Isr() interrupt 4
{
    uint8 res;
	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        //�����Զ�����
        if(res == 0x7F)
        {
            if(dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

//UART2�ж�
void UART2_Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
		busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//�������ݼĴ���Ϊ��S2BUF
		if(wireless_module_uart_handler != NULL)
		{
			// �ú���Ϊ����ָ��
			// �ٳ�ʼ������ģ���ʱ�����øú�����ַ
			wireless_module_uart_handler(S2BUF);
		}
	}
}


//UART3�ж�
void UART3_Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
		busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//�������ݼĴ���Ϊ��S3BUF

	}
}


//UART4�ж�
void UART4_Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
		busy[4] = 0;
	}
    if(UART4_GET_RX_FLAG)
	{
        UART4_CLEAR_RX_FLAG;
		//�������ݼĴ���Ϊ��S4BUF;


	}
}

#define LED P52
void INT0_Isr() interrupt 0
{
	LED = 0;	//����LED
}
void INT1_Isr() interrupt 2
{

}
void INT2_Isr() interrupt 10
{
	INT2_CLEAR_FLAG;  //����жϱ�־
}
void INT3_Isr() interrupt 11
{
	INT3_CLEAR_FLAG;  //����жϱ�־
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //����жϱ�־
}

void TM0_Isr() interrupt 1
{

}                                          

extern long  Left_Adc, Right_Adc;						//����ֵ
extern long  Left_Adc2, Right_Adc2;						//�����ֵ
extern int16 temp_right_pluse;							//�ҵ������ֵ

uint16 IN_island_encoder = 0, OUT_island_encoder = 5;   //���뻷���������ֵ

uint16 OUT_accelerate_encoder = 0;						//���������ٱ�����ֵ
uint8 out_island_accelerate = 0;						//���������ٱ�־λ

uint16 GO_accelerate = 0;								//��������
uint8 GO_accelerate_flag = 1;

uint16 Time_outtrack_cnt = 0;							//����������
uint8 outtrack_flag = 0;								//��������־

uint16 encoder_GO_accelerate = 6000, encoder_OUT_accelerate = 5000;

uint16 out_island_accelerate_flag_encoder = 0, out_island_accelerate_flag = 0;
uint8 ZHIZOU = 0;

/*TM1���ִ��뿪ʼ*/
void TM1_Isr() interrupt 3
{
	ADC_Final_Read_Deal();   //��ȡADCֵ
	encoderValue_get();  //���������ݶ�ȡ

	/**********  1.ͣ������-���������� *********/
	if (Left_Adc < 5 && Right_Adc2 < 5 && Left_Adc2 < 5 && Right_Adc < 5) Time_outtrack_cnt++;
	if (Time_outtrack_cnt > 80)
	{
		Time_outtrack_cnt = 0;
		outtrack_flag = 1;
	}
	/**********  1.ͣ������-���������� *********/

	/************ 2.���뻷������ ***********/
	if(Island_t.nolonger ==0){//�����жϻ�
		if (Left_Adc == 100 && Right_Adc > 75 && Left_Adc2 >= 2){
			Island_t.in_cnt++;				//����һ��ֵ��Ϊ�л���
			if (Island_t.in_cnt >= 5){
				Island_t.in_cnt = 0;
				Island_t.into_flag = 1;		//��λ������־
			}
		}
	}
	//ȷ��������ͨ���ۼƱ���ֵ����ձ�־λ��
	if (Island_t.into_flag == 1)  IN_island_encoder += temp_right_pluse;  //�ǵ�һ��ֵ����Ϊ�Ѿ����뻷��
	if (IN_island_encoder >= ENCODER_ISLAND_INTO+2000){			//·�̹���
		Island_t.into_flag = 0;				//��ջ�����־λ
		IN_island_encoder = 0;
		Island_t.out_flag = 1;
		Island_t.nolonger = 1;
	}
	//���뻷��һ��
	if (Island_t.out_flag == 1) OUT_island_encoder += temp_right_pluse;
	if (OUT_island_encoder >= ENCODER_ISLAND_OUT+3000){
		Island_t.out_flag = 0;
		OUT_island_encoder = 0;
		out_island_accelerate_flag = 1;		//������־λ����
		gpio_mode(P3_2, GPIO);				// ��P3.2����Ϊ������
	}
	/************  2.���뻷������ ***********/

	/************  3.ֱ������ ***********/
	if (GO_accelerate_flag == 1) GO_accelerate += temp_right_pluse;
	if (GO_accelerate >= encoder_GO_accelerate) {
		GO_accelerate = 0;
		GO_accelerate_flag = 0;			//�����ع�����
	}
	//�������ӳ�ϵͳ
	if (out_island_accelerate_flag == 1) out_island_accelerate_flag_encoder += temp_right_pluse;
	if (out_island_accelerate_flag_encoder >= 2000) {
		out_island_accelerate_flag_encoder = 0;
		out_island_accelerate_flag = 0;					//��־λ�û�
		out_island_accelerate = 1;						//��ʼ���ٱ�־λ
	}

	if (out_island_accelerate == 1) OUT_accelerate_encoder += temp_right_pluse;
	if (OUT_accelerate_encoder >= 1000) ZHIZOU = 1;		//����������ʱ������У����ִ�ֱ�������ȶ�
	if (OUT_accelerate_encoder >= encoder_OUT_accelerate) {
		OUT_accelerate_encoder = 0;
		out_island_accelerate = 0;		//�����ع�����
	}

	/************  3.ֱ������ ***********/

	//����ϵͳ
	Motor_PWM_Final_Control();	//�������
	Car_Control();				//�������
}


/*TM2���ִ��뿪ʼ*/
void TM2_Isr() interrupt 12
{
	/*


	
	*/
	//TIM2_CLEAR_FLAG;  //����жϱ�־
	
}



void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //����жϱ�־
	
}

void TM4_Isr() interrupt 20
{
	TIM4_CLEAR_FLAG; //����жϱ�־
	ccd_collect();	 //CCD�ɼ�����
//	pit_callback();
}

//void  INT0_Isr()  interrupt 0;
//void  TM0_Isr()   interrupt 1;
//void  INT1_Isr()  interrupt 2;
//void  TM1_Isr()   interrupt 3;
//void  UART1_Isr() interrupt 4;
//void  ADC_Isr()   interrupt 5;
//void  LVD_Isr()   interrupt 6;
//void  PCA_Isr()   interrupt 7;
//void  UART2_Isr() interrupt 8;
//void  SPI_Isr()   interrupt 9;
//void  INT2_Isr()  interrupt 10;
//void  INT3_Isr()  interrupt 11;
//void  TM2_Isr()   interrupt 12;
//void  INT4_Isr()  interrupt 16;
//void  UART3_Isr() interrupt 17;
//void  UART4_Isr() interrupt 18;
//void  TM3_Isr()   interrupt 19;
//void  TM4_Isr()   interrupt 20;
//void  CMP_Isr()   interrupt 21;
//void  I2C_Isr()   interrupt 24;
//void  USB_Isr()   interrupt 25;
//void  PWM1_Isr()  interrupt 26;
//void  PWM2_Isr()  interrupt 27;